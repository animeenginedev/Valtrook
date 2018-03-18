#include "TextureAsset.h"

#include "Logger.h"
#include "Exit.h"
#include "RuntimeConstants.h"
#include "AABB.h"
#include "UV.h"
#include "JsonReadWriter.h"
#include "Conversion.h"

#include <SDL_image.h>
#include <vector>

namespace Val {
	std::vector<Texture*> TextureAsset::textures;
	std::vector<AtlasTexture*> TextureAsset::atlasTextures;
	std::unordered_map<ResourceLocation, TextureSheet*> TextureAsset::textureSheets;

	const constexpr float atlasSize() { return 2048.0f; }

	TextureResource TextureAsset::getTexture(std::string name, std::string extension, bool atlasTexture, GLenum forceDifferentWrapping) {
		return getTexture(ResourceLocation(name, extension, RuntimeConstants::Instance->TexturePath), atlasTexture, forceDifferentWrapping);
	}

	TextureResource TextureAsset::getTexture(ResourceLocation filename, bool atlasTexture, GLenum forceDifferentWrapping) {
		for (unsigned int i = 0; i < textures.size(); i++) {
			if (textures[i]->hasBounds(filename)) {
				return TextureResource(textures[i], filename);
			}
		}

		Texture* texture = nullptr;
		if (atlasTexture) {
			AtlasTexture* atlas = nullptr;
			AABB<float> atlasAABB = AABB<float>(0.0f, 0.0f, 0.0f);
			GLTexture* t = new GLTexture(loadTexture(filename, forceDifferentWrapping));
			for (unsigned int i = 0; i < atlasTextures.size(); i++) {
				if (!atlasTextures[i]->filled) {
					atlasAABB = atlasTextures[i]->getSpace(t->operator[](0), t->operator[](1));
					if (atlasAABB.getHalfWidth() > Meter(0.0f)) {
						atlas = atlasTextures[i];
						break;
					}
				}
			}
			//No Atlas That can fit texture
			if (atlas == nullptr) {
				GLuint textureBuffer;
				glGenTextures(1, &textureBuffer);
				glBindTexture(GL_TEXTURE_2D, textureBuffer);

				//Set the stuff for the buffer...
				glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, static_cast<GLsizei>(atlasSize()), static_cast<GLsizei>(atlasSize()), 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

				std::vector<unsigned char> pixels = std::vector<unsigned char>();
				pixels.resize(t->operator[](0) * t->operator[](1) * 4);
				glBindTexture(GL_TEXTURE_2D, t->getTextureID());
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
				glBindTexture(GL_TEXTURE_2D, textureBuffer);
				glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, t->operator[](0), t->operator[](1), GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);

				atlas = new AtlasTexture(new GLTexture(textureBuffer, static_cast<unsigned int>(atlasSize()), static_cast<unsigned int>(atlasSize())));
				texture = dynamic_cast<Texture*>(atlas);
				atlasTextures.push_back(atlas);
				textures.push_back(texture);
				atlas->addBounds(ResourceLocation("FULL", "", ""), UV(0.0f, 0.0f, 1.0f, 1.0f));
				atlas->addBounds(filename, UV(0.0f, 0.0f, t->operator[](0) / atlasSize(), t->operator[](1) / atlasSize()));
				atlas->addSpace(AABB<float>(
					                 UnalignedPixelToWorld<float>(t->operator[](0) / 2.0f),
					UnalignedPixelToWorld<float>(t->operator[](1) / 2.0f),
					UnalignedPixelToWorld<float>(t->operator[](0) / 2.0f),
					UnalignedPixelToWorld<float>(t->operator[](1) / 2.0f)));
			} else {
				std::vector<unsigned char> pixels = std::vector<unsigned char>();
				pixels.resize(t->operator[](0) * t->operator[](1) * 4);
				glBindTexture(GL_TEXTURE_2D, t->getTextureID());
				glGetTexImage(GL_TEXTURE_2D, 0, GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);
				glBindTexture(GL_TEXTURE_2D, atlas->getGLTexture()->getTextureID());
				glTexSubImage2D(GL_TEXTURE_2D, 0,
								WorldToPixel<unsigned int, float>(atlasAABB.getX() - atlasAABB.getHalfWidth()),
								WorldToPixel<unsigned int, float>(atlasAABB.getY() - atlasAABB.getHalfHeight()),
								t->operator[](0), t->operator[](1), GL_RGBA, GL_UNSIGNED_BYTE, &pixels[0]);


				texture = dynamic_cast<Texture*>(atlas);
				atlas->addBounds(filename, UV(WorldToPixel<unsigned int, float>(atlasAABB.getX() - atlasAABB.getHalfWidth()),
											  WorldToPixel<unsigned int, float>(atlasAABB.getY() - atlasAABB.getHalfHeight()),
													t->operator[](0), t->operator[](1),
													static_cast<int>(atlasSize()), static_cast<int>(atlasSize())));
			}

			glBindTexture(GL_TEXTURE_2D, t->getTextureID());
			glDeleteTextures(1, &t->getTextureIDForGL());
			delete t;
		} else {
			texture = new Texture(new GLTexture(loadTexture(filename, forceDifferentWrapping)));

			texture->addBounds(filename, UV(0.0f, 0.0f, 1.0f, 1.0f));
			textures.push_back(texture);
		}

