#pragma once



class MainDlg
{
public:
    MainDlg()
        :hInst_(0)
        , hbrBkgnd_(0)
    {

    }

    HRESULT Initialize(HINSTANCE hInstance);
    INT_PTR DoModal();

    ULONG GetIPAddress(TCHAR szIP[], ULONG nSize);
    WORD GetPortNumber();
    ULONG GetConnCount();

    VOID PrintMessage(LPCTSTR fmt, ...);

private:
    HINSTANCE hInst_;
    HBRUSH hbrBkgnd_;
    HWND hDlg_;

    static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
    INT_PTR OnInitDialog(HWND, UINT, WPARAM, LPARAM);
    INT_PTR OnColorStatic(HWND, UINT, WPARAM, LPARAM);
    INT_PTR OnCommand(HWND, UINT, WPARAM, LPARAM);

    std::tstring msg_;
};
