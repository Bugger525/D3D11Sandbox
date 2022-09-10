#pragma once

#include <array>

class Input
{
public:
	Input();

	void SetKeyDown(unsigned int key);
	void SetKeyUp(unsigned int key);

	bool IsKeyDown(unsigned int key);
	bool IsKeyUp(unsigned int key);
private:
	std::array<bool, 256> mKeys;
};