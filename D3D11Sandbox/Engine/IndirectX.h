#pragma once

#include <string>
#include <DirectXMath.h>

struct HWND__;
struct IDXGISwapChain;
struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11Texture2D;
struct ID3D11DepthStencilState;
struct ID3D11DepthStencilView;
struct ID3D11RasterizerState;

class IndirectX
{
public:
	IndirectX(int windowWidth, int windowHeight, HWND__* handle, bool vsync, bool fullScreen, float screenDepth, float screenNear);
	~IndirectX();
	void Cleanup();

	void BeginScene(float r, float g, float b, float);
	void EndScene();

	ID3D11Device* GetDevice() const;
	ID3D11DeviceContext* GetDeviceContext() const;

	const DirectX::XMMATRIX& GetProjectionMatrix() const;
	const DirectX::XMMATRIX& GetWorldMatrix() const;
	const DirectX::XMMATRIX& GetOrthoMatrix() const;

	std::string GetVideoCardName() const;
	int GetVideoCardMemory() const;
private:
	bool mVsyncEnabled = false;
	int mVideoCardMemory = 0;

	std::string mVideoCardDescription;
	IDXGISwapChain* mSwapChain = nullptr;
	ID3D11Device* mDevice = nullptr;
	ID3D11DeviceContext* mDeviceContext = nullptr;
	ID3D11RenderTargetView* mRenderTargetView = nullptr;
	ID3D11Texture2D* mDepthStencilBuffer = nullptr;
	ID3D11DepthStencilState* mDepthStencilState = nullptr;
	ID3D11DepthStencilView* mDepthStencilView = nullptr;
	ID3D11RasterizerState* mRasterState = nullptr;

	DirectX::XMMATRIX mProjectionMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX mWorldMatrix = DirectX::XMMATRIX();
	DirectX::XMMATRIX mOrthoMatrix = DirectX::XMMATRIX();
};