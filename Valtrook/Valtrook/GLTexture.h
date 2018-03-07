#pragma once

#include <array>
#include <GL\glew.h>

namespace Val {
	struct GLTexture {
		GLTexture();
		GLTexture(GLuint id, unsigned int width, unsigned int height);
		GLTexture(GLuint id, std::array<unsigned int, 2> size);

		GLuint& getTextureIDForGL();

		GLuint getTextureID() const;
		unsigned int getPixelWidth() const;
		unsigned int getPixelHeight() const;
		std::array<unsigned int, 2> getPixelSize() const;

		const bool operator== (const GLTexture& rhs);
		const bool operator!= (const GLTexture& rhs);
		unsigned int operator[](const std::size_t& index) {
			return size[index];
		}
	protected:
		GLuint id;
		std::array<unsigned int, 2> size;
	};
}