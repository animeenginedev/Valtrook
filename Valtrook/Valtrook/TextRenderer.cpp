#include "TextRenderer.h"

#include "Logger.h"
#include <SDL2\SDL.h>
#include "FontAsset.h"

namespace Val {
	std::unordered_map<TextResource, Texture*> TextRenderer::loadedTextures;

	TextureResource TextRenderer::getTexture(TextResource resource) {
		if (resource.getTextString() == ""  || resource.getFont() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Text requested, string\"" + resource.getTextString() + "\", " + (resource.getFont() == nullptr ? "font == nullptr" : "font != nullptr"));
			return TextureResource(nullptr, ResourceLocation("Invalid Text Render", "", ""));
		}

		if (loadedTextures.find(resource) == loadedTextures.end()) {
			auto tex = loadText(resource);
			loadedTextures.insert(std::make_pair(resource, tex));
		}
		return TextureResource(loadedTextures.at(resource), ResourceLocation(resource.getTextString(), ".TEXT", "Fuck You\\"));
	}

	void TextRenderer::clearText(TextResource resource) {
		//Cleanup memes
		if (loadedTextures.find(resource) != loadedTextures.end()) {
			auto Tex = loadedTextures.at(resource);
			Tex->getGLTexture()->getTextureIDForGL();
			if (Tex->getGLTexture()->getTextureIDForGL() != 0) {
				glDeleteTextures(1, &Tex->getGLTexture()->getTextureIDForGL());
			}
			delete Tex->getGLTexture();
		}
	}

	void TextRenderer::clearCache() {
		for (auto res : loadedTextures) {
			clearText(res.first);
			delete res.second;
		}
		loadedTextures.clear();
	}

	Texture * TextRenderer::loadText(TextResource res) {		
		SDL_Color c;
		c.r = 255;
		c.g = 255;
		c.b = 255;
		c.a = 255;

		SDL_Surface* textSurface = TTF_RenderText_Blended(res.getFont()->getTTF(), res.getTextString().c_str(), c);
		Uint32 rmask, gmask, bmask, amask;

#if SDL_BYTEORDER == SDL_BIG_ENDIAN
		rmask = 0xff000000;
		gmask = 0x00ff0000;
		bmask = 0x0000ff00;
		amask = 0x000000ff;
#else
		rmask = 0x000000ff;
		gmask = 0x0000ff00;
		bmask = 0x00ff0000;
		amask = 0xff000000;
#endif

		SDL_Surface* rgbSurface = SDL_CreateRGBSurface(0, textSurface->w, textSurface->h, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(textSurface, 0, rgbSurface, 0);

		int mode = GL_RGB;
		GLuint textureId = 0;
		
		glGenTextures(1, &textureId);
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (rgbSurface->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, rgbSurface->w, rgbSurface->h, 0, mode, GL_UNSIGNED_BYTE, rgbSurface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
				
		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		SDL_FreeSurface(textSurface);
		SDL_FreeSurface(rgbSurface);

		return new Texture(new GLTexture(textureId, textSurface->w, textSurface->h));
	}
}