/******************************************************************************************************
**                                                                                                   **
**    D3D12  控制流                                                                                  **
**                                                                                                   **
**              湖北·恩施 xx_player   2019.1                                                        **
**                                                                                                   **
******************************************************************************************************/
#include"stdafx.h"
#include "CGraphics.h"
#include "CWin32D3D12Donuts.h"
#include "CGames.h"
CGraphics::CGraphics(CWin32D3D12Donuts*sender)
	:DXSample(sender), m_games(this),
	m_frameIndex(0),
	m_frameCounter(0),
	m_fenceValue(0),
	m_rtvDescriptorSize(0),
	m_cbvSrvDescriptorSize(0),
	m_currentFrameResourceIndex(0)
{
	m_sender = sender;

}
CGraphics::~CGraphics()
{
	for (int i = 0;i < FrameCount;i++) {
		if (m_cbvUploadHeap[i]) m_cbvUploadHeap[i]->Unmap(0, nullptr);
		if (m_pConstantBuffers[i]) m_pConstantBuffers[i] = nullptr;
	}

}
BOOL CGraphics::OnInit() {
	ZeroMemory(m_szErrorMsg, MAX_PATH);
	TCHAR szMsg[MAX_PATH];

	m_games.LoadSoundFile();

	HRESULT hr = LoadPipeline();
	if (FAILED(hr)) {
		StringCbPrintf(szMsg, MAX_PATH, L"%s 失败! hr=%s 错误码=0X%x ", (lstrlen(m_szErrorMsg)>0) ? m_szErrorMsg : L"LoadPipeline()", GetDXGIError(hr), HRESULT_FROM_WIN32(GetLastError()));
		MessageBox(m_sender->GethWnd(), szMsg, L"提示", MB_OK);
		return FALSE;
	}
	hr = LoadAssets();
	if (FAILED(hr)) {
		StringCbPrintf(szMsg, MAX_PATH, L"%s 失败! hr=%s 错误码=0X%x ", (lstrlen(m_szErrorMsg)>0) ? m_szErrorMsg : L"LoadAssets()", GetDXGIError(hr), HRESULT_FROM_WIN32(GetLastError()));
		MessageBox(m_sender->GethWnd(), szMsg, L"提示", MB_OK);
		return FALSE;
	}
	return TRUE;
}

// 更新帧基础的值.
void CGraphics::OnUpdate()
{
	m_timer.Tick(NULL);

	if (m_frameCounter == 500)
	{
		// 用FPS值修改窗口标题
		wchar_t fps[64];
		swprintf_s(fps, L"%s[%ufps]",m_sender->GetTitle(), m_timer.GetFramesPerSecond());
		SetCustomWindowText(fps);
		m_frameCounter = 0;
	}
	m_frameCounter++;

	const UINT64 lastCompletedFence = m_fence->GetCompletedValue();
	// 移到下一帧资源.
	m_currentFrameResourceIndex = (m_currentFrameResourceIndex + 1) % FrameCount;

	// 保证帧资源不在被GPU使用，如果在用，等直到完成
	if (m_cbvfenceValue[m_currentFrameResourceIndex] != 0 && m_cbvfenceValue[m_currentFrameResourceIndex] > lastCompletedFence)
	{
		HRESULT hr = m_fence->SetEventOnCompletion(m_cbvfenceValue[m_currentFrameResourceIndex], m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

	m_games.GetInput(m_currentFrameResourceIndex);

	//游戏控制
	UINT64 dwElap = m_timer.GetElapsedTicks();
	m_games.Update(m_currentFrameResourceIndex, (float)dwElap);

	//修改常量缓冲区
	SceneConstantBuffer scb;
	XMMATRIX matWorld, matTexture;
	Set2DEnvironment();

	//传送矩阵位置
	UINT nTotalModelCount = m_games.GetMaxObjectsCount();
	//更新矩阵
	for (UINT j = 0; j < nTotalModelCount; j++)
	{
			matWorld = XMLoadFloat4x4(&m_games.m_objectCollections[m_currentFrameResourceIndex][j].modelMatrices);
			matTexture = XMLoadFloat4x4(&m_games.m_objectCollections[m_currentFrameResourceIndex][j].textureMatrices);
			XMStoreFloat4x4(&scb.mvp,XMMatrixTranspose(matWorld * m_matView * m_matProj));
			XMStoreFloat4x4(&scb.tt, XMMatrixTranspose(matTexture));
			memcpy(&m_pConstantBuffers[m_currentFrameResourceIndex][j], &scb, sizeof(scb));
	}
	//同步帧
	for (UINT i = 0;i < FrameCount;i++) {
		if (i != m_currentFrameResourceIndex) {
			m_games.m_objectCollections[i] = m_games.m_objectCollections[m_currentFrameResourceIndex];
		}
	}

}

// 渲染场景
void CGraphics::OnRender()
{
	// 记录所有需要渲染场景到命令表的命令.
	PopulateBegin();
	PopulateCommandList();
	PopulateEnd();

	// 出席并修改帧索引到下一帧
	HRESULT hr = m_swapChain->Present(1, 0);

	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();
	m_cbvfenceValue[m_currentFrameResourceIndex] = m_fenceValue;
	hr = m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
	m_fenceValue++;
}

void CGraphics::OnDestroy()
{
	// 保证 GPU 不再引用资源，被析构函数清理
	const UINT64 fence = m_fenceValue;
	const UINT64 lastCompletedFence = m_fence->GetCompletedValue();

	// 信号并增加栅栏值.
	HRESULT hr = m_commandQueue->Signal(m_fence.Get(), m_fenceValue);
	m_fenceValue++;
	// 等直到先前帧完成.
	if (lastCompletedFence < fence)
	{
		hr = m_fence->SetEventOnCompletion(fence, m_fenceEvent);
		WaitForSingleObject(m_fenceEvent, INFINITE);
	}

}

void CGraphics::OnKeyDown(UINT8 key)
{
	m_games.OnKeyDown(key);
}

void CGraphics::OnKeyUp(UINT8 key)
{
	m_games.OnKeyUp(key);
}


BOOL CGraphics::CreateEnvironment() {
	RECT rc;
	GetClientRect(m_sender->GethWnd(), &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
	m_viewport = CD3DX12_VIEWPORT(0.0f, 0.0f, static_cast<float>(width), static_cast<float>(height));
	m_scissorRect = CD3DX12_RECT(0, 0, static_cast<LONG>(width), static_cast<LONG>(height));

	InitDXArg(width, height, GetTitle());

	BOOL b = OnInit();
	if (!b) return FALSE;

	srand((unsigned int)time(NULL));

	return TRUE;
}
VOID CGraphics::ReleaseEnvironment() {


}

// 载入渲染管道依赖
HRESULT CGraphics::LoadPipeline()
{
	HRESULT hr = S_OK;
	//1、建立工厂
	hr = CreateFactory();
	if (FAILED(hr)) return hr;
	//2、建立设备
	hr = CreateDevice();
	if (FAILED(hr)) return hr;
	//3、填写表述并建立命令队列
	hr = CreateCommandQueue();
	if (FAILED(hr)) return hr;
	//4、填写表述并建立交换链
	hr = CreateSwapChain();
	if (FAILED(hr)) return hr;
	//5、填写描述并建立描述堆.
	hr = CreateHeap();
	if (FAILED(hr)) return hr;
	//6、建立命令分配器
	hr = CreateCommandAllocator();
	if (FAILED(hr)) return hr;
	return S_OK;
}
// 载入示例资源
HRESULT CGraphics::LoadAssets()
{
	HRESULT hr = S_OK;

	hr = CreateRootSignature();
	if (FAILED(hr)) return hr;
	hr = CreatePipelineState();
	if (FAILED(hr)) return hr;
	hr = CreateCommandList();
	if (FAILED(hr)) return hr;
	hr = CreateRenderTargetView();
	if (FAILED(hr)) return hr;
	hr = CreateResource();
	if (FAILED(hr)) return hr;
	hr = CreateVertex();
	if (FAILED(hr)) return hr;
	hr = CreateIndex();
	if (FAILED(hr)) return hr;
	// 创建纹理和采样器
	hr = CreateSampler();
	if (FAILED(hr)) return hr;
	// 创建深度模板视
	hr = CreateDepthStencilView();
	if (FAILED(hr)) return hr;
	//
	ExecCommandList();

	// 创建同步对象并等直到资源上传到 GPU.
	hr = m_device->CreateFence(m_fenceValue, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&m_fence));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateFence");
	m_fenceValue++;

	// 建立帧同步的事件句柄
	m_fenceEvent = CreateEvent(nullptr, FALSE, FALSE, nullptr);
	if (m_fenceEvent == nullptr)
	{
		hr = HRESULT_FROM_WIN32(GetLastError());
		if (FAILED(hr)) return hr;
	}

	// 等命令表执行;在主循环再用统一命令表，在继续之前，要等安装完成
	// 信号并增加栅栏值
	const UINT64 fenceToWaitFor = m_fenceValue;
	hr = m_commandQueue->Signal(m_fence.Get(), fenceToWaitFor);
	if (FAILED(hr)) return ThrowHrWithError(hr, L"Signal");
	m_fenceValue++;

	// 等直到栅栏完成.
	hr = m_fence->SetEventOnCompletion(fenceToWaitFor, m_fenceEvent);
	if (FAILED(hr)) return ThrowHrWithError(hr, L"SetEventOnCompletion");
	WaitForSingleObject(m_fenceEvent, INFINITE);

	hr = CreateConstantBuffer();
	if (FAILED(hr)) return hr;

	return S_OK;
}
HRESULT CGraphics::CreateFactory()
{
	UINT dxgiFactoryFlags = 0;
	HRESULT hr = S_OK;
#if defined(_DEBUG)
	// 可能调试层(需要图形工具"optional feature").
	// NOTE: 在设备建立后可能调试层将不可用激活设备.
	{
		ComPtr<ID3D12Debug> debugController;
		if (SUCCEEDED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugController))))
		{
			debugController->EnableDebugLayer();
			// 可能附加的调试层
			dxgiFactoryFlags |= DXGI_CREATE_FACTORY_DEBUG;
		}
	}
