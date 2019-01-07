// Win32D3D12Raiden.cpp : 定义应用程序的入口点。
//
#include "stdafx.h"
#include "CWin32D3D12Donuts.h"
int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: 在此放置代码。
	CWin32D3D12Donuts Application(hInstance, nCmdShow);
	return Application.Go();
}

