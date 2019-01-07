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
#include "stdafx.h"
#include "DXSample.h"
#include "CWin32D3D12Donuts.h"

using namespace Microsoft::WRL;

DXSample::DXSample(CWin32D3D12Donuts*sender)
{
	m_sender = sender;
}
 
void DXSample::InitDXArg(UINT width, UINT height, std::wstring name)
{
	m_width = width;
	m_height = height;
	m_title = name;
	m_useWarpDevice = true;
	WCHAR assetsPath[512];
	GetAssetsPath(assetsPath, _countof(assetsPath));
	m_assetsPath = assetsPath;
	m_aspectRatio = static_cast<float>(width) / static_cast<float>(height);
}

DXSample::~DXSample()
{

}

// ��ȡ��Դȫ·��
std::wstring DXSample::GetAssetFullPath(LPCWSTR assetName)
{
	return m_assetsPath + assetName;
}

// ��ȡ֧��  Direct3D 12 �ĵ�һ������������
// ���û�ҵ���*ppAdapter ��Ϊ nullptr.

_Use_decl_annotations_

void DXSample::GetHardwareAdapter(IDXGIFactory2* pFactory, IDXGIAdapter1** ppAdapter)
{
	ComPtr<IDXGIAdapter1> adapter;
	*ppAdapter = nullptr;
	for (UINT adapterIndex = 0; DXGI_ERROR_NOT_FOUND != pFactory->EnumAdapters1(adapterIndex, &adapter); ++adapterIndex)
	{

		DXGI_ADAPTER_DESC1 desc;
		adapter->GetDesc1(&desc);
		if (desc.Flags & DXGI_ADAPTER_FLAG_SOFTWARE)
		{
			// ��ѡ�������Ⱦ����������.
			// ���Ҫ���������,����"/warp" ��������.
			continue;
		}
		// ����������Ƿ�֧�� Direct3D 12,��������ʵ���豸
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}
	*ppAdapter = adapter.Detach();
}

// ���ô��ڱ����ı�
void DXSample::SetCustomWindowText(LPCWSTR text)
{
	std::wstring windowText = m_title + L" " + text;
	SetWindowText(m_sender->GethWnd(), windowText.c_str());
}

// �����κ������в���
_Use_decl_annotations_
void DXSample::ParseCommandLineArgs(WCHAR* argv[], int argc)
{
	for (int i = 1; i < argc; ++i)
	{
		if (_wcsnicmp(argv[i], L"-warp", wcslen(argv[i])) == 0 ||
			_wcsnicmp(argv[i], L"/warp", wcslen(argv[i])) == 0)
		{
			m_useWarpDevice = true;
			m_title = m_title + L" (WARP)";
		}
	}
}