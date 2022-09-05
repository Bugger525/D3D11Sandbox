#pragma once

#include <string>

struct GLFWwindow;

class Application
{
public:
	Application(int width, int height, const std::string_view& title = "");
	virtual ~Application();

	void Run();

protected:
	virtual void Cleanup();
	virtual bool Initialize();
	virtual bool Load() = 0;
	virtual void Render() = 0;
	virtual void Update() = 0;

	[[nodiscard]] GLFWwindow* GetWindow() const;
	[[nodiscard]] int GetWindowWidth() const;
	[[nodiscard]] int GetWindowHeight() const;

	static void HandleResize(GLFWwindow* window, int width, int height);
	virtual void OnResize(int width, int height);

private:
	std::string mTitle;
	int mWindowWidth;
	int mWindowHeight;

	GLFWwindow* mWindow;
};