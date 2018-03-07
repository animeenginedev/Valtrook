#include "GLTexture.h"

Val::GLTexture::GLTexture() : id(0), size({ 0, 0 }) {
}

Val::GLTexture::GLTexture(GLuint id, unsigned int width, unsigned int height) : id(id), size({ width, height }) {
}

Val::GLTexture::GLTexture(GLuint id, std::array<unsigned int, 2> size) : id(id), size(size) {
}

GLuint & Val::GLTexture::getTextureIDForGL() {
	return id;
}

GLuint Val::GLTexture::getTextureID() const {
	return id;
}

unsigned int Val::GLTexture::getPixelWidth() const {
	return size[0];
}

unsigned int Val::GLTexture::getPixelHeight() const {
	return size[1];
}

std::array<unsigned int, 2> Val::GLTexture::getPixelSize() const {
	return size;
}

const bool Val::GLTexture::operator==(const GLTexture & rhs) {
	return id == rhs.id;
}

const bool Val::GLTexture::operator!=(const GLTexture & rhs) {
	return id != rhs.id;
}
