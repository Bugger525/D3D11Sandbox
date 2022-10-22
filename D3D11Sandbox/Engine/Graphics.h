#pragma once

struct HWND__;
class IndirectX;

constexpr bool FULL_SCREEN = false;
constexpr bool VSYNC_ENABLED = true;
constexpr float SCREEN_DEPTH = 1000.0f;
constexpr float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics(int windowWidth, int windowHeight, HWND__* handle);
	~Graphics();

	bool Frame();

	void Cleanup();
private:
	bool Render();

	IndirectX* mInDX = nullptr;
};