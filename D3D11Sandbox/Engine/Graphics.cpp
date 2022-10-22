#include "Graphics.h"
#include "IndirectX.h"
#include <Windows.h>
#include <string>

Graphics::Graphics(int windowWidth, int windowHeight, HWND__* handle)
{
	mInDX = new IndirectX(
		windowWidth, windowHeight, handle,
		VSYNC_ENABLED,
		FULL_SCREEN,
		SCREEN_DEPTH,
		SCREEN_NEAR
	);
	if (!mInDX)
	{
		OutputDebugStringW(L"Error::Graphics::Init::IndirectX\n");
	}
}
Graphics::~Graphics()
{
	Cleanup();
}
bool Graphics::Frame()
{
	return Render();
}
void Graphics::Cleanup()
{
	if (mInDX)
	{
		mInDX->Cleanup();
		delete mInDX;
		mInDX = nullptr;
	}
}
bool Graphics::Render()
{
	mInDX->BeginScene(0.5f, 0.5f, 0.5f, 1.0f);
	mInDX->EndScene();
	return true;
}