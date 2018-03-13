#include "GenericShaderObject.h"

#include "Camera.h"

#include <vector>

namespace Val {
	auto getAttributes() {
		auto attribs = std::vector<std::string>();

		attribs.push_back("vertexPosition");
		attribs.push_back("vertexUV");
		attribs.push_back("vertexColor");

		return attribs;
	}

	GenericShaderObject::GenericShaderObject() : ShaderObject("assets/shaders/genericShader2D.vs",
															  "assets/shaders/genericShader2D.fs",
															  "", "", "", getAttributes()) {
	}

	GenericShaderObject::~GenericShaderObject() {
	}

	void GenericShaderObject::setCamera(Camera * camera) {
		GLint cameraLocation = this->getUniformLocation("P");
		glUniformMatrix4fv(cameraLocation, 1, GL_FALSE, &(camera->getMatrix()[0][0]));
	}

	void GenericShaderObject::updateShader(GenericShaderObjectUniform sU) {
		GLint textureLocation = this->getUniformLocation("sample");
		GLint lineMode = this->getUniformLocation("lineMode");

		glUniform1i(textureLocation, sU.texLocation);
		glUniform1i(lineMode, sU.lineMode);
	}

	void GenericShaderObject::setLineMode(bool value) {
		GLint lineMode = this->getUniformLocation("lineMode");
		glUniform1i(lineMode, value);
	}
}