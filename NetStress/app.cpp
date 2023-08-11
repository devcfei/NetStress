#include "framework.h"

App::App()
    :hInstance_(NULL)
    ,bStart_(FALSE)
{
    ZeroMemory(szAppPath_, sizeof(szAppPath_));
}

App::~App()
{
    if (threadDataArr_)
    {
        delete[] threadDataArr_;
    }
}

HRESULT App::Initialize(HINSTANCE hInstance)
{
    HRESULT hr = S_OK;
    hInstance_ = hInstance;


    // Get application path
    GetModuleFileName(NULL, szAppPath_, MAX_PATH);
    PathRemoveFileSpec(szAppPath_);


    // WSAStartup
    WSADATA wsaData;
    if ((WSAStartup(MAKEWORD(2, 2), &wsaData)) != 0) {
        return HRESULT_FROM_WIN32(GetLastError());
    }

    // initiaize dialog
    dlgMain_.Initialize(hInstance);

 
    return hr;
}

HRESULT App::Run()
{
    HRESULT hr = S_OK;

    dlgMain_.DoModal();

    return hr;
}

HRESULT App::Start()
{
    HRESULT hr = S_OK;

    ULONG conncnt=dlgMain_.GetConnCount();


    threadDataArr_ = new THREAD_DATA[conncnt];

    for (ULONG i = 0; i < conncnt; ++i)
    {
        HANDLE hThread = INVALID_HANDLE_VALUE;
        DWORD dwThreadId = 0;

        threadDataArr_[i].hThread = hThread;
        threadDataArr_[i].dwSendCount = 0;
        threadDataArr_[i].dwID = i;
        threadDataArr_[i].pApp = this;

        hThread = CreateThread(NULL, 0, WorkerThreadProc, (LPVOID) & threadDataArr_[i], 0, &dwThreadId);
        if (hThread == NULL) {
            hr = HRESULT_FROM_WIN32(GetLastError());
            LOGF(_T("CreateThread failed! hr = %08X"), hr);
            return hr;

        }


    }

    hExitEvent_ = CreateEvent(0, TRUE, 0, 0);

    bStart_ = TRUE;


    return hr;
}

HRESULT App::Stop()
{
    HRESULT hr = S_OK;
    bStart_ = FALSE;

    SetEvent(hExitEvent_);

    return hr;
}

BOOL App::IsStart()
{
    return bStart_;
}





DWORD WINAPI App::WorkerThreadProc(LPVOID lpParam)
{
    THREAD_DATA* pData = (THREAD_DATA*)lpParam;
    return pData->pApp->WorkerThread(pData->dwID);
}

