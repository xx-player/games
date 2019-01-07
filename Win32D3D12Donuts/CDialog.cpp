#include "stdafx.h"
#include "CDialog.h"
#include "CWin32D3D12Donuts.h"
INT_PTR CALLBACK CDialog::DialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	if (message == WM_INITDIALOG) SetWindowLongPtr(hDlg, DWLP_USER, lParam);
	CDialog*p = (CDialog*)GetWindowLongPtr(hDlg, DWLP_USER);
	if (!p) return 0;
	return p->CDialogProc(hDlg, message, wParam, lParam);

}
CDialog::CDialog(CWin32D3D12Donuts*sender) {
	m_sender = sender;
}
CDialog::~CDialog() {


}
void CDialog::ShowDialog(int Idd) {
	if (!m_sender) return;
	DialogBoxParam(m_sender->GethInstance(),
		MAKEINTRESOURCE(Idd),
		m_sender->GethWnd(),
		DialogProc, (LPARAM)this);
}


// “关于”框的消息处理程序。
INT_PTR CALLBACK CDialog::CDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		return (INT_PTR)TRUE;

	case WM_COMMAND:
		if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
		{
			EndDialog(hDlg, LOWORD(wParam));
			return (INT_PTR)TRUE;
		}
		break;
	}
	return (INT_PTR)FALSE;
}
