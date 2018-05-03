#pragma once

#include "ATexturedRect.h"
#include "TextResource.h"
#include "Glyph.h"
#include "RegisterToScript.h"

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class SimpleTextRectangle : public ATexturedRect, public RegisterToScript{
	public:
		SimpleTextRectangle(const TextResource& texture);
		SimpleTextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight);
		SimpleTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize);
		SimpleTextRectangle(const TextResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		SimpleTextRectangle(const TextResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~SimpleTextRectangle();

		void registerToScript(chaiscript::ChaiScript* script);

		void setTextResource(const TextResource& text);
		void setText(const std::string& contents);
		void setFont(FontAsset* newFont);
		void setScaleTextToHeight(bool scaleTextToHeight);
		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);

		TextResource getTextResource() const;
		std::string getText() const;
		FontAsset* getFont() const;
		bool doesScaleTextToHeight() const;
		AABB<float> getCullSurface() const;

		void reconstruct();

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine) override;
		void sendRenderInformation(VBOBatcher* batcher) override;
	protected:
		AABB<float> cullAABB;
		bool bHasCullSurface;

		TextResource textResource;
		std::array<TriangleGlyph, 2> Glyph;
		bool scaleTextToHeight;

		void recalculateVertexes();
	};
}
