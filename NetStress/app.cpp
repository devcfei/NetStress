#include "framework.h"

App::App()
    :hInstance_(NULL)
    ,bStart_(FALSE)
{
    ZeroMemory(szAppPath_, sizeof(szAppPath_));
}

App::~App()
{

}

HRESULT App::Initialize(HINSTANCE hInstance)
{
    HRESULT hr = S_OK;
    hInstance_ = hInstance;


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
    bStart_ = TRUE;
    return hr;
}

HRESULT App::Stop()
{
    HRESULT hr = S_OK;
    bStart_ = FALSE;

    return hr;
}

BOOL App::IsStart()
{
    return bStart_;
}
