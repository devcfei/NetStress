#include "framework.h"


HRESULT MainDlg::Initialize(HINSTANCE hInstance)
{
    HRESULT hr = S_OK;
    hInst_ = hInstance;

    TCHAR szCfgPath[MAX_PATH];
    GetApp()->GetAppPath(szCfgPath, MAX_PATH);
    StringCchCat(szCfgPath, MAX_PATH, _T("\\NetStress.ini"));



    BOOL bFileExist = FALSE;
    DWORD dwAttrib = GetFileAttributes(szCfgPath);
    if (dwAttrib != INVALID_FILE_ATTRIBUTES && !(dwAttrib & FILE_ATTRIBUTE_DIRECTORY))
    {
        bFileExist = TRUE;
        hr = S_OK;
    }

    if (!bFileExist)
    {
        LOGW(_T("NetStress.ini doesn't exist, create it"));
        hr = S_OK;
        WriteConfig(szCfgPath);
    }
    else
    {
        ReadConfig(szCfgPath);
    }

    return hr;
}


INT_PTR MainDlg::DoModal()
{
    INT_PTR r = 0;

    DialogBoxParam(hInst_, MAKEINTRESOURCE(IDD_MAIN), NULL, DialogProc, (LPARAM)this);

    return r;
}

INT_PTR CALLBACK MainDlg::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR r = (INT_PTR)FALSE;
    UNREFERENCED_PARAMETER(lParam);
    static MainDlg* pThis;
    switch (message)
    {
    case WM_INITDIALOG:
        pThis = (MainDlg*)lParam;

        pThis->hDlg_ = hDlg;
        r = pThis->OnInitDialog(hDlg, message, wParam, lParam);
        break;

    case WM_CTLCOLORSTATIC:

        r = pThis->OnColorStatic(hDlg, message, wParam, lParam);
        break;

    case WM_COMMAND:

        r = pThis->OnCommand(hDlg, message, wParam, lParam);
        break;
    }

    return r;
}


HRESULT MainDlg::InitializeDefaultConfig()
{
    HRESULT hr = S_OK;

    StringCchCopy(szIP_, 16, _T("127.0.0.1"));
    wPort_ = 10240;
    dwConnCount_ = 1;
    return hr;
}

HRESULT MainDlg::ReadConfig(LPCTSTR lpszCfgFile)
{
    HRESULT hr = S_OK;

    // [SETTINGS]
    GetPrivateProfileString(_T("SETTINGS"), _T("IP"), _T("127.0.0.1"), szIP_, 16, lpszCfgFile);
    wPort_ = GetPrivateProfileInt(_T("SETTINGS"), _T("PORT"), 1433, lpszCfgFile);
    dwConnCount_ = GetPrivateProfileInt(_T("SETTINGS"), _T("CONNCOUNT"), 1433, lpszCfgFile);


    return hr;
}

HRESULT MainDlg::WriteConfig(LPCTSTR lpszCfgFile)
{
    HRESULT hr = S_OK;

    TCHAR value[32];

    // [SETTINGS]
    WritePrivateProfileString(_T("SETTINGS"), _T("IP"), szIP_, lpszCfgFile);
    StringCchPrintf(value, 32, _T("%d"), wPort_);
    WritePrivateProfileString(_T("SETTINGS"), _T("PORT"), value, lpszCfgFile);
    StringCchPrintf(value, 32, _T("%d"), dwConnCount_);
    WritePrivateProfileString(_T("SETTINGS"), _T("CONNCOUNT"), value, lpszCfgFile);

    return hr;
}

