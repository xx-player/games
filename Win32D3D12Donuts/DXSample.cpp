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

// 获取资源全路径
std::wstring DXSample::GetAssetFullPath(LPCWSTR assetName)
{
	return m_assetsPath + assetName;
}

// 获取支持  Direct3D 12 的第一个可用适配器
// 如果没找到，*ppAdapter 设为 nullptr.

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
			// 不选择基本渲染驱动适配器.
			// 如果要软件适配器,放置"/warp" 在命令行.
			continue;
		}
		// 检查适配器是否支持 Direct3D 12,但不建立实际设备
		if (SUCCEEDED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_11_0, _uuidof(ID3D12Device), nullptr)))
		{
			break;
		}
	}
	*ppAdapter = adapter.Detach();
}

// 设置窗口标题文本
void DXSample::SetCustomWindowText(LPCWSTR text)
{
	std::wstring windowText = m_title + L" " + text;
	SetWindowText(m_sender->GethWnd(), windowText.c_str());
}

// 构成任何命令行参数
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