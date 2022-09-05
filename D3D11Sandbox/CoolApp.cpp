#include "CoolApp.h"

#include <GLFW/glfw3.h> // Window
#define GLFW_EXPOSE_NATIVE_WIN32
#include <GLFW/glfw3native.h>

#include <d3dcompiler.h> // HLSL
#include <DirectXMath.h> // Math

#include <iostream>

#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3dcompiler.lib")
#pragma comment(lib, "winmm.lib")
#pragma comment(lib, "dxguid.lib")


CoolApp::CoolApp(int width, int height)
    : Application(width, height)
{
}
bool CoolApp::Initialize()
{
    if (!Application::Initialize())
    {
        return false;
    }

    if (FAILED(CreateDXGIFactory2(
        0,
        IID_PPV_ARGS(&_dxgiFactory))))
    {
        std::cout << "Error::CoolApp::FailedToCreateDXGIFactory\n";
        return false;
    }

    return true;
}
bool CoolApp::Load()
{
    return true;
}
bool CoolApp::CreateSwapchainResources()
{
    return true;
}
void CoolApp::DestroySwapchainResources()
{
}
void CoolApp::OnResize(int width, int height)
{

}
void CoolApp::Update()
{
}
void CoolApp::Render()
{
}