#endif
	//建立DXGI工厂
	hr = CreateDXGIFactory2(dxgiFactoryFlags, IID_PPV_ARGS(&m_factory));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateDXGIFactory2");
	return hr;
}
HRESULT CGraphics::CreateDevice() {
	HRESULT hr = S_OK;
	if (m_useWarpDevice)
	{
		ComPtr<IDXGIAdapter> warpAdapter;
		hr = m_factory->EnumWarpAdapter(IID_PPV_ARGS(&warpAdapter));
		if (FAILED(hr)) return FALSE;
		hr = D3D12CreateDevice(
			warpAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		);
		if (FAILED(hr)) return ThrowHrWithError(hr, L"D3D12CreateDevice");
	}
	//b、硬设备
	else
	{
		ComPtr<IDXGIAdapter1> hardwareAdapter;
		GetHardwareAdapter(m_factory.Get(), &hardwareAdapter);
		hr = D3D12CreateDevice(
			hardwareAdapter.Get(),
			D3D_FEATURE_LEVEL_11_0,
			IID_PPV_ARGS(&m_device)
		);
		if (FAILED(hr)) return ThrowHrWithError(hr, L"D3D12CreateDevice");
	}
	return hr;

}
HRESULT CGraphics::CreateSwapChain() {
	HRESULT hr = S_OK;
	//填写表述并建立交换链
	DXGI_SWAP_CHAIN_DESC1 swapChainDesc = {};
	swapChainDesc.BufferCount = FrameCount;
	swapChainDesc.Width = m_width;
	swapChainDesc.Height = m_height;
	swapChainDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
	swapChainDesc.SampleDesc.Count = 1;
	ComPtr<IDXGISwapChain1> swapChain;
	hr = m_factory->CreateSwapChainForHwnd(
		m_commandQueue.Get(),
		m_sender->GethWnd(),
		&swapChainDesc,
		nullptr,
		nullptr,
		&swapChain
	);
	if (FAILED(hr)) return hr;

	// 本例不支持满屏
	hr = m_factory->MakeWindowAssociation(m_sender->GethWnd(), DXGI_MWA_NO_ALT_ENTER);
	//5、关联并获取当前后台缓冲索引
	hr = swapChain.As(&m_swapChain);
	m_frameIndex = m_swapChain->GetCurrentBackBufferIndex();

	return hr;
}
HRESULT CGraphics::CreateCommandQueue() {
	HRESULT hr = S_OK;
	D3D12_COMMAND_QUEUE_DESC queueDesc = {};
	queueDesc.Flags = D3D12_COMMAND_QUEUE_FLAG_NONE;
	queueDesc.Type = D3D12_COMMAND_LIST_TYPE_DIRECT;
	hr = m_device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&m_commandQueue));
	if (FAILED(hr)) return hr;
	NAME_D3D12_OBJECT(m_commandQueue);
	return hr;
}
HRESULT CGraphics::CreateHeap() {
	HRESULT hr = S_OK;
	//描述堆
	//6、填写描述并建立渲染目标视(RTV)描述堆.
	D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
	rtvHeapDesc.NumDescriptors = FrameCount;
	rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;
	rtvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&m_rtvHeap));
	if (FAILED(hr)) return hr;
	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

	//7、 深度视(DSV)描述堆
	D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
	dsvHeapDesc.NumDescriptors = 1;
	dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;
	dsvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_NONE;
	hr = m_device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&m_dsvHeap));
	if (FAILED(hr)) return hr;
	//8、着色资源视和常量缓冲区视(CBV)描述堆.
	D3D12_DESCRIPTOR_HEAP_DESC cbvSrvHeapDesc = {};
	cbvSrvHeapDesc.NumDescriptors = 1000+FrameCount* m_games.GetMaxObjectsCount();
	cbvSrvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	cbvSrvHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = m_device->CreateDescriptorHeap(&cbvSrvHeapDesc, IID_PPV_ARGS(&m_cbvSrvHeap));
	if (FAILED(hr)) return hr;

	//9、填写描述并建立采样着色资源视 (SRV) 堆
	D3D12_DESCRIPTOR_HEAP_DESC samplerHeapDesc = {};
	samplerHeapDesc.NumDescriptors = 1;
	samplerHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
	samplerHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	hr = m_device->CreateDescriptorHeap(&samplerHeapDesc, IID_PPV_ARGS(&m_samplerHeap));
	if (FAILED(hr)) return hr;

	m_rtvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);
	m_cbvSrvDescriptorSize = m_device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV);
	//

	return hr;
}
HRESULT CGraphics::CreateCommandAllocator() {
	HRESULT hr = S_OK;
	hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_commandAllocator));
	if (FAILED(hr)) return hr;
	return hr;
}


