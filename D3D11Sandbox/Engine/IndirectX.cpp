#include "IndirectX.h"
#include <dxgi.h>
#include <d3dcommon.h>
#include <d3d11.h>
#include <array>
#include <vector>

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")

namespace WRL = Microsoft::WRL;
namespace DX = DirectX;

IndirectX::IndirectX(int windowWidth, int windowHeight, HWND__* handle, bool vsync, bool fullScreen, float screenDepth, float screenNear)
{
	HRESULT result = S_OK;

	mVsyncEnabled = vsync;

	WRL::ComPtr<IDXGIFactory> factory = nullptr;
	result = CreateDXGIFactory(__uuidof(IDXGIFactory), (void**)&factory);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::Factory\n");
		return;
	}

	WRL::ComPtr<IDXGIAdapter> adapter = nullptr;
	result = factory->EnumAdapters(0, &adapter);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::Adapter\n");
		return;
	}

	WRL::ComPtr<IDXGIOutput> adapterOutput = nullptr;
	result = adapter->EnumOutputs(0, &adapterOutput);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::AdapterOutput\n");
		return;
	}

	unsigned int numModes = 0;
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, nullptr);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::DisplayModeListLength\n");
		return;
	}

	std::vector<DXGI_MODE_DESC> displayModeList(numModes);
	result = adapterOutput->GetDisplayModeList(DXGI_FORMAT_R8G8B8A8_UNORM, DXGI_ENUM_MODES_INTERLACED, &numModes, displayModeList.data());
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::DisplayModeList\n");
		return;
	}

	unsigned int numerator = 0, denominator = 1;
	for (int i = 0; i < numModes; i++)
	{
		if (displayModeList[i].Width == windowWidth)
		{
			if (displayModeList[i].Height == windowHeight)
			{
				numerator = displayModeList[i].RefreshRate.Numerator;
				denominator = displayModeList[i].RefreshRate.Denominator;
				break;
			}
		}
	}

	DXGI_ADAPTER_DESC adapterDesc;
	result = adapter->GetDesc(&adapterDesc);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::AdapterDesc\n");
		return;
	}

	mVideoCardMemory = static_cast<int>(adapterDesc.DedicatedVideoMemory / 1024 / 1024);

	auto ws = std::wstring(adapterDesc.Description);

	mVideoCardDescription.assign(ws.begin(), ws.end());

	displayModeList.clear();

	adapterOutput->Release();
	adapterOutput = nullptr;

	adapter->Release();
	adapter = nullptr;

	factory->Release();
	factory = nullptr;

	DXGI_SWAP_CHAIN_DESC swapChainDesc;
	ZeroMemory(&swapChainDesc, sizeof(swapChainDesc));

	swapChainDesc.BufferCount = 1;

	swapChainDesc.BufferDesc.Width = windowWidth;
	swapChainDesc.BufferDesc.Height = windowHeight;

	swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;

	swapChainDesc.BufferDesc.RefreshRate.Numerator = numerator;
	swapChainDesc.BufferDesc.RefreshRate.Denominator = denominator;

	swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;

	swapChainDesc.OutputWindow = handle;

	swapChainDesc.SampleDesc.Count = 1;
	swapChainDesc.SampleDesc.Quality = 0;


	swapChainDesc.Windowed = !fullScreen;

	swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
	swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;

	swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD;
	swapChainDesc.Flags = 0;

	D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL_11_0;

	result = D3D11CreateDeviceAndSwapChain(nullptr, D3D_DRIVER_TYPE_HARDWARE, nullptr, 0, &featureLevel, 1,
		D3D11_SDK_VERSION, &swapChainDesc, &mSwapChain, &mDevice, nullptr, &mDeviceContext);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::DeviceAndSwapChain\n");
		return;
	}

	ID3D11Texture2D* backBufferPtr = nullptr;
	result = mSwapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), (LPVOID*)&backBufferPtr);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::BackBuffer\n");
		return;
	}

	result = mDevice->CreateRenderTargetView(backBufferPtr, nullptr, &mRenderTargetView);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::RenderTargetView\n");
		return;
	}

	backBufferPtr->Release();
	backBufferPtr = 0;

	D3D11_TEXTURE2D_DESC depthBufferDesc;

	ZeroMemory(&depthBufferDesc, sizeof(depthBufferDesc));

	depthBufferDesc.Width = windowWidth;
	depthBufferDesc.Height = windowHeight;
	depthBufferDesc.MipLevels = 1;
	depthBufferDesc.ArraySize = 1;
	depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthBufferDesc.SampleDesc.Count = 1;
	depthBufferDesc.SampleDesc.Quality = 0;
	depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
	depthBufferDesc.CPUAccessFlags = 0;
	depthBufferDesc.MiscFlags = 0;

	result = mDevice->CreateTexture2D(&depthBufferDesc, nullptr, &mDepthStencilBuffer);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::BufferTexture\n");
		return;
	}

	D3D11_DEPTH_STENCIL_DESC depthStencilDesc;

	ZeroMemory(&depthStencilDesc, sizeof(depthStencilDesc));

	depthStencilDesc.DepthEnable = true;
	depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
	depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;

	depthStencilDesc.StencilEnable = true;
	depthStencilDesc.StencilReadMask = 0xFF;
	depthStencilDesc.StencilWriteMask = 0xFF;

	depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
	depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
	depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
	depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

	result = mDevice->CreateDepthStencilState(&depthStencilDesc, &mDepthStencilState);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::DepthStencilState\n");
		return;
	}

	mDeviceContext->OMSetDepthStencilState(mDepthStencilState, 1);

	D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc;
	ZeroMemory(&depthStencilViewDesc, sizeof(depthStencilViewDesc));

	depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
	depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
	depthStencilViewDesc.Texture2D.MipSlice = 0;

	result = mDevice->CreateDepthStencilView(mDepthStencilBuffer, &depthStencilViewDesc, &mDepthStencilView);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::DepthStencilView\n");
		return;
	}

	mDeviceContext->OMSetRenderTargets(1, &mRenderTargetView, mDepthStencilView);

	D3D11_RASTERIZER_DESC rasterDesc;

	rasterDesc.AntialiasedLineEnable = false;
	rasterDesc.CullMode = D3D11_CULL_BACK;
	rasterDesc.DepthBias = 0;
	rasterDesc.DepthBiasClamp = 0.0f;
	rasterDesc.DepthClipEnable = true;
	rasterDesc.FillMode = D3D11_FILL_SOLID;
	rasterDesc.FrontCounterClockwise = false;
	rasterDesc.MultisampleEnable = false;
	rasterDesc.ScissorEnable = false;
	rasterDesc.SlopeScaledDepthBias = 0.0f;

	result = mDevice->CreateRasterizerState(&rasterDesc, &mRasterState);
	if (FAILED(result))
	{
		OutputDebugStringW(L"Error::IndirectX::Init::RasterizerState\n");
		return;
	}

	mDeviceContext->RSSetState(mRasterState);

	D3D11_VIEWPORT viewport;

	viewport.Width = static_cast<float>(windowWidth);
	viewport.Height = static_cast<float>(windowHeight);
	viewport.MinDepth = 0.0f;
	viewport.MaxDepth = 1.0f;
	viewport.TopLeftX = 0.0f;
	viewport.TopLeftY = 0.0f;

	mDeviceContext->RSSetViewports(1, &viewport);

	float  fieldOfView = static_cast<float>(DX::XM_PI) / 4.0f;
	float screenAspect = static_cast<float>(windowWidth) / static_cast<float>(windowHeight);

	mProjectionMatrix = DX::XMMatrixPerspectiveFovLH(fieldOfView, screenAspect, screenNear, screenDepth);
	mWorldMatrix = DX::XMMatrixIdentity();
	mOrthoMatrix = DX::XMMatrixOrthographicLH(static_cast<float>(windowWidth), static_cast<float>(windowHeight), screenNear, screenDepth);
}
IndirectX::~IndirectX()
{
	Cleanup();
}
void IndirectX::Cleanup()
{
	if (mSwapChain)
		mSwapChain->SetFullscreenState(false, nullptr);

	if (mRasterState)
	{
		mRasterState->Release();
		mRasterState = nullptr;
	}

	if (mDepthStencilView)
	{
		mDepthStencilView->Release();
		mDepthStencilView = nullptr;
	}

	if (mDepthStencilState)
	{
		mDepthStencilState->Release();
		mDepthStencilState = nullptr;
	}

	if (mDepthStencilBuffer)
	{
		mDepthStencilBuffer->Release();
		mDepthStencilBuffer = nullptr;
	}

	if (mRenderTargetView)
	{
		mRenderTargetView->Release();
		mRenderTargetView = nullptr;
	}

	if (mDeviceContext)
	{
		mDeviceContext->Release();
		mDeviceContext = nullptr;
	}

	if (mDevice)
	{
		mDevice->Release();
		mDevice = nullptr;
	}

	if (mSwapChain)
	{
		mSwapChain->Release();
		mSwapChain = nullptr;
	}
}
void IndirectX::BeginScene(float r, float g, float b, float a)
{
	std::array<float, 4> color = { r, g, b, a };

	mDeviceContext->ClearRenderTargetView(mRenderTargetView, color.data());
	mDeviceContext->ClearDepthStencilView(mDepthStencilView, D3D11_CLEAR_DEPTH, 1.0f, 0);
}
void IndirectX::EndScene()
{
	mSwapChain->Present(static_cast<unsigned int>(mVsyncEnabled), 0);
}
ID3D11Device* IndirectX::GetDevice() const
{
	return mDevice;
}
ID3D11DeviceContext* IndirectX::GetDeviceContext() const
{
	return mDeviceContext;
}
const DX::XMMATRIX& IndirectX::GetProjectionMatrix() const
{
	return mProjectionMatrix;
}
const DX::XMMATRIX& IndirectX::GetWorldMatrix() const
{
	return mWorldMatrix;
}
const DX::XMMATRIX& IndirectX::GetOrthoMatrix() const
{
	return mOrthoMatrix;
}
std::string IndirectX::GetVideoCardName() const
{
	return mVideoCardDescription;
}
int IndirectX::GetVideoCardMemory() const
{
	return mVideoCardMemory;
}
