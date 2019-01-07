#include "stdafx.h"
#include "CWin32D3D12Donuts.h"
#include "Win32D3D12Donuts.h"
LRESULT CALLBACK CWin32D3D12Donuts::WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	if (message == WM_CREATE) {
		LPCREATESTRUCT lpcs =(LPCREATESTRUCT)lParam;
		SetWindowLongPtr(hWnd, GWLP_USERDATA,(LONG)lpcs->lpCreateParams);
	}
	CWin32D3D12Donuts*p = (CWin32D3D12Donuts*)GetWindowLongPtr(hWnd, GWLP_USERDATA);
	if (!p) return DefWindowProc(hWnd, message, wParam, lParam);
	return p->CWndProc(hWnd, message, wParam, lParam);
}
CWin32D3D12Donuts::CWin32D3D12Donuts(HINSTANCE hInstance, int nCmdShow):
	m_about(this),
	m_guide(this),
	m_graphics(this),
	m_bQuit(FALSE)
{
	m_hInstance = hInstance;
	m_nCmdShow = nCmdShow;
}
CWin32D3D12Donuts::~CWin32D3D12Donuts()
{


}
int CWin32D3D12Donuts::Go() {
	// 初始化全局字符串
	LoadStringW(m_hInstance, IDS_APP_TITLE, m_szTitle, MAX_LOADSTRING);
	LoadStringW(m_hInstance, IDC_WIN32D3D12RAIDEN, m_szWindowClass, MAX_LOADSTRING);
	WNDCLASSEXW wcex;
	wcex.cbSize = sizeof(WNDCLASSEX);
	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = m_hInstance;
	wcex.hIcon = LoadIcon(m_hInstance, MAKEINTRESOURCE(IDI_WIN32D3D12RAIDEN));
	wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCEW(IDC_WIN32D3D12RAIDEN);
	wcex.lpszClassName = m_szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));
	RegisterClassExW(&wcex);
	RECT rc = { 0,0,800,600 };
	AdjustWindowRect(&rc, WS_OVERLAPPEDWINDOW, TRUE);
	// 执行应用程序初始化: 
	m_hWnd = CreateWindowW(m_szWindowClass, m_szTitle, WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,CW_USEDEFAULT,rc.right-rc.left,rc.bottom-rc.top, nullptr, nullptr,
		m_hInstance, this);
	if (!m_hWnd) return FALSE;
	DWORD dwStyle = GetWindowLong(m_hWnd, GWL_STYLE);
	dwStyle &= ~WS_MAXIMIZEBOX;
	dwStyle &= ~WS_THICKFRAME;
	SetWindowLong(m_hWnd, GWL_STYLE,dwStyle);

	ShowWindow(m_hWnd, m_nCmdShow);
	UpdateWindow(m_hWnd);

	UINT uThreadID = 0;
	_beginthreadex(nullptr, 0,(_beginthreadex_proc_type) Online, this, 0, &uThreadID);

	HACCEL hAccelTable = LoadAccelerators(m_hInstance, MAKEINTRESOURCE(IDC_WIN32D3D12RAIDEN));
	MSG msg;
	// 主消息循环: 
	while (GetMessage(&msg, nullptr, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	m_bQuit = TRUE;
	return (int)msg.wParam;

}
LRESULT CALLBACK CWin32D3D12Donuts::CWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
	case WM_COMMAND:HandleWmCommand(hWnd, message, wParam, lParam);break;
	case WM_PAINT:HandleWmPaint(hWnd, message, wParam, lParam);break;
	case WM_KEYDOWN:HandleWmKeyDown(hWnd, message, wParam, lParam);break;
	case WM_KEYUP:  HandleWmKeyUp(hWnd, message, wParam, lParam);break;
	case WM_DESTROY:PostQuitMessage(0);	break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
LRESULT CALLBACK CWin32D3D12Donuts::HandleWmPaint(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
	PAINTSTRUCT ps;
	HDC hdc = BeginPaint(hWnd, &ps);
	// TODO: 在此处添加使用 hdc 的任何绘图代码...
	EndPaint(hWnd, &ps);
	return 0;
}
LRESULT CALLBACK CWin32D3D12Donuts::HandleWmCommand(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId = LOWORD(wParam);
	// 分析菜单选择: 
	switch (wmId)
	{
	case IDM_ABOUT:	m_about.ShowDialog(IDD_ABOUTBOX);break;
	case IDM_GUIDE:	m_guide.ShowDialog(IDD_GUIDE);break;
	case IDM_EXIT:	DestroyWindow(hWnd);break;
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
DWORD WINAPI CWin32D3D12Donuts::Online(LPVOID lpParam) {
	CWin32D3D12Donuts*p = (CWin32D3D12Donuts*)lpParam;
	p->m_graphics.CreateEnvironment();
	while (!p->m_bQuit)
	{
		p->m_graphics.OnUpdate();
		p->m_graphics.OnRender();
		SleepEx(1, TRUE);
	}
    p->m_graphics.OnDestroy();
	p->m_graphics.ReleaseEnvironment();
	return 0;
}
LRESULT CALLBACK CWin32D3D12Donuts::HandleWmKeyDown(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_graphics.OnKeyDown(static_cast<UINT8>(wParam));
	return 0;
}
LRESULT CALLBACK CWin32D3D12Donuts::HandleWmKeyUp(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	m_graphics.OnKeyUp(static_cast<UINT8>(wParam));
	return 0;
}
