#pragma once
#include "Win32D3D12Donuts.h"
#include "CDialog.h"
#include "CGraphics.h"
#include "CGuideDialog.h"
class CWin32D3D12Donuts
{
#define MAX_LOADSTRING 100
	HWND m_hWnd;
	// 全局变量: 
	HINSTANCE m_hInstance;                            // 当前实例
	WCHAR m_szTitle[MAX_LOADSTRING];                  // 标题栏文本
	WCHAR m_szWindowClass[MAX_LOADSTRING];            // 主窗口类名
	int m_nCmdShow;
public:
	inline TCHAR*GetTitle() { return m_szTitle; }
	static DWORD WINAPI Online(LPVOID lpParam);
	inline HWND GethWnd() { return m_hWnd; }
	inline HINSTANCE GethInstance() { return m_hInstance; }
	static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	CWin32D3D12Donuts(HINSTANCE hInstance,int nCmdShow);
	~CWin32D3D12Donuts();
	int Go();
	LRESULT CALLBACK CWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleWmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleWmCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleWmKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	LRESULT CALLBACK HandleWmKeyUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);
	CDialog m_about;
	CGuideDialog m_guide;
	CGraphics m_graphics;
	BOOL m_bQuit;
};