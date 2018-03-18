#pragma once

#include "GLTexture.h"
#include "Texture.h"
#include "TextureSheet.h"
#include "ResourceLocation.h"

#include <SDL2/SDL.h>
#include <string>
#include <vector>
#include <unordered_map>

namespace Val {
	class TextureAsset {
	public:
		static TextureResource getTexture(std::string name, std::string extension = ".png", bool atlasTexture = true, GLenum forceDifferentWrapping = GL_CLAMP_TO_EDGE);
		static TextureResource getTexture(ResourceLocation filename, bool atlasTexture = true, GLenum forceDifferentWrapping = GL_CLAMP_TO_EDGE);
		//Texture sheets are forceully atlas'd.
		static TextureSheet* getTextureSheet(ResourceLocation filename);

	private:
		static std::vector<Texture*> textures;
		static std::vector<AtlasTexture*> atlasTextures;
		static std::unordered_map<ResourceLocation, TextureSheet*> textureSheets;

		static GLTexture loadTexture(ResourceLocation filename, GLenum wrapping);
	};

}