#pragma once

#include <string>
#include <wrl.h>
#include <DirectXMath.h>

struct HWND__;
struct ID3D11Device;
struct ID3D11DeviceContext;

class IndirectX
{
public:
	IndirectX(int windowWidth, int windowHeight, HWND__* handle, bool vsync, bool fullScreen, float screenDepth, float screenNear);
	~IndirectX();
	void Cleanup();

	void BeginScene(float r, float g, float b, float a);
	void EndScene();

	WRL::ComPtr<ID3D11Device> GetDevice() const;
	WRL::ComPtr<ID3D11DeviceContext> GetDeviceContext() const;

	const DirectX::XMMATRIX& GetProjectionMatrix() const;
	const DirectX::XMMATRIX& GetWorldMatrix() const;
	const DirectX::XMMATRIX& GetOrthoMatrix() const;

	std::string GetVideoCardName() const;
	int GetVideoCardMemory() const;
private:
	struct IDXGISwapChai
	struct ID3D11RenderTargetView;
	struct ID3D11Texture2D;
	struct ID3D11DepthStencilState;
	struct ID3D11DepthStencilView;
	struct ID3D11RasterizerState;
	
	bool mVsyncEnabled = false;
	int mVideoCardMemory = 0;

	std::string mVideoCardDescription;
	Microsoft::WRL::ComPtr<IDXGISwapChain> mSwapChain = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Device> mDevice = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DeviceContext> mDeviceContext = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RenderTargetView> mRenderTargetView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11Texture2D> mDepthStencilBuffer = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilState> mDepthStencilState = nullptr;
	Microsoft::WRL::ComPtr<ID3D11DepthStencilView> mDepthStencilView = nullptr;
	Microsoft::WRL::ComPtr<ID3D11RasterizerState> mRasterState = nullptr;

	DirectX::XMMATRIX mProjectionMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX mWorldMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX mOrthoMatrix = DirectX::XMMATRIX();
};