HRESULT CGraphics::CreateRootSignature() {
	HRESULT hr = S_OK;
	//检查根签名
	D3D12_FEATURE_DATA_ROOT_SIGNATURE featureData = {};
	featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_1;
	if (FAILED(m_device->CheckFeatureSupport(D3D12_FEATURE_ROOT_SIGNATURE, &featureData, sizeof(featureData))))
	{
		featureData.HighestVersion = D3D_ROOT_SIGNATURE_VERSION_1_0;
	}
	//着色器行范围
	CD3DX12_DESCRIPTOR_RANGE1 ranges[3];
	ranges[0].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SRV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);//t0  
	ranges[1].Init(D3D12_DESCRIPTOR_RANGE_TYPE_SAMPLER, 1, 0);//s0
	ranges[2].Init(D3D12_DESCRIPTOR_RANGE_TYPE_CBV, 1, 0, 0, D3D12_DESCRIPTOR_RANGE_FLAG_DATA_STATIC);//b0

	CD3DX12_ROOT_PARAMETER1 rootParameters[3];
	rootParameters[0].InitAsDescriptorTable(1, &ranges[0], D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[1].InitAsDescriptorTable(1, &ranges[1], D3D12_SHADER_VISIBILITY_PIXEL);
	rootParameters[2].InitAsDescriptorTable(1, &ranges[2], D3D12_SHADER_VISIBILITY_VERTEX);

	CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
	rootSignatureDesc.Init_1_1(_countof(rootParameters), rootParameters, 0, nullptr, D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT);

	ComPtr<ID3DBlob> signature;
	ComPtr<ID3DBlob> error;
	hr = D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, featureData.HighestVersion, &signature, &error);
	if (FAILED(hr)) return hr;
	hr = m_device->CreateRootSignature(0,
		signature->GetBufferPointer(),
		signature->GetBufferSize(),
		IID_PPV_ARGS(&m_rootSignature));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateRootSignature");
	NAME_D3D12_OBJECT(m_rootSignature);
	return hr;
}
HRESULT CGraphics::CreatePipelineState() {
	HRESULT hr = S_OK;
	//运行版
	//// 建立管道状态，包含载入着色器.
	//UINT8* pVertexShaderData;
	//UINT8* pPixelShaderData;
	//UINT vertexShaderDataLength;
	//UINT pixelShaderDataLength;
	//hr = ReadDataFromFile(GetAssetFullPath(L"shader_vertex.cso").c_str(), &pVertexShaderData, &vertexShaderDataLength);
	//if (FAILED(hr)) return hr;
	//hr = ReadDataFromFile(GetAssetFullPath(L"shader_pixel.cso").c_str(), &pPixelShaderData, &pixelShaderDataLength);
	//if (FAILED(hr)) return hr;
	//CD3DX12_RASTERIZER_DESC rasterizerStateDesc(D3D12_DEFAULT);
	//rasterizerStateDesc.CullMode = D3D12_CULL_MODE_NONE;
	//// 描述并创建图形管道状态对象(PSO).
	//D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	//psoDesc.InputLayout = { SampleAssets::StandardVertexDescription, SampleAssets::StandardVertexDescriptionNumElements };
	//psoDesc.pRootSignature = m_rootSignature.Get();
	//psoDesc.VS = CD3DX12_SHADER_BYTECODE(pVertexShaderData, vertexShaderDataLength);
	//psoDesc.PS = CD3DX12_SHADER_BYTECODE(pPixelShaderData, pixelShaderDataLength);
	//psoDesc.RasterizerState = rasterizerStateDesc;
	//psoDesc.BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	//psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	//psoDesc.SampleMask = UINT_MAX;
	//psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	//psoDesc.NumRenderTargets = 1;
	//psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	//psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	//psoDesc.SampleDesc.Count = 1;
	//hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	//if (FAILED(hr)) return hr;
	//NAME_D3D12_OBJECT(m_pipelineState);

	//delete pVertexShaderData;
	//delete pPixelShaderData;

	//编译版
	ComPtr<ID3DBlob> vertexShader;
	ComPtr<ID3DBlob> pixelShader;
#if defined(_DEBUG)
	// 可能用图形调试工具较好的着色调试.
	UINT compileFlags = D3DCOMPILE_DEBUG | D3DCOMPILE_SKIP_OPTIMIZATION;
#else
	UINT compileFlags = 0;
#endif
	// 编译着色器
	hr = D3DCompileFromFile(GetAssetFullPath(L"shader_vertex.hlsl").c_str(), nullptr, nullptr, "VSMain", "vs_5_0", compileFlags, 0, &vertexShader, nullptr);
	if (FAILED(hr)) return ThrowHrWithError(hr, L"D3DCompileFromFile");
	hr = D3DCompileFromFile(GetAssetFullPath(L"shader_pixel.hlsl").c_str(), nullptr, nullptr, "PSMain", "ps_5_0", compileFlags, 0, &pixelShader, nullptr);
	if (FAILED(hr)) return ThrowHrWithError(hr, L"D3DCompileFromFile");

	const D3D12_INPUT_ELEMENT_DESC StandardVertexDescription[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
		{ "TANGENT",  0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D12_INPUT_CLASSIFICATION_PER_VERTEX_DATA, 0 },
	};
	UINT StandardVertexDescriptionNumElements = _countof(StandardVertexDescription);

	D3D12_RASTERIZER_DESC RasterizerState = CD3DX12_RASTERIZER_DESC(D3D12_DEFAULT);
	RasterizerState.FillMode = D3D12_FILL_MODE_SOLID;
	RasterizerState.CullMode = D3D12_CULL_MODE_NONE;
	CD3DX12_BLEND_DESC BlendState = CD3DX12_BLEND_DESC(D3D12_DEFAULT);
	BlendState.RenderTarget[0].BlendEnable = TRUE;
	BlendState.RenderTarget[0].LogicOpEnable = false;
	BlendState.RenderTarget[0].SrcBlend = D3D12_BLEND_SRC_ALPHA;
	BlendState.RenderTarget[0].DestBlend = D3D12_BLEND_INV_SRC_ALPHA;
	BlendState.RenderTarget[0].BlendOp = D3D12_BLEND_OP_ADD;
	BlendState.RenderTarget[0].SrcBlendAlpha = D3D12_BLEND_ONE;
	BlendState.RenderTarget[0].DestBlendAlpha = D3D12_BLEND_ZERO;
	BlendState.RenderTarget[0].BlendOpAlpha = D3D12_BLEND_OP_ADD;
	BlendState.RenderTarget[0].LogicOp = D3D12_LOGIC_OP_NOOP;
	BlendState.RenderTarget[0].RenderTargetWriteMask = D3D12_COLOR_WRITE_ENABLE_ALL;

	// 描述并建立图形管道状态对象 (PSO).
	D3D12_GRAPHICS_PIPELINE_STATE_DESC psoDesc = {};
	psoDesc.InputLayout = { StandardVertexDescription, StandardVertexDescriptionNumElements };
	psoDesc.pRootSignature = m_rootSignature.Get();
	psoDesc.VS = CD3DX12_SHADER_BYTECODE(vertexShader.Get());
	psoDesc.PS = CD3DX12_SHADER_BYTECODE(pixelShader.Get());
	psoDesc.RasterizerState = RasterizerState;
	psoDesc.BlendState = BlendState;
	psoDesc.DepthStencilState = CD3DX12_DEPTH_STENCIL_DESC(D3D12_DEFAULT);
	psoDesc.SampleMask = UINT_MAX;
	psoDesc.PrimitiveTopologyType = D3D12_PRIMITIVE_TOPOLOGY_TYPE_TRIANGLE;
	psoDesc.NumRenderTargets = 1;
	psoDesc.RTVFormats[0] = DXGI_FORMAT_R8G8B8A8_UNORM;
	psoDesc.DSVFormat = DXGI_FORMAT_D32_FLOAT;
	psoDesc.SampleDesc.Count = 1;
	hr = m_device->CreateGraphicsPipelineState(&psoDesc, IID_PPV_ARGS(&m_pipelineState));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateGraphicsPipelineState");

	return hr;
}
HRESULT CGraphics::CreateCommandList() {
	HRESULT hr = S_OK;
	hr = m_device->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, m_commandAllocator.Get(), nullptr, IID_PPV_ARGS(&m_commandList));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateCommandList");

	NAME_D3D12_OBJECT(m_commandList);
	return hr;
}
HRESULT CGraphics::CreateRenderTargetView() {
	HRESULT hr = S_OK;
	// 创建渲染目标视 (RTVs).
	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart());
	for (UINT i = 0; i < FrameCount; i++)
	{
		hr = m_swapChain->GetBuffer(i, IID_PPV_ARGS(&m_renderTargets[i]));
		if (FAILED(hr)) return ThrowHrWithError(hr, L"GetBuffer");
		m_device->CreateRenderTargetView(m_renderTargets[i].Get(), nullptr, rtvHandle);
		rtvHandle.Offset(1, m_rtvDescriptorSize);
		NAME_D3D12_OBJECT_INDEXED(m_renderTargets, i);
	}
	return hr;
}
HRESULT CGraphics::CreateSampler() {
	HRESULT hr = S_OK;
	// 描述并创建采样器
	D3D12_SAMPLER_DESC samplerDesc = {};
	samplerDesc.Filter = D3D12_FILTER_MIN_MAG_MIP_LINEAR;
	samplerDesc.AddressU = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressV = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.AddressW = D3D12_TEXTURE_ADDRESS_MODE_WRAP;
	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D12_FLOAT32_MAX;
	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;
	samplerDesc.ComparisonFunc = D3D12_COMPARISON_FUNC_ALWAYS;
	m_device->CreateSampler(&samplerDesc, m_samplerHeap->GetCPUDescriptorHandleForHeapStart());
	return hr;
}
HRESULT CGraphics::CreateDepthStencilView() {
	HRESULT hr = S_OK;
	D3D12_DEPTH_STENCIL_VIEW_DESC depthStencilDesc = {};
	depthStencilDesc.Format = DXGI_FORMAT_D32_FLOAT;
	depthStencilDesc.ViewDimension = D3D12_DSV_DIMENSION_TEXTURE2D;
	depthStencilDesc.Flags = D3D12_DSV_FLAG_NONE;

	D3D12_CLEAR_VALUE depthOptimizedClearValue = {};
	depthOptimizedClearValue.Format = DXGI_FORMAT_D32_FLOAT;
	depthOptimizedClearValue.DepthStencil.Depth = 1.0f;
	depthOptimizedClearValue.DepthStencil.Stencil = 0;

	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Tex2D(DXGI_FORMAT_D32_FLOAT, m_width, m_height, 1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_DEPTH_STENCIL),
		D3D12_RESOURCE_STATE_DEPTH_WRITE,
		&depthOptimizedClearValue,
		IID_PPV_ARGS(&m_depthStencil)
	);
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateDepthStencilView");
	NAME_D3D12_OBJECT(m_depthStencil);
	m_device->CreateDepthStencilView(m_depthStencil.Get(), &depthStencilDesc, m_dsvHeap->GetCPUDescriptorHandleForHeapStart());

	return hr;
}
HRESULT CGraphics::CreateConstantBuffer() {
	HRESULT hr = S_OK;
	UINT nTotalModelCount = m_games.GetMaxObjectsCount();
	//常量缓冲数据（SRV:0-1000 CBV:1000-5000 
	for (UINT i = 0; i < FrameCount; i++)
	{
		UINT base = 1000 + i*nTotalModelCount;
		m_games.m_objectCollections[i].resize(nTotalModelCount);    //分配矩阵及相关信息

		hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_DIRECT, IID_PPV_ARGS(&m_cbvcommandAllocator[i]));
		hr = m_device->CreateCommandAllocator(D3D12_COMMAND_LIST_TYPE_BUNDLE, IID_PPV_ARGS(&m_cbvbundleAllocator[i]));
		if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateCommandAllocator");
		// 为常量建立上传堆
		hr = m_device->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(sizeof(SceneConstantBuffer) *  nTotalModelCount),
			D3D12_RESOURCE_STATE_GENERIC_READ,
			nullptr,
			IID_PPV_ARGS(&m_cbvUploadHeap[i]));
		if (FAILED(hr)) return ThrowHrWithError(hr, L"m_cbvUploadHeap");


		CD3DX12_CPU_DESCRIPTOR_HANDLE cbvSrvHandle(m_cbvSrvHeap->GetCPUDescriptorHandleForHeapStart(), base, m_cbvSrvDescriptorSize);
		// 为每个位置建立常量缓冲区视
		UINT64 cbOffset = 0;
		for (UINT j = 0; j <nTotalModelCount; j++)
		{
			// 描述并创建常量缓冲区视 (CBV).
			D3D12_CONSTANT_BUFFER_VIEW_DESC cbvDesc = {};
			cbvDesc.BufferLocation = m_cbvUploadHeap[i]->GetGPUVirtualAddress() + cbOffset;
			cbvDesc.SizeInBytes = sizeof(SceneConstantBuffer);
			cbOffset += cbvDesc.SizeInBytes;
			m_device->CreateConstantBufferView(&cbvDesc, cbvSrvHandle);
			cbvSrvHandle.Offset(m_cbvSrvDescriptorSize);
		}
		// Map常量缓冲区
		CD3DX12_RANGE readRange(0, 0);
		hr = m_cbvUploadHeap[i]->Map(0, &readRange, reinterpret_cast<void**>(&m_pConstantBuffers[i]));
		if (FAILED(hr)) return ThrowHrWithError(hr, L"Map");

		// 命令绑定
		hr = m_device->CreateCommandList(0,
			D3D12_COMMAND_LIST_TYPE_BUNDLE,
			m_cbvbundleAllocator[i].Get(),
			m_pipelineState.Get(),
			IID_PPV_ARGS(&m_bundleCommandList[i]));
		if (FAILED(hr)) return ThrowHrWithError(hr, L"m_bundleCommandList");

		BoundCommand(i, m_bundleCommandList[i]);
		//////////////////////////////////////
		//初始世界矩阵位置
		m_games.Initialize(i);
	}
	return  hr;
}

