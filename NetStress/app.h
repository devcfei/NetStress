#pragma once





class App
{
public:
    App();
    ~App();
    HRESULT Initialize(HINSTANCE hInstance);
    HRESULT Run();
    HRESULT Start();
    HRESULT Stop();
    BOOL IsStart();

    HRESULT GetAppPath(TCHAR *szAppPath, int nSize)
    {
        return StringCchCopy(szAppPath, nSize, szAppPath_);
    }
private:
    HINSTANCE hInstance_;
    TCHAR szAppPath_[MAX_PATH];
    BOOL bStart_;



    MainDlg dlgMain_;

    typedef struct _THREAD_DATA
    {
        HANDLE hThread;
        DWORD dwSendCount;
        DWORD dwID;
        App* pApp;
    }THREAD_DATA;

    THREAD_DATA *threadDataArr_;
    //HANDLE* hThreadArr_;
    //DWORD *dwCountArr_;
    static DWORD WINAPI WorkerThreadProc(LPVOID lpParam);
    DWORD WorkerThread(DWORD dwID);



    HANDLE hExitEvent_;
};