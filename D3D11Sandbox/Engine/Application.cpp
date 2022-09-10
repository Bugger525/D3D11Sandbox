#include "Application.h"
#include <Windows.h>
#include "Graphics.h"
#include "Input.h"

Application::Application(int width, int height, const std::string_view& title)
{
	mWindowWidth = width;
	mWindowHeight = height;
	mWindowTitle.assign(title.begin(), title.end());
	mHinstance = nullptr;
	mHandle = nullptr;

	mGraphics = nullptr;
	mInput = nullptr;
}
Application::~Application()
{
	Cleanup();
}
void Application::Run()
{
	if (!Initialize())
	{
		OutputDebugStringW(L"Error::Application::Init");
		return;
	}
	MSG msg;

	ZeroMemory(&msg, sizeof(MSG));

	while (true)
	{
		if (PeekMessageW(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
			{
				break;
			}
			else
			{
				if (!Frame())
				{
					break;
				}
			}
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}
	Cleanup();
}
long long __stdcall Application::MessageHandler(HWND__* hwnd, unsigned int msg, unsigned long long wParam, long long lParam)
{
	
	Application* thisPtr = reinterpret_cast<Application*>(GetWindowLongPtrW(hwnd, GWLP_USERDATA));
	if (thisPtr)
		return thisPtr->WndProc(hwnd, msg, wParam, lParam);

	return DefWindowProc(hwnd, msg, wParam, lParam);
}
bool Application::Initialize()
{
	WNDCLASSEX wc{};
	int posX, posY;

	mHinstance = GetModuleHandle(NULL);

	wc.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
	wc.lpfnWndProc = MessageHandler;
	wc.cbClsExtra = 0;
	wc.cbWndExtra = 0;
	wc.hInstance = mHinstance;
	wc.hIcon = LoadIcon(NULL, IDI_WINLOGO);
	wc.hIconSm = wc.hIcon;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wc.lpszMenuName = NULL;
	wc.lpszClassName = mWindowTitle.c_str();
	wc.cbSize = sizeof(WNDCLASSEX);

	RegisterClassEx(&wc);

	posX = (GetSystemMetrics(SM_CXSCREEN) - mWindowWidth) / 2;
	posY = (GetSystemMetrics(SM_CYSCREEN) - mWindowHeight) / 2;

	mHandle = CreateWindowW(mWindowTitle.c_str(), mWindowTitle.c_str(),
		WS_OVERLAPPEDWINDOW | WS_VISIBLE,
		posX, posY, mWindowWidth, mWindowHeight, NULL, NULL, mHinstance, NULL);

	ShowWindow(mHandle, SW_SHOW);
	SetFocus(mHandle);
	UpdateWindow(mHandle);

	SetWindowLongPtrW(mHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(this));

	mGraphics = new Graphics(mWindowWidth, mWindowHeight, mHandle);
	if (mGraphics == nullptr)
	{
		OutputDebugStringW(L"Error::Application::Init::Graphics");
		return false;
	}
	mInput = new Input;
	if (mInput == nullptr)
	{
		OutputDebugStringW(L"Error::Application::Init::Input");
		return false;
	}

	return true;
}
bool Application::Frame()
{
	if (mInput->IsKeyDown(VK_ESCAPE))
		return false;
	if (!mGraphics->Frame())
		return false;

	return true;
}
void Application::Cleanup()
{
	if (mGraphics != nullptr)
	{
		mGraphics->Cleanup();
		delete mGraphics;
		mGraphics = nullptr;
	}
	if (mInput != nullptr)
	{
		delete mInput;
		mInput = nullptr;
	}

	DestroyWindow(mHandle);
	mHandle = nullptr;

	UnregisterClassW(mWindowTitle.c_str(), mHinstance);
	mHinstance = nullptr;
}

long long __stdcall Application::WndProc(HWND__* hwnd, unsigned int msg, unsigned long long wParam, long long lParam)
{
	switch (msg)
	{
	case WM_DESTROY:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_CLOSE:
	{
		PostQuitMessage(0);
		break;
	}
	case WM_KEYDOWN:
	{
		mInput->SetKeyDown(static_cast<unsigned int>(wParam));
		break;
	}
	case WM_KEYUP:
	{
		mInput->SetKeyUp(static_cast<unsigned int>(wParam));
		break;
	}
	}
	return DefWindowProc(hwnd, msg, wParam, lParam);
}