HRESULT CGraphics::ExecCommandList() {
	HRESULT hr = S_OK;
	// 关闭命令表并执行它开始初始 GPU setup.
	hr = m_commandList->Close();
	if (FAILED(hr)) return hr;

	ID3D12CommandList* ppCommandLists[] = { m_commandList.Get() };
	m_commandQueue->ExecuteCommandLists(_countof(ppCommandLists), ppCommandLists);
	return hr;
}

VOID CGraphics::BoundCommand(UINT i, ComPtr<ID3D12GraphicsCommandList> bundleCommand)
{
	UINT nMaxObjectsCount = m_games.GetMaxObjectsCount();
	UINT nTotalModelCount = nMaxObjectsCount;
	UINT base = 1000 + i*nTotalModelCount;
	// 如果根签名匹配调用者根签名,继承绑定，否则绑定复位
	m_bundleCommandList[i]->SetGraphicsRootSignature(m_rootSignature.Get());
	// 指明使用堆
	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeap.Get(), m_samplerHeap.Get() };
	m_bundleCommandList[i]->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
	// 绘制参数
	m_bundleCommandList[i]->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	m_bundleCommandList[i]->IASetIndexBuffer(&m_IndexBufferView);
	m_bundleCommandList[i]->IASetVertexBuffers(0, 1, &m_VertexBufferView);
	// 设置采样器
	m_bundleCommandList[i]->SetGraphicsRootDescriptorTable(1, m_samplerHeap->GetGPUDescriptorHandleForHeapStart());
	// 设置
	{
		//位置世界矩阵 
		CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart(), base, m_cbvSrvDescriptorSize);
		for (UINT k = 0; k < nTotalModelCount; k++)
		{
			if (m_games.m_objectCollections[i][k].status == 1)
			{
				//设置纹理
				CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart(),
					m_games.m_objectCollections[i][k].texid,
					m_cbvSrvDescriptorSize);
				m_bundleCommandList[i]->SetGraphicsRootDescriptorTable(0, srvHandle);
				m_bundleCommandList[i]->SetPipelineState(m_pipelineState.Get());
				// 设置对象世界矩阵
				m_bundleCommandList[i]->SetGraphicsRootDescriptorTable(2, cbvHandle);
				//绘制
				m_bundleCommandList[i]->DrawIndexedInstanced(m_nTotalIndices, 1, 0, 0, 0);
			}
			cbvHandle.Offset(m_cbvSrvDescriptorSize);
		}
	}
	m_bundleCommandList[i]->Close();
}
// 顶点缓冲区2D顶点 湖北·恩施 xx_player
HRESULT CGraphics::CreateVertex() {
	HRESULT hr = S_OK;
	//定义几何形状,注意：纹理范围是0，1区间，这里最小动画单位16X16,如果显示动画是反的转，需要颠倒纹理坐标
	Vertex SpriteVertices[] =
	{
		{ { 0.0f,  0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f,1.0f },{ 0.0f, 1.0f, 0.0f } },
		{ { 0.0f,  1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 0.0f,(DONUTS_BITMAP_HEIGHT - 16.0f) / DONUTS_BITMAP_HEIGHT },{ 0.0f, 1.0f, 0.0f } },
		{ { 1.0f,  1.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 16.0f / DONUTS_BITMAP_WIDTH ,(DONUTS_BITMAP_HEIGHT - 16.0f) / DONUTS_BITMAP_HEIGHT },{ 0.0f, 1.0f, 0.0f } },
		{ { 1.0f,  0.0f, 0.0f },{ 0.0f, 0.0f, 1.0f },{ 16.0f / DONUTS_BITMAP_WIDTH ,1.0f },{ 0.0f, 1.0f, 0.0f } }
	};
	const UINT VertexDataSize = sizeof(SpriteVertices);

	// 创建顶点缓冲区
	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(VertexDataSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_VertexBuffer));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"m_VertexBuffer");
	//上传堆方式
	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(VertexDataSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&vertexBufferUploadHeap));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"vertexBufferUploadHeap");
	NAME_D3D12_OBJECT(m_VertexBuffer);

	// 复制数据到中间上传堆，然后安排从上传堆到顶点缓冲的拷贝 
	D3D12_SUBRESOURCE_DATA vertexData = {};
	vertexData.pData = SpriteVertices;
	vertexData.RowPitch = VertexDataSize;
	vertexData.SlicePitch = vertexData.RowPitch;

	UpdateSubresources<1>(m_commandList.Get(), m_VertexBuffer.Get(), vertexBufferUploadHeap.Get(), 0, 0, 1, &vertexData);
	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(
			m_VertexBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER)
	);

	// 初始顶点缓冲区视.
	m_VertexBufferView.BufferLocation = m_VertexBuffer->GetGPUVirtualAddress();
	m_VertexBufferView.StrideInBytes = sizeof(Vertex);
	m_VertexBufferView.SizeInBytes = VertexDataSize;

	return S_OK;
}
HRESULT CGraphics::CreateIndex() {
	HRESULT hr = S_OK;
	DWORD dwSpriteIndex[] = { 0,1,3,1,2,3 };
	const UINT IndexDataSize = sizeof(dwSpriteIndex);

	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(IndexDataSize),
		D3D12_RESOURCE_STATE_COPY_DEST,
		nullptr,
		IID_PPV_ARGS(&m_IndexBuffer));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateIndex");

	hr = m_device->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
		D3D12_HEAP_FLAG_NONE,
		&CD3DX12_RESOURCE_DESC::Buffer(IndexDataSize),
		D3D12_RESOURCE_STATE_GENERIC_READ,
		nullptr,
		IID_PPV_ARGS(&indexBufferUploadHeap));
	if (FAILED(hr)) return ThrowHrWithError(hr, L"CreateIndex");
	NAME_D3D12_OBJECT(m_IndexBuffer);

	// 复制数据到中间上传堆，然后安排从上传堆到索引缓冲的拷贝 
	D3D12_SUBRESOURCE_DATA indexData = {};
	indexData.pData = dwSpriteIndex;
	indexData.RowPitch = IndexDataSize;
	indexData.SlicePitch = indexData.RowPitch;

	UpdateSubresources<1>(m_commandList.Get(), m_IndexBuffer.Get(), indexBufferUploadHeap.Get(), 0, 0, 1, &indexData);
	m_commandList->ResourceBarrier(1,
		&CD3DX12_RESOURCE_BARRIER::Transition(m_IndexBuffer.Get(),
			D3D12_RESOURCE_STATE_COPY_DEST,
			D3D12_RESOURCE_STATE_INDEX_BUFFER)
	);

	//描述索引缓冲区视
	m_IndexBufferView.BufferLocation = m_IndexBuffer->GetGPUVirtualAddress();
	m_IndexBufferView.Format = DXGI_FORMAT_R32_UINT;
	m_IndexBufferView.SizeInBytes = IndexDataSize;

	m_nTotalIndices = IndexDataSize / 4;    // R32_UINT  = 4 

	return S_OK;
}

