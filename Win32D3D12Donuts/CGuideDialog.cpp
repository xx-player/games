#include "stdafx.h"
#include "CGuideDialog.h"
#include "Win32D3D12Donuts.h"
CGuideDialog::CGuideDialog(CWin32D3D12Donuts*sender) :CDialog(sender)
{


}
CGuideDialog::~CGuideDialog()
{

}
// 指南对话框处理程序。
INT_PTR CALLBACK CGuideDialog::CDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	UNREFERENCED_PARAMETER(lParam);
	switch (message)
	{
	case WM_INITDIALOG:
		InitDialog(hDlg);
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

VOID CGuideDialog::InitDialog(HWND hDlg)
{
	TCHAR szMsg[MAX_PATH];
	StringCbPrintf(szMsg, MAX_PATH, L"%s",
		L"箭头←左行\r\n"
		L"箭头→右行\r\n"
		L"箭头↑上行\r\n"
		L"箭头↓下行\r\n"
		L"字母A 左旋\r\n"
		L"字母D 右旋\r\n"
		L"字母W 解除盾\r\n"
		L"字母S 开盾\r\n"
		L"空格  射击\r\n"
		L"字母P 开/关音乐"
	);
	SetDlgItemText(hDlg, IDC_EDIT1, szMsg);
}