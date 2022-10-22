#pragma once

#include <string>

class Graphics;
class Input;
struct HINSTANCE__;
struct HWND__;

class Application
{
public:
	Application(int width, int height, const std::string_view& title);
	~Application();

	void Run();

	static long long __stdcall MessageHandler(HWND__*, unsigned int, unsigned long long, long long);
private:
	bool Initialize();
	bool Frame();
	void Cleanup();

	long long __stdcall WndProc(HWND__*, unsigned int, unsigned long long, long long);

	int mWindowWidth = 800;
	int mWindowHeight = 600;
	std::wstring mWindowTitle = L"D3D11Sandbox";
	HINSTANCE__* mHinstance = nullptr;
	HWND__* mHandle = nullptr;

	Graphics* mGraphics = nullptr;
	Input* mInput = nullptr;
};