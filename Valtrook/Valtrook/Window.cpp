#include "Window.h"

#include "Exit.h"
#include <GL/glew.h>
#include <SDL2\SDL.h>
#include <SDL_image.h>
#include <functional>
#include "RuntimeConstants.h"

namespace Val {
	Window::Window() : bIgnoreDelegates(false) {}

	Window::~Window() {}

	void Window::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Window>(), "Window");

		script->add(chaiscript::fun(&Window::resizeWindow), "resizeWindow");
		script->add(chaiscript::fun(&Window::setWindowBrightness), "setWindowBrightness");
		script->add(chaiscript::fun(&Window::setWindowBordered), "setWindowBordered");
		script->add(chaiscript::fun(&Window::setWindowFullscreen), "setWindowFullscreen");
		script->add(chaiscript::fun(&Window::setWindowGrab), "setWindowGrab");
		script->add(chaiscript::fun(&Window::setWindowIcon), "setWindowIcon");
		script->add(chaiscript::fun(&Window::setWindowMaximunSize), "setWindowMaximunSize");
		script->add(chaiscript::fun(&Window::setWindowMinimunSize), "setWindowMinimunSize");
		script->add(chaiscript::fun(&Window::setWindowPosition), "setWindowPosition");
		script->add(chaiscript::fun(&Window::setWindowCentered), "setWindowCentered");

		script->add(chaiscript::fun(&Window::setWindowTitle), "setWindowTitle");
		script->add(chaiscript::fun(&Window::maximiseWindow), "maximiseWindow");
		script->add(chaiscript::fun(&Window::minimiseWindow), "minimiseWindow");
	}

	int Window::create( unsigned int currentFlags) {

		RuntimeConstants::Instance->WindowName.addReciever(&WindowName);
		RuntimeConstants::Instance->Window_Size.addReciever(& WindowSize);
		RuntimeConstants::Instance->Window_Size_Min.addReciever(&WindowSize_Min);

		WindowName.funcOnChange(std::bind(&Window::setWindowTitle, this, std::placeholders::_1));
		WindowSize.funcOnChange(std::bind(&Window::resizeWindowDel, this, std::placeholders::_1));
		WindowSize_Min.funcOnChange(std::bind(&Window::setWindowMinimunSizeDel, this, std::placeholders::_1));


		Uint32 flags = SDL_WINDOW_OPENGL;

		if (currentFlags & FULLSCREEN) { flags |= SDL_WINDOW_FULLSCREEN; }
		if (currentFlags & FULLSCREEN_DESKTOP) { flags |= SDL_WINDOW_FULLSCREEN_DESKTOP; }
		if (currentFlags & HIDDEN) { flags |= SDL_WINDOW_HIDDEN; }
		if (currentFlags & BORDERLESS) { flags |= SDL_WINDOW_BORDERLESS; }
		if (currentFlags & RESIZABLE) { flags |= SDL_WINDOW_RESIZABLE; }
		if (currentFlags & MINIMIZED) { flags |= SDL_WINDOW_MINIMIZED; }
		if (currentFlags & MAXIMIZED) { flags |= SDL_WINDOW_MAXIMIZED; }
		if (currentFlags & INPUT_GRABBED) { flags |= SDL_WINDOW_INPUT_GRABBED; }
		if (currentFlags & INPUT_FOCUS) { flags |= SDL_WINDOW_INPUT_FOCUS; }
		if (currentFlags & MOUSE_FOCUS) { flags |= SDL_WINDOW_MOUSE_FOCUS; }
		if (currentFlags & ALLOW_HIGH_DPI) { flags |= SDL_WINDOW_ALLOW_HIGHDPI; }
		//if (currentFlags & MOUSE_CAPTURE) { flags |= SDL_WINDOW_MOUSE_CAPTURE; }

		if (SDL_Init(SDL_INIT_VIDEO) < 0) {
			fprintf(stderr, "Video initialization failed @SDL2: %s\n",
					SDL_GetError());
		}

		SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 5);
		SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);
		SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);

		std::pair<unsigned int, unsigned int> size = WindowSize.Source->get();
		sdlWindow = SDL_CreateWindow(WindowName.Source->get().c_str(), SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, size.first, size.second, flags);
		if (sdlWindow == nullptr) {
			pausedExit(ERROR_SDL_FAILED_TO_CREATE_WINDOW);
		}

		SDL_SetHint(SDL_HINT_RENDER_OPENGL_SHADERS, 0);

		//Set up our OpenGL context
		SDL_GLContext glContext = SDL_GL_CreateContext(sdlWindow);
		if (glContext == nullptr) {
			pausedExit(ERROR_SDL_FAILED_TO_CREATE_GL_CONTEXT);
		}

		//Set up glew (optional but recommended)
		GLenum error = glewInit();
		if (error != GLEW_OK) {
			pausedExit(ERROR_GLEW_FAILED_TO_INITIALIZE);
		}

		if (!(IMG_Init(IMG_INIT_PNG | IMG_INIT_JPG) & (IMG_INIT_PNG | IMG_INIT_JPG))) {
			printf("SDL could not initialize! SDL Error: %s\n", SDL_GetError());
			pausedExit(ERROR_FAILED_TO_INITIALISE_SDL_IMG);
		}

		//Set the background color to blue
		Colour clearColour = RuntimeConstants::Instance->GLClearColour.get();
		glClearColor(static_cast<float>(clearColour.r) / 255.0f, static_cast<float>(clearColour.g) / 255.0f, static_cast<float>(clearColour.b) / 255.0f, static_cast<float>(clearColour.a) / 255.0f);

		//Set VSYNC
		SDL_GL_SetSwapInterval(0);

		// Enable alpha blend by default
		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glShadeModel(GL_SMOOTH);
		glEnable(GL_ALPHA_TEST);
		glAlphaFunc(GL_GREATER, 0.0f);
		glEnable(GL_DEPTH_TEST);
		glClearDepth(1.0f);
		glDepthFunc(GL_LESS);
		//glDepthMask(GL_TRUE);
		glLineWidth(2.5f);
		glEnable(GL_TEXTURE_2D);
		glEnable(GL_TEXTURE0);

		return 0;
	}

	void Window::swapBuffer() {
		SDL_GL_SwapWindow(sdlWindow);
	}

	void Window::resizeWindow(int newWidth, int newHeight) {
		SDL_SetWindowSize(sdlWindow, newWidth, newHeight);
	}

	void Window::setWindowBrightness(float brightness) {
		SDL_SetWindowBrightness(sdlWindow, brightness);
	}

	void Window::setWindowBordered(bool bordered) {
		SDL_SetWindowBordered(sdlWindow, bordered ? SDL_TRUE : SDL_FALSE);
	}

	void Window::setWindowFullscreen(bool desktopResolution) {
		if (desktopResolution)
			SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN_DESKTOP);
		else
			SDL_SetWindowFullscreen(sdlWindow, SDL_WINDOW_FULLSCREEN);
	}

	void Window::setWindowGrab(bool grabbed) {
		SDL_SetWindowGrab(sdlWindow, grabbed ? SDL_TRUE : SDL_FALSE);
	}

	void Window::setWindowIcon(std::string filePath) {
		SDL_Surface* icon;
		icon = SDL_LoadBMP(filePath.c_str());
		SDL_SetColorKey(icon, SDL_TRUE, 0xFF00FF);

		SDL_SetWindowIcon(sdlWindow, icon);
		SDL_FreeSurface(icon);
	}

	void Window::setWindowMaximunSize(int maxWidth, int maxHeight) {
		SDL_SetWindowMaximumSize(sdlWindow, maxWidth, maxHeight);
	}

	void Window::setWindowMinimunSize(int minWidth, int minHeight) {
		SDL_SetWindowMinimumSize(sdlWindow, minWidth, minHeight);
	}

	void Window::setWindowPosition(int x, int y) {
		SDL_SetWindowPosition(sdlWindow, x, y);
	}

	void Window::setWindowCentered() {
		SDL_SetWindowPosition(sdlWindow, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED);
	}

	void Window::setWindowTitleDel(std::string newTitle) {
		if (bIgnoreDelegates)
			return;
		SDL_SetWindowTitle(sdlWindow, newTitle.c_str());
	}

	void Window::resizeWindowDel(std::pair<unsigned int, unsigned int> size) {
		if (bIgnoreDelegates)
			return;
		SDL_SetWindowSize(sdlWindow, size.first, size.second);
	}

	void Window::setWindowMinimunSizeDel(std::pair<unsigned int, unsigned int> minSize) {
		if (bIgnoreDelegates)
			return;
		SDL_SetWindowMinimumSize(sdlWindow, minSize.first, minSize.second);
	}

	void Window::setWindowTitle(std::string newTitle) {
		SDL_SetWindowTitle(sdlWindow, newTitle.c_str());
	}

	void Window::showWindow() {
		SDL_ShowWindow(sdlWindow);
	}

	void Window::hideWindow() {
		SDL_HideWindow(sdlWindow);
	}

	void Window::maximiseWindow() {
		SDL_MaximizeWindow(sdlWindow);
	}

	void Window::minimiseWindow() {
		SDL_MinimizeWindow(sdlWindow);
	}

	bool Window::IgnoresDelegates() {
		return bIgnoreDelegates;
	}

	void Window::SetIgnoreDelegates(bool ignoreDelegates) {
		bIgnoreDelegates = ignoreDelegates;
	}

	SDL_Window * Window::GetRawWindow() {
		return sdlWindow;
	}
}