void CGraphics::PopulateBegin() {

	HRESULT hr;
	hr = m_cbvcommandAllocator[m_currentFrameResourceIndex]->Reset();
	hr = m_commandList->Reset(m_cbvcommandAllocator[m_currentFrameResourceIndex].Get(), m_pipelineState.Get());

	m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());

	ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeap.Get(), m_samplerHeap.Get() };
	m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);

	m_commandList->RSSetViewports(1, &m_viewport);
	m_commandList->RSSetScissorRects(1, &m_scissorRect);

	// 指示渲染目标的后台缓冲区
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_PRESENT, D3D12_RESOURCE_STATE_RENDER_TARGET));

	CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(m_rtvHeap->GetCPUDescriptorHandleForHeapStart(), m_frameIndex, m_rtvDescriptorSize);
	CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(m_dsvHeap->GetCPUDescriptorHandleForHeapStart());
	m_commandList->OMSetRenderTargets(1, &rtvHandle, FALSE, &dsvHandle);

	// 记录命令
	const float clearColor[] = { 0.0f, 0.2f, 0.4f, 1.0f };
	m_commandList->ClearRenderTargetView(rtvHandle, clearColor, 0, nullptr);
	m_commandList->ClearDepthStencilView(m_dsvHeap->GetCPUDescriptorHandleForHeapStart(), D3D12_CLEAR_FLAG_DEPTH, 1.0f, 0, 0, nullptr);

}
void CGraphics::PopulateEnd() {
	// 指示当前出席的后台缓冲区.
	m_commandList->ResourceBarrier(1, &CD3DX12_RESOURCE_BARRIER::Transition(m_renderTargets[m_frameIndex].Get(), D3D12_RESOURCE_STATE_RENDER_TARGET, D3D12_RESOURCE_STATE_PRESENT));

	// 执行命令表
	ExecCommandList();
}
void CGraphics::PopulateCommandList()
{
	if (UseBundles)
	{
		// 执行预建的绑定.
		m_commandList->ExecuteBundle(m_bundleCommandList[m_currentFrameResourceIndex].Get());

	}
	else
	{
		UINT i = m_currentFrameResourceIndex;
		UINT nMaxObjectsCount = m_games.GetMaxObjectsCount();
		// 如果根签名匹配调用者根签名,继承绑定，否则绑定复位
		m_commandList->SetGraphicsRootSignature(m_rootSignature.Get());
		// 指明使用堆
		ID3D12DescriptorHeap* ppHeaps[] = { m_cbvSrvHeap.Get(), m_samplerHeap.Get() };
		m_commandList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
		// 绘制方式
		m_commandList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
		m_commandList->IASetIndexBuffer(&m_IndexBufferView);
		m_commandList->IASetVertexBuffers(0, 1, &m_VertexBufferView);
		//指定采样器
		m_commandList->SetGraphicsRootDescriptorTable(1, m_samplerHeap->GetGPUDescriptorHandleForHeapStart());
		//指明常量缓冲区偏移 
		UINT base = 1000;
		UINT nTotalModelCount = nMaxObjectsCount;
		//绘制对象
		{
			//设置世界矩阵 
			CD3DX12_GPU_DESCRIPTOR_HANDLE cbvHandle(
				m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart(),
				base + nTotalModelCount * m_currentFrameResourceIndex,
				m_cbvSrvDescriptorSize);
			//绘制
			for (UINT k = 0; k < nTotalModelCount; k++)
			{
				if (m_games.m_objectCollections[i][k].status == 1)
				{
					//设置纹理
					CD3DX12_GPU_DESCRIPTOR_HANDLE srvHandle(m_cbvSrvHeap->GetGPUDescriptorHandleForHeapStart(),
						m_games.m_objectCollections[i][k].texid,
						m_cbvSrvDescriptorSize);
					m_commandList->SetGraphicsRootDescriptorTable(0, srvHandle);
					m_commandList->SetPipelineState(m_pipelineState.Get());
					m_commandList->SetGraphicsRootDescriptorTable(2, cbvHandle);
					m_commandList->DrawIndexedInstanced(m_nTotalIndices, 1, 0, 0, 0);
				}
				cbvHandle.Offset(m_cbvSrvDescriptorSize);
			}

		}
	}
}


