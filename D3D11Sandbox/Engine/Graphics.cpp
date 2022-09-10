#include "Graphics.h"

Graphics::Graphics(int windowWidth, int windowHeight, HWND__* handle)
{

}
Graphics::~Graphics()
{
	Cleanup();
}
bool Graphics::Frame()
{
	return true;
}
void Graphics::Cleanup()
{

}
bool Graphics::Render()
{
	return true;
}