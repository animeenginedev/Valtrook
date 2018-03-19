#include "Texture.h"

#include "Conversion.h"

namespace Val {
	Texture::Texture(GLTexture * texture) : texture(texture), defaultBounds(0.0f, 0.0f, 1.0f, 1.0f) {
	}

	Texture::~Texture() {
	}

	GLTexture * Texture::getGLTexture() {
		return texture;
	}

	bool Texture::hasBounds(ResourceLocation resourceName) {
		return (bounds.find(resourceName) != bounds.end());
	}

	UV Texture::getBounds(ResourceLocation resourceName) {
		auto res = bounds.find(resourceName);
		if (res != bounds.end())
			return res->second;
		return defaultBounds;
	}

	void Texture::addBounds(ResourceLocation resourceName, UV bounds) {
		this->bounds.insert(std::make_pair(resourceName, bounds));
	}

	TextureResource::TextureResource(Texture * texture, ResourceLocation resourceName) : texture(texture), resourceName(resourceName) {
	}

	void TextureResource::reinitialize(Texture * texture, ResourceLocation resourceName) {
		this->texture = texture;
		this->resourceName = resourceName;
	}

	GLTexture * TextureResource::getGLTexture() {
		return texture->getGLTexture();
	}

	Texture * TextureResource::getTexture() {
		return texture;
	}

	UV TextureResource::getBounds() const {
		return texture != nullptr ? texture->getBounds(resourceName) : UV();
	}

	ResourceLocation TextureResource::getResourceName() {
		return resourceName;
	}


	std::array<unsigned int, 2> TextureResource::getTextureSizeInPixel() {
		return{ static_cast<unsigned int>(abs(this->getBounds().uWidth) * texture->getGLTexture()->operator[](0)),
			static_cast<unsigned int>(abs(this->getBounds().vHeight) * texture->getGLTexture()->operator[](1)) };
	}

	void Texture::setDefaultBounds(UV bounds) {
		this->defaultBounds = bounds;
	}

	AtlasTexture::AtlasTexture(GLTexture * texture) : Texture(texture), filled(false) {
	}

	AtlasTexture::~AtlasTexture() {
	}

	AABB<float> AtlasTexture::getSpace(unsigned int textureWidth, unsigned int textureHeight) {
		float texWidthOver2 = static_cast<float>(textureWidth) / 2.0f;
		float texHeightOver2 = static_cast<float>(textureHeight) / 2.0f;

		float cX = UnalignedPixelToWorld<Meter>(texWidthOver2);
		float cY = UnalignedPixelToWorld<Meter>(texHeightOver2);
		float inW = UnalignedPixelToWorld<Meter>(static_cast<float>(texture->operator[](0)) - texWidthOver2);
		float inH = UnalignedPixelToWorld<Meter>(static_cast<float>(texture->operator[](1)) - texHeightOver2);

		AABB<float> inputAABB = AABB<float>(cX, cY, cX, cY);

		for (float h = cY; h < inH; h = h + PixelToWorld<Pixel, Meter>(10)) {
			for (float w = cX; w < inW; w = w + PixelToWorld<Pixel, Meter>(10)) {
				inputAABB.setCenter(w, h);
				if (!aabbInFilledSpace(inputAABB)) {
					filledSpace.push_back(inputAABB);
					return inputAABB;
				}
			}
		}

		filled = true;
		return AABB<float>(-1.0f, -1.0f, -1.0f);
	}

	void AtlasTexture::addSpace(AABB<float> aabb) {
		filledSpace.push_back(aabb);
	}

	bool AtlasTexture::aabbInFilledSpace(AABB<float> aabb) {
		for (auto it : filledSpace) {
			if (it.intersectsAABB(aabb)) {
				return true;
			}
		}

		return false;
	}
}