// 资源载入及控制  湖北·恩施 xx_player
HRESULT CGraphics::CreateResource() {
	HRESULT hr = S_OK;
	int nTexWidth = DONUTS_BITMAP_WIDTH;
	int nTexHeight = DONUTS_BITMAP_HEIGHT;
	int nTexPixelCount = 4;
	{
		//纹理至少256x256尺寸
		std::wstring FileNameList[] = {
			//
			GetAssetFullPath(L"donuts.bmp"),
			GetAssetFullPath(L"splash.bmp"),
			GetAssetFullPath(L"warn.bmp")
		};
		float fAlpha[3] = { 0.0f,1.0f,0.5f };
		// 载入纹理
		CD3DX12_CPU_DESCRIPTOR_HANDLE srvHandle(m_cbvSrvHeap->GetCPUDescriptorHandleForHeapStart(), 0, m_cbvSrvDescriptorSize);
		int len = sizeof(FileNameList) / sizeof(FileNameList[0]);
		// 分配空间
		m_objectTexture.resize(len);
		textureUploadHeap.resize(len);
		for (int i = 0;i <len;i++)
		{
			D3D12_RESOURCE_DESC textureDesc = {};
			textureDesc.MipLevels = 1;
			textureDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
			textureDesc.Width = nTexWidth;
			textureDesc.Height = nTexHeight;
			textureDesc.Flags = D3D12_RESOURCE_FLAG_NONE;
			textureDesc.DepthOrArraySize = 1;
			textureDesc.SampleDesc.Count = 1;
			textureDesc.SampleDesc.Quality = 0;
			textureDesc.Dimension = D3D12_RESOURCE_DIMENSION_TEXTURE2D;
			hr = m_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
				D3D12_HEAP_FLAG_NONE,
				&textureDesc,
				D3D12_RESOURCE_STATE_COPY_DEST,
				nullptr,
				IID_PPV_ARGS(&m_objectTexture[i]));
			if (FAILED(hr)) return ThrowHrWithError(hr, L"m_objectTexture");

			const UINT subresourceCount = 1;// textureDesc.DepthOrArraySize * textureDesc.MipLevels;
			const UINT64 uploadBufferSize = GetRequiredIntermediateSize(m_objectTexture[i].Get(), 0, subresourceCount);
			hr = m_device->CreateCommittedResource(
				&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
				D3D12_HEAP_FLAG_NONE,
				&CD3DX12_RESOURCE_DESC::Buffer(uploadBufferSize),
				D3D12_RESOURCE_STATE_GENERIC_READ,
				nullptr,
				IID_PPV_ARGS(&textureUploadHeap[i]));
			if (FAILED(hr)) return ThrowHrWithError(hr, L"textureUploadHeap");

			std::vector<UINT8> tex = LoadGDITextureData(FileNameList[i].c_str(), nTexWidth, nTexHeight, fAlpha[i]);
			// 复制数据到中间上传堆并安排从上传堆到漫射光纹理的复制
			D3D12_SUBRESOURCE_DATA textureData = {};
			textureData.pData = &tex[0];
			textureData.RowPitch = nTexWidth*nTexPixelCount;
			textureData.SlicePitch = textureData.RowPitch*nTexHeight;

			UpdateSubresources(m_commandList.Get(), m_objectTexture[i].Get(), textureUploadHeap[i].Get(), 0, 0, subresourceCount, &textureData);
			m_commandList->ResourceBarrier(1,
				&CD3DX12_RESOURCE_BARRIER::Transition(m_objectTexture[i].Get(),
					D3D12_RESOURCE_STATE_COPY_DEST,
					D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE)
			);
			//建立着色视
			D3D12_SHADER_RESOURCE_VIEW_DESC texSrvDesc = {};
			texSrvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
			texSrvDesc.Format = textureDesc.Format;
			texSrvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;
			texSrvDesc.Texture2D.MipLevels = 1;
			m_device->CreateShaderResourceView(m_objectTexture[i].Get(), &texSrvDesc, srvHandle);
			srvHandle.Offset(m_cbvSrvDescriptorSize);
		}
	}

	return hr;
}

