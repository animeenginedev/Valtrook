#pragma once

#include "VBOBatcher.h"
#include "Window.h"

#include "OrthographicCamera.h"
#include "GenericShaderObject.h"

namespace Val {
	class RenderingEngine {
	public:
		RenderingEngine();
		~RenderingEngine();

		static void InitWindow();

		virtual void initialise();
		virtual void render();

		GenericShaderObject* shaderObject;

		OrthographicCamera* currentCamera;

		VBOBatcher* getRenderer();
		VBOBatcher* getGUIRenderer();
		Window getWindow();
	protected:
		VBOBatcher renderer;
		VBOBatcher GUIRenderer;
		static Window window;
	};

}