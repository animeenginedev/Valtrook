#pragma once

#include "ShaderObject.h"

#include <GL\glew.h>
#include <glm\glm.hpp>

namespace Val {
	struct GenericShaderObjectUniform {
		GLint texLocation;
		GLboolean lineMode;

		GenericShaderObjectUniform(GLboolean lineMode = false, GLint texLocation = 0) :
			lineMode(lineMode), texLocation(texLocation) {
		}
	};

	class GenericShaderObject :
		public ShaderObject {
	public:
		GenericShaderObject();
		virtual ~GenericShaderObject();

		virtual void setCamera(Camera* camera);
		virtual void updateShader(GenericShaderObjectUniform sU);

		void setLineMode(bool value);
	private:
		virtual void updateShader() {}
	};

}