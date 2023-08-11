#pragma once



class MainDlg
{
public:
    MainDlg()
        :hInst_(0)
        , hbrBkgnd_(0)
        , hDlg_(0)
    {
        InitializeDefaultConfig();
    }

    HRESULT Initialize(HINSTANCE hInstance);
    INT_PTR DoModal();

    ULONG GetIPAddress(TCHAR szIP[], ULONG nSize);
    WORD GetPortNumber();
    DWORD GetConnCount();

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

private:
    // config
    TCHAR szIP_[16];
    WORD wPort_;
    DWORD dwConnCount_;

    HRESULT InitializeDefaultConfig();
    HRESULT ReadConfig(LPCTSTR lpszCfgFile);
    HRESULT WriteConfig(LPCTSTR lpszCfgFile);
};
