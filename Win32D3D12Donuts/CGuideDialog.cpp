#include "stdafx.h"
#include "CGuideDialog.h"
#include "Win32D3D12Donuts.h"
CGuideDialog::CGuideDialog(CWin32D3D12Donuts*sender) :CDialog(sender)
{


}
CGuideDialog::~CGuideDialog()
{

}
// ָ�϶Ի��������
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
		L"��ͷ������\r\n"
		L"��ͷ������\r\n"
		L"��ͷ������\r\n"
		L"��ͷ������\r\n"
		L"��ĸA ����\r\n"
		L"��ĸD ����\r\n"
		L"��ĸW �����\r\n"
		L"��ĸS ����\r\n"
		L"�ո�  ���\r\n"
		L"��ĸP ��/������"
	);
	SetDlgItemText(hDlg, IDC_EDIT1, szMsg);
}