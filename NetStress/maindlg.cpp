#include "framework.h"


HRESULT MainDlg::Initialize(HINSTANCE hInstance)
{
    HRESULT hr = S_OK;
    hInst_ = hInstance;
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
    SetWindowText(hWndIP, _T("127.0.0.1"));
    HWND hWndPort = GetDlgItem(hDlg, IDC_EDIT_PORT);
    SetWindowText(hWndPort, _T("10240"));
    HWND hWndConnCount = GetDlgItem(hDlg, IDC_EDIT_CONN_COUNT);
    SetWindowText(hWndConnCount, _T("10"));

    

    //
    TCHAR lpszLatin[] = L"Lorem ipsum dolor sit amet, consectetur "
        L"adipisicing elit, sed do eiusmod tempor "
        L"incididunt ut labore et dolore magna "
        L"aliqua. Ut enim ad minim veniam, quis "
        L"nostrud exercitation ullamco laboris nisi "
        L"ut aliquip ex ea commodo consequat. Duis "
        L"aute irure dolor in reprehenderit in "
        L"voluptate velit esse cillum dolore eu "
        L"fugiat nulla pariatur. Excepteur sint "
        L"occaecat cupidatat non proident, sunt "
        L"in culpa qui officia deserunt mollit "
        L"anim id est laborum.";

    HWND hWndEditLog = GetDlgItem(hDlg, IDC_EDIT_LOG);
    SetWindowText(hWndEditLog, lpszLatin);


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
        EndDialog(hDlg, LOWORD(wParam));
        r = (INT_PTR)TRUE;
        break;

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