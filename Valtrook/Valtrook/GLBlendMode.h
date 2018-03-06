#pragma once

#include <GL/glew.h>

namespace Val {
	struct GLBlendMode {
		static GLBlendMode Blend_Default;
		static GLBlendMode Blend_Additive;
		
		GLBlendMode(GLenum SrcColour, GLenum DstColour, GLenum SrcAlpha, GLenum DstAlpha, unsigned int BlendOrder = 0) : SrcColour(SrcColour), DstColour(DstColour), SrcAlpha(SrcAlpha), DstAlpha(DstAlpha), BlendOrder(BlendOrder) {

		};

		GLenum SrcColour, SrcAlpha;
		GLenum DstColour, DstAlpha;
		unsigned int BlendOrder;
	};
}