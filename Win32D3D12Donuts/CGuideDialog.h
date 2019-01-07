#pragma once
#include "CDialog.h"
class CGuideDialog :public CDialog
{
public:
	CGuideDialog(CWin32D3D12Donuts*sender);
	~CGuideDialog();
	virtual INT_PTR CALLBACK CDialogProc(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam);
	VOID InitDialog(HWND hDlg);
};