// 简单载入位图纹理 湖北·恩施 xx_player
std::vector<UINT8> CGraphics::LoadGDITextureData(const TCHAR *szBitmapName,int width,int height,float fAlpha)
{
	int nAlpha = (int)(fAlpha * 255)%256;
	HBITMAP hBitmap;
	BYTE*p = NULL;
	hBitmap = (HBITMAP)LoadImage(NULL, szBitmapName, IMAGE_BITMAP, 0, 0, LR_DEFAULTCOLOR | LR_CREATEDIBSECTION | LR_LOADFROMFILE);
	BITMAP bitmap;
	GetObject(hBitmap, sizeof(BITMAP), &bitmap);
	int w = bitmap.bmWidth;
	int h = bitmap.bmHeight;
	int size = width* height * 4;
	std::vector<UINT8> data(size);
	UINT8* pData = &data[0];
    memset(pData, 0, size);
	if (bitmap.bmBitsPixel != 8 && bitmap.bmBitsPixel != 24 && bitmap.bmBitsPixel != 32) {
		DeleteObject(hBitmap);
		return data;
	}
	if (bitmap.bmBitsPixel == 8)
	{
		RGBQUAD rgbq[256];
		HDC hMemDC = CreateCompatibleDC(NULL);
		HANDLE hOldBitmap = (HBITMAP)SelectObject(hMemDC, hBitmap);
		GetDIBColorTable(hMemDC, 0, 256, rgbq);
		p = (BYTE*)bitmap.bmBits;
		UINT dst;
		for (int i =0; i< height ; i++)
		{
			if (i >= h) continue;
			for (int j = 0; j < width; j++)
			{
				if (j >= w) continue;
				dst = (i*width + j) * 4;
				UINT src = i*w + j;
				BYTE color = p[src];
				pData[dst + 0] = rgbq[color].rgbRed;
				pData[dst + 1] = rgbq[color].rgbGreen;
				pData[dst + 2] = rgbq[color].rgbBlue;
				//透明色0
				if ((pData[dst + 2] == 0) && (pData[dst + 1] == 0) && (pData[dst + 0] == 0)) pData[dst + 3] = nAlpha;
						else pData[dst + 3] = 0xff;
			}
		}
		//当位图小于创建纹理大小时，上述操作位图是底线对齐，顶部空行。需要把数据移到顶部对齐。
		int lines = height - h;
		int mvdatalen = lines*width * 4;
		if(mvdatalen>0) memmove(pData + mvdatalen, pData, size - mvdatalen);
		SelectObject(hMemDC, hOldBitmap);
		DeleteDC(hMemDC);
	}
	if (bitmap.bmBitsPixel ==24)
	{
		p = (BYTE*)bitmap.bmBits;
		UINT dst;
		for (int i = 0; i< height; i++)
		{
			if (i >= h) continue;
			for (int j = 0; j < width; j++)
			{
				if (j >= w) continue;
				dst = (i*width + j) * 4;
				UINT src = (i*w + j) * 3;
				pData[dst + 0] = p[src + 2];
				pData[dst + 1] = p[src + 1];
				pData[dst + 2] = p[src + 0];
				//透明色0
				if ((p[dst + 2] == 0) && (p[dst + 1] == 0) && (p[dst + 0] == 0)) pData[dst + 3] = nAlpha;
				else pData[dst + 3] = 0xff;
			}
 		}
		//当位图小于创建纹理大小时，上述操作位图是底线对齐，顶部空行。需要把数据移到顶部对齐。
		int lines = height - h;
		int mvdatalen = lines*width * 4;
		if (mvdatalen>0) memmove(pData + mvdatalen, pData, size-mvdatalen);
	}
	if (bitmap.bmBitsPixel == 32)
	{
		p = (BYTE*)bitmap.bmBits;
		UINT dst;
		for (int i = 0; i< height; i++)
		{
			if (i >= h) continue;
			for (int j = 0; j < width; j++)
			{
				if (j >= w) continue;
				dst = (i*width + j) * 4;
				UINT src = (i*w + j) * 4;
				pData[dst + 0] = p[src + 2];
				pData[dst + 1] = p[src + 1];
				pData[dst + 2] = p[src + 0];
				pData[dst + 3] = p[src + 3];
			}
		}
		//当位图小于创建纹理大小时，上述操作位图是底线对齐，顶部空行。需要把数据移到顶部对齐。
		int lines = height - h;
		int mvdatalen = lines*width * 4;
		if (mvdatalen>0) memmove(pData + mvdatalen, pData, size - mvdatalen);
	}
	DeleteObject(hBitmap);
	return data;
}

