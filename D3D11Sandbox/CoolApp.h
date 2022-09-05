#pragma once

#include "Engine/Application.h"

#include <d3d11_2.h> // D3D11
#include <wrl.h> // ComPtr

class CoolApp final : public Application
{
    template <typename T>
    using ComPtr = Microsoft::WRL::ComPtr<T>;
public:
    CoolApp(int width, int height);
    ~CoolApp() override;

protected:
    bool Initialize() override;
    bool Load() override;
    void OnResize(int width, int height) override;
    void Render() override;
    void Update() override;

private:
    bool CreateSwapchainResources();
    void DestroySwapchainResources();

    ComPtr<ID3D11Device> _device = nullptr;
    ComPtr<ID3D11DeviceContext> _deviceContext = nullptr;
    ComPtr<IDXGIFactory2> _dxgiFactory = nullptr;
    ComPtr<IDXGISwapChain1> _swapChain = nullptr;
    ComPtr<ID3D11RenderTargetView> _renderTarget = nullptr;
};