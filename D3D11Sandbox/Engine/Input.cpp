#include "Input.h"

Input::Input()
{
	mKeys.fill(false);
}
void Input::SetKeyDown(unsigned int key)
{
	mKeys[key] = true;
}
void Input::SetKeyUp(unsigned int key)
{
	mKeys[key] = false;
}
bool Input::IsKeyDown(unsigned int key)
{
	return mKeys[key];
}
bool Input::IsKeyUp(unsigned int key)
{
	return !mKeys[key];
}