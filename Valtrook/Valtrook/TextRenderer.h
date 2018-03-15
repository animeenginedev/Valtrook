#pragma once

#include "TextResource.h"

#include "Texture.h"

#include <sdl_ttf.h>
#include <unordered_map>


namespace Val {
	class TextRenderer {
	public:
		static TextureResource getTexture(TextResource resource);

		//Clear a specific piece of text
		static void clearText(TextResource resource);
		//Very Expensive, clears every loaded piece of text; Will also clear those being used so they have to be re rendered.
		static void clearCache();
	protected:
		static Texture* loadText(TextResource res);

		static std::unordered_map<TextResource, Texture*> loadedTextures;
	};
}