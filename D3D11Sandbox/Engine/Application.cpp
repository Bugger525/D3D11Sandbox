#include "Application.h"
#include <GLFW/glfw3.h>
#include <iostream>

Application::Application(int width, int height, const std::string_view& title)
{
	mTitle = "ApplicationTitle";
	mWindowWidth = width;
	mWindowHeight = height;

	mWindow = nullptr;
}
Application::~Application()
{
	Cleanup();
}
void Application::Run()
{
	if (!Initialize())
	{
		std::cout << "Error::Application::FailedToInit\n";
		return;
	}

	if (!Load())
	{
		std::cout << "Error::Application::FailedToLoad\n";
		return;
	}

	while (!glfwWindowShouldClose(mWindow))
	{
		glfwPollEvents();
		Update();
		Render();
	}
}
void Application::Cleanup()
{
	if (mWindow != nullptr)
	{
		glfwDestroyWindow(mWindow);
		mWindow = nullptr;
	}
	glfwTerminate();
}
bool Application::Initialize()
{
	if (!glfwInit())
	{
		std::cout << "Error::Application::FailedToInitGLFW\n";
		return false;
	}
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, mTitle.c_str(), nullptr, nullptr);
	if (mWindow == nullptr)
	{
		std::cout << "Error::Application::FailedToCreateWindow\n";
		glfwTerminate();
		return false;
	}

	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, HandleResize);

	return true;
}
GLFWwindow* Application::GetWindow() const
{
	return mWindow;
}

int Application::GetWindowWidth() const
{
	return mWindowWidth;
}

int Application::GetWindowHeight() const
{
	return mWindowHeight;
}
void Application::HandleResize(GLFWwindow* window, int width, int height)
{
	Application& application = *static_cast<Application*>(glfwGetWindowUserPointer(window));
	application.OnResize(width, height);
}
void Application::OnResize(int width, int height)
{
	mWindowWidth = width;
	mWindowHeight = height;
}