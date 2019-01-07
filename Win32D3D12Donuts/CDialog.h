#pragma once
class CWin32D3D12Donuts;
class CDialog
{
	CWin32D3D12Donuts*m_sender;
public:
	static INT_PTR CALLBACK DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	CDialog(CWin32D3D12Donuts*sender);
	~CDialog();
	void ShowDialog(int Idd);
	virtual INT_PTR CALLBACK CDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
};