#pragma once


#include "Notify.h"

#include <SDL2/SDL.h> 
#include <string>
#include "RegisterToScript.h"

namespace Val {
	enum WindowFlags : Uint32 {
		FULLSCREEN = 1,    //< Fullscreen at the specified resolution
		FULLSCREEN_DESKTOP = 2,    //< Fullscreen at the same resolution as the desktop
		HIDDEN = 4,    //< Hidden Window.
		BORDERLESS = 8,    //< Borderless Window
		RESIZABLE = 16,   //< Resizable Window
		MINIMIZED = 32,   //< Starts Minimized
		MAXIMIZED = 64,   //< Starts Maximized
		INPUT_GRABBED = 128,  //< Window has grabbed input focux
		INPUT_FOCUS = 256,  //< Window has input focus
		MOUSE_FOCUS = 512,  //< Window has mouse focus
		ALLOW_HIGH_DPI = 1024  //< Window supports High-DPI mode
							   //MOUSE_CAPTURE      = 2048  //< NOT CURRENTLY SUPPORTED (Not built with SDL 2.0.4) Window has mouse captured
	};

	class Window : public RegisterToScript {
	public:
		Window();
		~Window();

		void registerToScript(chaiscript::ChaiScript* script);

		int create(unsigned int currentFlags);

		void swapBuffer();

		void resizeWindow(int newWidth, int newHeight);
		void setWindowBrightness(float brightness);
		void setWindowBordered(bool bordered);
		void setWindowFullscreen(bool desktopResolution = false);
		void setWindowGrab(bool grabbed);
		void setWindowIcon(std::string filePath);
		void setWindowMaximunSize(int maxWidth, int maxHeight);
		void setWindowMinimunSize(int minWidth, int minHeight);
		void setWindowPosition(int x, int y);
		void setWindowCentered();

		void setWindowTitleDel(std::string newTitle);
		void resizeWindowDel(std::pair<unsigned int, unsigned int> size);
		void setWindowMinimunSizeDel(std::pair<unsigned int, unsigned int> minSize);

		void setWindowTitle(std::string newTitle);
		void showWindow();
		void hideWindow();
		void maximiseWindow();
		void minimiseWindow();

		bool IgnoresDelegates();
		void SetIgnoreDelegates(bool ignoreDelegates);

		SDL_Window* GetRawWindow();

		NotifyDelegate<std::string> WindowName;
		NotifyDelegate<std::pair<unsigned int, unsigned int>> WindowSize;
		NotifyDelegate<std::pair<unsigned int, unsigned int>> WindowSize_Min;
	private:
		bool bIgnoreDelegates;
		SDL_Window* sdlWindow;
	};
}