TCHAR*CGraphics::GetDXGIError(HRESULT hr)
{
	static TCHAR buffer[20];
	StringCchPrintf(buffer, 20, L"0x");
	switch (hr) {
	case 0X887A002B:return L"DXGI_ERROR_ACCESS_DENIED";
	case 0x887A0026:return L"DXGI_ERROR_ACCESS_LOST";
	case 0x887A0036L:return L"DXGI_ERROR_ALREADY_EXISTS";
	case 0x887A002A:return L"DXGI_ERROR_CANNOT_PROTECT_CONTENT";
	case 0x887A0006:return L"DXGI_ERROR_DEVICE_HUNG";
	case 0x887A0005:return L"DXGI_ERROR_DEVICE_REMOVED";
	case 0x887A0007:return L"DXGI_ERROR_DEVICE_RESET";
	case 0x887A0020:return L"DXGI_ERROR_DRIVER_INTERNAL_ERROR";
	case 0x887A000B:return L"DXGI_ERROR_FRAME_STATISTICS_DISJOINT";
	case 0x887A000C:return L"DXGI_ERROR_GRAPHICS_VIDPN_SOURCE_IN_USE";
	case 0x887A0001:return L"DXGI_ERROR_INVALID_CALL";
	case 0x887A0003:return L"DXGI_ERROR_MORE_DATA";
	case 0x887A002C:return L"DXGI_ERROR_NAME_ALREADY_EXISTS";
	case 0x887A0021:return L"DXGI_ERROR_NONEXCLUSIVE";
	case 0x887A0022:return L"DXGI_ERROR_NOT_CURRENTLY_AVAILABLE";
	case 0x887A0002:return L"DXGI_ERROR_NOT_FOUND";
	case 0x887A0023:return L"XGI_ERROR_REMOTE_CLIENT_DISCONNECTED";
	case 0x887A0024:return L"DXGI_ERROR_REMOTE_OUTOFMEMORY";
	case 0x887A0029:return L"DXGI_ERROR_RESTRICT_TO_OUTPUT_STALE";
	case 0x887A002D:return L"DXGI_ERROR_SDK_COMPONENT_MISSING";
	case 0x887A0028:return L"DXGI_ERROR_SESSION_DISCONNECTED";
	case 0x887A0004:return L"DXGI_ERROR_UNSUPPORTED";
	case 0x887A0027:return L"DXGI_ERROR_WAIT_TIMEOUT";
	case 0x887A000A:return L"DXGI_ERROR_WAS_STILL_DRAWING";

	case 0x8007000E:return L"E_OUTOFMEMORY";
	case 0x80070057:return L"E_INVALIDARG";

	default:
		errno_t e;
		e = _itow_s(hr, &buffer[2], 18, 16);
		return buffer;
	}

	return 0;
}
HRESULT CGraphics::ThrowHrWithError(HRESULT hr, TCHAR*msg) {
	StringCbCopy(m_szErrorMsg, MAX_PATH, msg);
	return hr;
}
// D3D2D设置 湖北·恩施 xx_player
void CGraphics::Set2DEnvironment() {

	RECT rc;
	GetClientRect(m_sender->GethWnd(), &rc);
	float left = 0.0f;
	float top = 0.0f;
	float right = (float)(rc.right - rc.left);
	float bottom = (float)(rc.bottom - rc.top);
	m_matWorld = XMMatrixIdentity();
	m_matView = XMMatrixIdentity();
	m_matProj = XMMatrixOrthographicOffCenterLH(left, right, bottom, top, NearZ, FarZ);

}
