#pragma once

#include "ARect.h"
#include "Colour.h"
#include "GLBlendMode.h"
#include "TextureAsset.h"
#include "UV.h"
#include <chaiscript\chaiscript.hpp>

namespace Val {
	class ATexturedRect : public ARect {
	public:
		static void registerToScript(chaiscript::ChaiScript* script);

		ATexturedRect(const TextureResource& texture, std::array<float, 2> center, std::array<float, 2> halfSize, float depth, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~ATexturedRect();

		void setTexture(const TextureResource& texture);
		void setHalfWidth(float h_width);
		void setHalfHeight(float h_height);
		void setHalfSize(const std::array<float, 2>& halfSize);
		void setHalfSize(float h_width, float h_height);
		void setColour(const Colour& colour);
		void setUV(const UV& uv);
		void setBlendMode(const GLBlendMode& blendMode);

		Colour getColour() const;
		UV getUV() const;
		GLBlendMode getBlendMode() const;

	protected:
		TextureResource texture;
		GLBlendMode blendMode;
		UV uvBounds;
		Colour colour;
	};
}