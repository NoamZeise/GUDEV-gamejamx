#include "app.h"


App::App()
{
	//set member variables
	mWindowWidth = TARGET_WIDTH * 3;
	mWindowHeight = TARGET_HEIGHT * 3;
	//init glfw window
	glfwSetErrorCallback(error_callback);
	if (!glfwInit())
			throw std::runtime_error("failed to initialise glfw!");
	glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API); //using vulkan not openGL
	mWindow = glfwCreateWindow(mWindowWidth, mWindowHeight, "Vulkan App", nullptr, nullptr);
	if(!mWindow)
	{
		glfwTerminate();
		throw std::runtime_error("failed to create glfw window!");
	}
	if(FIXED_RATIO)
		glfwSetWindowAspectRatio(mWindow, TARGET_WIDTH, TARGET_HEIGHT);
	glfwSetWindowUserPointer(mWindow, this);
	glfwSetFramebufferSizeCallback(mWindow, framebuffer_size_callback);
	glfwSetCursorPosCallback(mWindow, mouse_callback);
	glfwSetScrollCallback(mWindow, scroll_callback);
	glfwSetKeyCallback(mWindow, key_callback);
	glfwSetMouseButtonCallback(mWindow, mouse_button_callback);
	mRender = new Render(mWindow, glm::vec2(TARGET_WIDTH, TARGET_HEIGHT));
	loadAssets();
}


App::~App()
{
	delete mRender;
	mRender = nullptr;
	//cleanup glfw
	glfwDestroyWindow(mWindow);
	glfwTerminate();
}

void App::loadAssets()
{
	//TODO load assets
	msgManager.LoadTextures(mRender);
	map = Map(*mRender);
	map.genMap(10, 10);
	mRender->endTextureLoad();
}

void App::run()
{
	while (!glfwWindowShouldClose(mWindow))
	{
		update();
		draw();
	}
}

void App::resize(int windowWidth, int windowHeight)
{
	mWindowWidth = windowWidth;
	mWindowHeight = windowHeight;
	if(mRender != nullptr)
		mRender->framebufferResized = true;
}

void App::update()
{
	glfwPollEvents();
	btn.press.Update(input.Keys);

	//TODO update app
	msgManager.Update(timer, btn);

	btn.prev = btn.press;
}

void App::draw()
{
	mRender->startDraw();

	//TODO draw app	
	map.Draw(*mRender);
	msgManager.Draw();

	mRender->endDraw();
}

glm::vec2 App::correctedPos(glm::vec2 pos)
{
	return glm::vec2(pos.x * ((float)TARGET_WIDTH / (float)mWindowWidth), pos.y * ((float)TARGET_HEIGHT / (float)mWindowHeight));
}

glm::vec2 App::correctedMouse()
{
	return correctedPos(glm::vec2(input.X, input.Y));
}


#pragma region GLFW_CALLBACKS


void App::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
	auto app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->resize(width, height);
}

void App::mouse_callback(GLFWwindow* window, double xpos, double ypos)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->input.X = xpos;
	app->input.Y = ypos;
}
void App::scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	app->input.offset = yoffset;
}

void App::key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));
	if (key == GLFW_KEY_F && action == GLFW_RELEASE)
	{
		if (glfwGetWindowMonitor(window) == nullptr)
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, glfwGetPrimaryMonitor(), 0, 0, mode->width, mode->height, mode->refreshRate);
		}
		else
		{
			const GLFWvidmode* mode = glfwGetVideoMode(glfwGetPrimaryMonitor());
			glfwSetWindowMonitor(window, NULL, 100, 100, app->mWindowWidth, app->mWindowHeight, mode->refreshRate);
		}
	}
	if(key == GLFW_KEY_ESCAPE && action == GLFW_RELEASE)
	{
		glfwSetWindowShouldClose(window, GLFW_TRUE);
	}
	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
		{
			app->input.Keys[key] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->input.Keys[key] = false;
		}
	}
}

void App::mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	App* app = reinterpret_cast<App*>(glfwGetWindowUserPointer(window));

	if (button >= 0 && button < 8)
	{
		if (action == GLFW_PRESS)
		{
			app->input.Buttons[button] = true;
		}
		else if (action == GLFW_RELEASE)
		{
			app->input.Buttons[button] = true;
		}
	}
}

void App::error_callback(int error, const char* description)
{
    throw std::runtime_error(description);
}

#pragma endregion
