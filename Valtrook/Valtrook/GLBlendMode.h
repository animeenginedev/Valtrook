#pragma once

#include <GL/glew.h>

namespace Val {
	struct GLBlendMode {
		static GLBlendMode Blend_Default;
		static GLBlendMode Blend_Additive;

		GLBlendMode() {
			SrcColour = Blend_Default.SrcColour;
			SrcAlpha = Blend_Default.SrcAlpha;
			DstColour = Blend_Default.DstColour;
			DstAlpha = Blend_Default.DstAlpha;
			BlendOrder = Blend_Default.BlendOrder;
		}

		GLBlendMode(GLenum SrcColour, GLenum DstColour, GLenum SrcAlpha, GLenum DstAlpha, unsigned int BlendOrder = 0) : SrcColour(SrcColour), DstColour(DstColour), SrcAlpha(SrcAlpha), DstAlpha(DstAlpha), BlendOrder(BlendOrder) {

		};

		GLenum SrcColour, SrcAlpha;
		GLenum DstColour, DstAlpha;
		unsigned int BlendOrder;
		
		bool operator==(const GLBlendMode& rhs) const {
			return SrcColour == rhs.SrcColour && SrcAlpha == rhs.SrcAlpha &&
				DstColour == rhs.DstColour && DstAlpha == rhs.DstAlpha &&
				BlendOrder == rhs.BlendOrder;
		}


		bool operator!=(const GLBlendMode& rhs) const {
			return SrcColour != rhs.SrcColour || SrcAlpha != rhs.SrcAlpha ||
				DstColour != rhs.DstColour || DstAlpha != rhs.DstAlpha ||
				BlendOrder != rhs.BlendOrder;
		}
	};
}