DWORD App::WorkerThread(DWORD dwID)
{
    int r;
    ADDRINFOT* result = NULL,
        * ptr = NULL,
        hints;
    SOCKET sockConn = INVALID_SOCKET;

    DWORD EventTotal = 0;
    TCHAR szIPAddr[16];
    TCHAR szPort[16];
    WORD wPort;
    dlgMain_.GetIPAddress(szIPAddr, 16);
    wPort = dlgMain_.GetPortNumber();
    StringCchPrintf(szPort, 16, _T("%d"), wPort);

    WSABUF wsabufSend = { 0 };
    WSABUF wsabufRecv = { 0 };

    dlgMain_.PrintMessage(_T("start thread"));



    __try
    {


        // 
        ZeroMemory(&hints, sizeof(hints));
        hints.ai_family = AF_UNSPEC;
        hints.ai_socktype = SOCK_STREAM;
        hints.ai_protocol = IPPROTO_TCP;
        // Resolve the server address and port
        r = GetAddrInfo(szIPAddr, szPort, &hints, &result);
        if (r) {
            DWORD dwErr = GetLastError();
            LOGF(_T("GetAddrInfo failed - ERROR: %d\n"), dwErr);
            __leave;
        }

        for (ptr = result; ptr != NULL; ptr = ptr->ai_next) {

            // Create a SOCKET for connecting to server
            sockConn = WSASocket(ptr->ai_family, ptr->ai_socktype, ptr->ai_protocol, NULL, 0, WSA_FLAG_OVERLAPPED);
            if (sockConn == INVALID_SOCKET) {
                DWORD dwErr = GetLastError();
                LOGF(_T("create socket failed - ERROR: %d\n"), dwErr);
                __leave;
            }

            // Connect to server.
            r = connect(sockConn, ptr->ai_addr, (int)ptr->ai_addrlen);
            if (r == SOCKET_ERROR) {
                closesocket(sockConn);
                sockConn = INVALID_SOCKET;
                continue;
            }
            break;
        }


        DWORD dwSendFlags = 0;
        DWORD dwRecvFlags = 0;
        DWORD dwSendBytes = 0;
        DWORD dwRecvBytes = 0;

        WSAEVENT EventArray[WSA_MAXIMUM_WAIT_EVENTS];
        WSAOVERLAPPED wsaolSend;
        ZeroMemory(&wsaolSend, sizeof(WSAOVERLAPPED));
        wsaolSend.hEvent = WSACreateEvent();
        WSAOVERLAPPED wsaolRecv;
        ZeroMemory(&wsaolSend, sizeof(WSAOVERLAPPED));
        wsaolRecv.hEvent = WSACreateEvent();


        EventArray[EventTotal] = hExitEvent_;
        EventTotal++;

        BOOL bRun = TRUE;

        wsabufSend.len = 4096;
        wsabufSend.buf = new CHAR[wsabufSend.len];

        wsabufRecv.len = 4096;
        wsabufRecv.buf = new CHAR[wsabufSend.len];


        
        r = WSARecv(
            sockConn,
            &wsabufRecv,
            1,
            &dwRecvBytes,
            &dwRecvFlags,
            &wsaolRecv,
            NULL);
        if (r == SOCKET_ERROR)
        {
            DWORD dwErr = WSAGetLastError();
            if (WSA_IO_PENDING != dwErr)
            {
                closesocket(sockConn);
                __leave;
            }
        }


        *(DWORD*)wsabufSend.buf = threadDataArr_[dwID].dwSendCount++;

        r = WSASend(
            sockConn,
            &wsabufSend,
            1,
            &dwSendBytes,
            dwSendFlags,
            &wsaolSend,
            NULL);
        if (r == SOCKET_ERROR)
        {
            DWORD dwErr = WSAGetLastError();
            if (WSA_IO_PENDING != dwErr)
            {
                closesocket(sockConn);
                __leave;
            }
        }


        while (bRun)
        {
            BOOL bResult;
            DWORD dwWaitRet = WSAWaitForMultipleEvents(EventTotal, EventArray, FALSE, 100, FALSE);

            switch (dwWaitRet)
            {
            case WSA_WAIT_EVENT_0:
                bRun = FALSE; // exit
                break;

            case WSA_WAIT_EVENT_0 + 1: // send completed

                bResult = WSAGetOverlappedResult(sockConn, &wsaolSend, &dwSendBytes, FALSE, &dwSendFlags);
                if (bResult)
                {

                }

                if (dwSendBytes == 0)
                {
                    closesocket(sockConn);
                    bRun = FALSE;
                }

                dlgMain_.PrintMessage(_T("send packet length %d"), dwSendBytes);

                WSAResetEvent(EventArray[dwWaitRet - WSA_WAIT_EVENT_0]);

                *(DWORD*)wsabufSend.buf = threadDataArr_[dwID].dwSendCount++;
                r = WSASend(
                    sockConn,
                    &wsabufSend,
                    1,
                    &dwSendBytes,
                    dwSendFlags,
                    &wsaolSend,
                    NULL);
                if (r == SOCKET_ERROR)
                {
                    DWORD dwErr = WSAGetLastError();
                    if (WSA_IO_PENDING != dwErr)
                    {
                        closesocket(sockConn);
                        __leave;
                    }
                }


                break;
            case WSA_WAIT_EVENT_0 + 2:
                WSAResetEvent(EventArray[dwWaitRet - WSA_WAIT_EVENT_0]);

                break;

            case WSA_WAIT_TIMEOUT:

                *(DWORD*)wsabufSend.buf = threadDataArr_[dwID].dwSendCount++;

                r = WSASend(
                    sockConn,
                    &wsabufSend,
                    1,
                    &dwSendBytes,
                    dwSendFlags,
                    &wsaolSend,
                    NULL);
                if (r == SOCKET_ERROR)
                {
                    DWORD dwErr = WSAGetLastError();
                    if (WSA_IO_PENDING != dwErr)
                    {
                        closesocket(sockConn);
                        __leave;
                    }
                }
                dlgMain_.PrintMessage(_T("send packet length %d"), dwSendBytes);


                break;
            default:
                break;
            }
        }

    }
    __finally
    {
        if (wsabufSend.buf)
        {
            delete[] wsabufSend.buf;
        }
        if (wsabufRecv.buf)
        {
            delete[] wsabufRecv.buf;
        }

        closesocket(sockConn);

    }


    dlgMain_.PrintMessage(_T("exit thread"));

    return 0;
}