		return TextureResource(texture, filename);
	}

	TextureSheet * TextureAsset::getTextureSheet(ResourceLocation filename) {
		auto it = textureSheets.find(filename);
		if (it != textureSheets.end()) {
			return it->second;
		}

		TextureSheet* tSheet = new TextureSheet();

		JsonReadWriter readWriter = JsonReadWriter(RuntimeConstants::Instance->TextureSheetPath, filename.getName() + ".dat");
		readWriter.readAllDataFromFile();
		auto jsonRoot = *readWriter.getJsonData();

		GLTexture* t = new GLTexture(loadTexture(ResourceLocation(filename.getName(), "", RuntimeConstants::Instance->TexturePath), GL_CLAMP_TO_EDGE));

		for (int i = 0; i < jsonRoot["ENTRIES"].get<int>(); i++) {
			auto jsonIndex = jsonRoot[std::to_string(i)];

			std::string name = jsonIndex["NAME"].begin().value();
			std::string fileName = jsonIndex["FILE"].begin().value();

			tSheet->addFrame(std::string(name),
							 getTexture(ResourceLocation(fileName, "", ""), true));
		}

		glBindTexture(GL_TEXTURE_2D, t->getTextureID());
		glDeleteTextures(1, &t->getTextureIDForGL());
		delete t;

		textureSheets.insert(std::make_pair(filename, tSheet));
		return tSheet;
	}

	GLTexture TextureAsset::loadTexture(ResourceLocation filename, GLenum wrapping) {

		SDL_Surface* surface = IMG_Load(filename.getLocation().c_str());

		if (surface == nullptr) {
			return GLTexture(0, 0, 0);
		}

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

		SDL_Surface* rgbSurface = SDL_CreateRGBSurface(0, surface->w, surface->h, 32, rmask, gmask, bmask, amask);
		SDL_BlitSurface(surface, 0, rgbSurface, 0);

		int mode = GL_RGB;
		GLuint textureId;

		glGenTextures(1, &(textureId));
		glBindTexture(GL_TEXTURE_2D, textureId);

		if (surface->format->BytesPerPixel == 4) {
			mode = GL_RGBA;
		}

		glTexImage2D(GL_TEXTURE_2D, 0, mode, surface->w, surface->h, 0, mode, GL_UNSIGNED_BYTE, surface->pixels);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapping);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

		glGenerateMipmap(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, 0);

		GLTexture texture = GLTexture(textureId, surface->w, surface->h);

		SDL_FreeSurface(surface);
		SDL_FreeSurface(rgbSurface);

		return texture;
	}
}