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


    HANDLE* hThreadArr_;
    static DWORD WINAPI WorkerThreadProc(LPVOID lpParam);
    DWORD WorkerThread();

    HANDLE hExitEvent_;
};