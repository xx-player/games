//*********************************************************
//
// Copyright (c) Microsoft. All rights reserved.
// This code is licensed under the MIT License (MIT).
// THIS CODE IS PROVIDED *AS IS* WITHOUT WARRANTY OF
// ANY KIND, EITHER EXPRESS OR IMPLIED, INCLUDING ANY
// IMPLIED WARRANTIES OF FITNESS FOR A PARTICULAR
// PURPOSE, MERCHANTABILITY, OR NON-INFRINGEMENT.
//
//*********************************************************
#pragma once
#include "DXSampleHelper.h"
class CWin32D3D12Donuts;
class DXSample
{
public:
	DXSample(CWin32D3D12Donuts*sender);

	virtual ~DXSample();
	virtual BOOL OnInit() = 0;
	virtual void OnUpdate() = 0;
	virtual void OnRender() = 0;
	virtual void OnDestroy() = 0;
	// �����ؿ���ָ����Ϣ���¼����
	virtual void OnKeyDown(UINT8 /*key*/) {}
	virtual void OnKeyUp(UINT8 /*key*/) {}

	// ������
	UINT GetWidth() const { return m_width; }
	UINT GetHeight() const { return m_height; }
	const WCHAR* GetTitle() const { return m_title.c_str(); }

	void ParseCommandLineArgs(_In_reads_(argc) WCHAR* argv[], int argc);
	void InitDXArg(UINT width, UINT height, std::wstring name);
	std::wstring GetAssetFullPath(LPCWSTR assetName);

protected:
	void GetHardwareAdapter(_In_ IDXGIFactory2* pFactory, _Outptr_result_maybenull_ IDXGIAdapter1** ppAdapter);
	void SetCustomWindowText(LPCWSTR text);

	// ��Ƶ�˿�ά��
	UINT m_width;
	UINT m_height;
	float m_aspectRatio;

	// ��������Ϣ
	bool m_useWarpDevice;

	CWin32D3D12Donuts*m_sender;
private:
	// ����Դ·��
	std::wstring m_assetsPath;
	// ���ڱ���
	std::wstring m_title;

};