INT_PTR MainDlg::OnInitDialog(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    // make window in the center
    RECT rect;
    GetWindowRect(hDlg, &rect);
    int nWidth = rect.right - rect.left;
    int nHeight = rect.bottom - rect.top;

    int iScreenWidth = GetSystemMetrics(SM_CXSCREEN);
    int iScreenHeight = GetSystemMetrics(SM_CYSCREEN);

    int X = (iScreenWidth - nWidth) / 2;
    int Y = (iScreenHeight - nHeight) / 2;
    SetWindowPos(hDlg, NULL, X, Y, nWidth, nHeight, SWP_NOSIZE);

    // set dialog icon
    HICON hIcon = LoadIcon(hInst_, MAKEINTRESOURCE(IDI_FAVICON));
    SendMessage(hDlg, WM_SETICON, ICON_SMALL, (LPARAM)hIcon);
    SendMessage(hDlg, WM_SETICON, ICON_BIG, (LPARAM)hIcon);

    // bold the checkbox title
    //HWND hwnd;
    //HFONT hFontB, hFont;
    //LOGFONT lf;
    //hwnd = GetDlgItem(hDlg, IDC_CHECK_SERVER_MAINTAINANCE);
    //hFont = (HFONT)SendMessage(hwnd, WM_GETFONT, 0, 0);
    //GetObject(hFont, sizeof(LOGFONT), &lf);
    //lf.lfWeight = FW_BOLD;
    //hFontB = CreateFontIndirect(&lf);
    //SendMessage(hwnd, WM_SETFONT, (WPARAM)hFontB, TRUE);


    // create a global brush for log
    hbrBkgnd_ = CreateSolidBrush(RGB(0, 0, 0));


    HWND hWndIP = GetDlgItem(hDlg, IDC_IPADDRESS);
    SetWindowText(hWndIP, szIP_);
    HWND hWndPort = GetDlgItem(hDlg, IDC_EDIT_PORT);
    TCHAR szText[32];
    StringCchPrintf(szText, 32, _T("%d"), wPort_);
    SetWindowText(hWndPort, szText);
    HWND hWndConnCount = GetDlgItem(hDlg, IDC_EDIT_CONN_COUNT);
    StringCchPrintf(szText, 32, _T("%d"), dwConnCount_);
    SetWindowText(hWndConnCount, szText);

    PrintMessage(_T("Application initialized success!"));

    return (INT_PTR)TRUE;
}
INT_PTR MainDlg::OnColorStatic(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR r = (INT_PTR)FALSE;
    HDC hdc = (HDC)wParam;
    HWND hwnd = (HWND)lParam;

    int iCtrlID = GetDlgCtrlID(hwnd);

    switch (iCtrlID)
    {
    case IDC_EDIT_LOG:
        SetTextColor(hdc, RGB(200, 200, 200));
        SetBkColor(hdc, RGB(0, 0, 0));
        r = (INT_PTR)hbrBkgnd_;
        break;
    }

    return r;
}

INT_PTR MainDlg::OnCommand(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    INT_PTR r = (INT_PTR)FALSE;

    WORD idCtrlID = LOWORD(wParam);

    switch (idCtrlID)
    {
    case IDCANCEL:
    {
        TCHAR szCfgPath[MAX_PATH];
        GetApp()->GetAppPath(szCfgPath, MAX_PATH);
        StringCchCat(szCfgPath, MAX_PATH, _T("\\NetStress.ini"));
        // Get current settings

        TCHAR szText[16];

        HWND hWndIP = GetDlgItem(hDlg_, IDC_IPADDRESS);
        GetWindowText(hWndIP, szIP_, 16);

        HWND hWndPort = GetDlgItem(hDlg_, IDC_EDIT_PORT);
        GetWindowText(hWndPort, szText, 16);
        wPort_ = StrToInt(szText);


        HWND hWndConnCnt = GetDlgItem(hDlg_, IDC_EDIT_CONN_COUNT);
        GetWindowText(hWndConnCnt, szText, 16);
        dwConnCount_ = StrToInt(szText);

        WriteConfig(szCfgPath);

        EndDialog(hDlg, LOWORD(wParam));
        r = (INT_PTR)TRUE;
        break;
    }

    case IDC_BTN_STARTSTOP:
        if (GetApp()->IsStart())
        {
            GetApp()->Stop();
            SetWindowText((HWND)lParam, _T("START"));
        }
        else
        {
            GetApp()->Start();
            SetWindowText((HWND)lParam, _T("STOP"));
        }

        r = (INT_PTR)TRUE;
        break;
    }
    return r;
}

ULONG MainDlg::GetIPAddress(TCHAR szIP[], ULONG nSize)
{
    HWND hWndIP = GetDlgItem(hDlg_, IDC_IPADDRESS);
    return GetWindowText(hWndIP, szIP, nSize);
}

WORD MainDlg::GetPortNumber()
{
    HWND hWndPort = GetDlgItem(hDlg_, IDC_EDIT_PORT);
    TCHAR szText[16];
    GetWindowText(hWndPort, szText, 16);
    return StrToInt(szText);
}

DWORD MainDlg::GetConnCount()
{
    HWND hWndConnCnt = GetDlgItem(hDlg_, IDC_EDIT_CONN_COUNT);
    TCHAR szText[16];
    GetWindowText(hWndConnCnt, szText, 16);
    return StrToInt(szText);
}

VOID MainDlg::PrintMessage(LPCTSTR fmt,...)
{
    HRESULT hr;
    TCHAR szMessageBuffer[1024] = { 0 };
    va_list valist;
    va_start(valist, fmt);
    hr = StringCchVPrintf(szMessageBuffer, 1024, fmt, valist);
    if (FAILED(hr))
    {
        return ;
    }
    va_end(valist);


    msg_ += szMessageBuffer;
    msg_ += _T("\r\n");
    dwLineCount_++;

    HWND hWndEditLog = GetDlgItem(hDlg_, IDC_EDIT_LOG);
    SetWindowText(hWndEditLog, msg_.c_str());
    SendMessage(hWndEditLog, EM_LINESCROLL, 0, dwLineCount_);
}
