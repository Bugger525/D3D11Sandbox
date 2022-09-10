#pragma once

struct HWND__;

class Graphics
{
public:
	Graphics(int windowWidth, int windowHeight, HWND__* handle);
	~Graphics();

	bool Frame();

	void Cleanup();
private:
	bool Render();
};