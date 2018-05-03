#pragma once

#include "ATexturedRect.h"
#include "Glyph.h"
#include "RegisterToScript.h"

namespace Val {
	class SimpleRectangle : public ATexturedRect, public RegisterToScript{
	public:
		SimpleRectangle(const TextureResource& texture);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize);
		SimpleRectangle(const TextureResource& texture, float x, float y, float depth, float halfWidth, float halfHeight, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		SimpleRectangle(const TextureResource& texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, Colour colour, const GLBlendMode& blendMode = GLBlendMode::Blend_Default);
		~SimpleRectangle();

		void registerToScript(chaiscript::ChaiScript* script);

		//Honestly, don't ask. It's really used to GUI rendering, setting the center/height/width and the uv dependant on the culling aabb size&position
		void setCullSurface(AABB<float> cullAABB);

		TextureResource getTexture() const;
		AABB<float> getCullSurface() const;

		std::array<TriangleGlyph, 2> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine) override;
		void sendRenderInformation(VBOBatcher* batcher) override;
	protected:
		AABB<float> cullAABB;
		bool bHasCullSurface;

		std::array<TriangleGlyph, 2> Glyph;

		bool needsReconstructed;
		void recalculateVertexes();
	};
}