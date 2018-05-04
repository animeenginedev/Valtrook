#include "RenderingEngine.h"

#include <gl\glew.h>
#include <iostream>
#include <sstream>

namespace Val {
	Window RenderingEngine::window;

	RenderingEngine::RenderingEngine() {
	}

	RenderingEngine::~RenderingEngine() {
	}

	void RenderingEngine::InitWindow() {
		window.create(0);

		std::stringstream str = std::stringstream();
		str << glGetString(GL_VERSION) << "\n";
		std::cout << str.str();
	}

	void RenderingEngine::initialise() {
		renderer.initialise();
		GUIRenderer.initialise();

		shaderObject = new GenericShaderObject();
	}

	void RenderingEngine::render() {
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glDepthFunc(GL_LEQUAL);
		renderer.prepare();
		GUIRenderer.prepare();

		while (!renderer.isPrepared() && !GUIRenderer.isPrepared()) {
		}

		shaderObject->attach();
		renderer.render();
		shaderObject->setLineMode(true);
		renderer.renderLines();
		shaderObject->setLineMode(false);

		if (GUIRenderer.VertexCount() > 0) {
			glEnable(GL_DEPTH_TEST);
			glClear(GL_DEPTH_BUFFER_BIT);
			GUIRenderer.render();
			shaderObject->setLineMode(true);
			GUIRenderer.renderLines();
			shaderObject->setLineMode(false);
		}

		window.swapBuffer();

		renderer.clear();
		GUIRenderer.clear();
	}

	VBOBatcher * RenderingEngine::getRenderer() {
		return &renderer;
	}

	VBOBatcher * RenderingEngine::getGUIRenderer() {
		return &GUIRenderer;
	}

	Window RenderingEngine::getWindow() {
		return window;
	}
}