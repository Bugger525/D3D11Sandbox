#include <Windows.h>
#include "Engine/Application.h"

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PWSTR pScmdline, int iCmdshow)
{
	auto app = Application(800, 600, "D3D11Sandbox");
	app.Run();	

	return 0;
}