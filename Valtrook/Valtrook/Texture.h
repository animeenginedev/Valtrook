#pragma once

#include "GLTexture.h"

#include "AABB.h"
#include "UV.h"
#include "ResourceLocation.h"
#include <array>
#include <unordered_map>
#include <string>

namespace Val {
	class Texture;

	struct TextureResource {
		TextureResource() : resourceName("", "", "") {};
		TextureResource(Texture* texture, ResourceLocation resourceName);
		void reinitialize(Texture* texture, ResourceLocation resourceName);

		GLTexture* getGLTexture();
		Texture* getTexture();
		UV getBounds() const;
		ResourceLocation getResourceName();
		std::array<unsigned int, 2> getTextureSizeInPixel();

	protected:
		Texture* texture;
		ResourceLocation resourceName;
	};

	class Texture {
	public:
		static TextureResource errorTexture;

		Texture(GLTexture* texture);
		~Texture();

		GLTexture* getGLTexture();
		bool hasBounds(ResourceLocation resourceName);
		UV getBounds(ResourceLocation resourceName);

		virtual void addBounds(ResourceLocation resourceName, UV bounds);
		void setDefaultBounds(UV bounds);
	protected:
		GLTexture* texture;
		std::unordered_map<ResourceLocation, UV> bounds;

		UV defaultBounds;
	};

	//Uses a naive box packing implementation, but it still works.
	class AtlasTexture : public Texture {
	public:
		AtlasTexture(GLTexture* texture);
		~AtlasTexture();

		AABB<float> getSpace(unsigned int textureWidth, unsigned int textureHeight);
		void addSpace(AABB<float> aabb);

		bool filled;
	protected:
		std::vector<AABB<float>> filledSpace;

		bool aabbInFilledSpace(AABB<float> aabb);
	};
}