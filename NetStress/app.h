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
private:
    HINSTANCE hInstance_;
    TCHAR szAppPath_[MAX_PATH];
    BOOL bStart_;

    MainDlg dlgMain_;
};