#pragma once
class CWin32D3D12Donuts;
#include "DXSample.h"
#include "StepTimer.h"
#include "CGames.h"
using namespace DirectX;
using Microsoft::WRL::ComPtr;
#define FrameCount 3
//场景常量缓冲区
// 模型-视-投影矩阵 Model-view-projection (MVP) matrix.
// 纹理变换矩阵     Texture-Translate (TT) matrix.
struct SceneConstantBuffer
{
	XMFLOAT4X4 mvp;       
	XMFLOAT4X4 tt;        
	FLOAT padding[32];
};

class CGraphics :public DXSample
{
	CWin32D3D12Donuts*m_sender;

	CGames m_games;
	// 管道对象
	CD3DX12_VIEWPORT m_viewport;
	CD3DX12_RECT m_scissorRect;

	bool UseBundles = false;// true;
	float SpacingInterval;

	// 帧资源
	UINT m_currentFrameResourceIndex;

	// 管道对象
	ComPtr<IDXGIFactory4> m_factory;
	ComPtr<IDXGISwapChain3> m_swapChain;
	ComPtr<ID3D12Device> m_device;
	ComPtr<ID3D12Resource> m_renderTargets[FrameCount];
	ComPtr<ID3D12Resource> m_depthStencil;
	ComPtr<ID3D12CommandAllocator> m_commandAllocator;
	ComPtr<ID3D12CommandQueue> m_commandQueue;
	ComPtr<ID3D12RootSignature> m_rootSignature;
	ComPtr<ID3D12DescriptorHeap> m_rtvHeap;
	ComPtr<ID3D12DescriptorHeap> m_cbvSrvHeap;
	ComPtr<ID3D12DescriptorHeap> m_dsvHeap;
	ComPtr<ID3D12DescriptorHeap> m_samplerHeap;
	ComPtr<ID3D12PipelineState> m_pipelineState;
	ComPtr<ID3D12GraphicsCommandList> m_commandList;

	
	// 应用资源
	StepTimer m_timer;
	UINT m_cbvSrvDescriptorSize;
	UINT m_rtvDescriptorSize;

	std::vector<ComPtr<ID3D12Resource>>m_objectTexture;
	std::vector<ComPtr<ID3D12Resource>>textureUploadHeap;

	ComPtr<ID3D12CommandAllocator> m_cbvcommandAllocator[FrameCount];
	ComPtr<ID3D12CommandAllocator> m_cbvbundleAllocator[FrameCount];
	// 同步对象
	UINT m_frameIndex;
	UINT m_frameCounter;
	HANDLE m_fenceEvent;
	ComPtr<ID3D12Fence> m_fence;
	UINT64 m_fenceValue;

	ComPtr<ID3D12GraphicsCommandList> m_bundleCommandList[FrameCount];
	UINT64 m_cbvfenceValue[FrameCount];

	ComPtr<ID3D12Resource> vertexBufferUploadHeap;
	ComPtr<ID3D12Resource> indexBufferUploadHeap;

	SceneConstantBuffer* m_pConstantBuffers[FrameCount];
	ComPtr<ID3D12Resource> m_cbvUploadHeap[FrameCount];

	HRESULT LoadPipeline();
	HRESULT LoadAssets();

	void PopulateBegin();
	void PopulateCommandList();
	void PopulateEnd();


	VOID BoundCommand(UINT i, ComPtr<ID3D12GraphicsCommandList> bundleCommand);

	HRESULT ExecCommandList();


	HRESULT CreateFactory();
	HRESULT CreateDevice();
	HRESULT CreateSwapChain();
	HRESULT CreateCommandQueue();
	HRESULT CreateHeap();
	HRESULT CreateCommandAllocator();

	HRESULT CreateRootSignature();
	HRESULT CreatePipelineState();
	HRESULT CreateCommandList();

	HRESULT CreateRenderTargetView();
	HRESULT CreateResource();
	HRESULT CreateSampler();
	HRESULT CreateDepthStencilView();

	HRESULT CreateConstantBuffer();

	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
		XMFLOAT3 tangent;
	};

	ComPtr<ID3D12Resource> m_VertexBuffer;
	ComPtr<ID3D12Resource> m_IndexBuffer;
	D3D12_VERTEX_BUFFER_VIEW m_VertexBufferView;
	D3D12_INDEX_BUFFER_VIEW m_IndexBufferView;
	UINT m_nTotalIndices;

	HRESULT CreateVertex();
	HRESULT CreateIndex();



	std::vector<UINT8> LoadGDITextureData(const TCHAR *szBitmapName, int width, int hight, float fAlpha);

	TCHAR*GetDXGIError(HRESULT hr);
	TCHAR m_szErrorMsg[MAX_PATH];
	HRESULT ThrowHrWithError(HRESULT hr, TCHAR*msg);

	XMMATRIX m_matWorld;
	XMMATRIX m_matView;
	XMMATRIX m_matProj;


public:
	CGraphics(CWin32D3D12Donuts*sender);
	~CGraphics();

	virtual BOOL OnInit();
	virtual void OnUpdate();
	virtual void OnRender();
	virtual void OnDestroy();
	virtual void OnKeyDown(UINT8 key);
	virtual void OnKeyUp(UINT8 key);


	BOOL CreateEnvironment();
	VOID ReleaseEnvironment();

	void Set2DEnvironment();
};