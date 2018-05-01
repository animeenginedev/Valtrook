#include "ATexturedRect.h"


namespace Val {
	ATexturedRect::ATexturedRect(const TextureResource & texture, std::array<float, 2> center, std::array<float, 2> halfSize, float depth, Colour colour, const GLBlendMode & blendMode) : ARect(center, halfSize, depth), texture(texture), uvBounds(texture.getBounds()), colour(colour), blendMode(blendMode) {
	}
	ATexturedRect::~ATexturedRect() {
	}
	void ATexturedRect::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void ATexturedRect::setHalfWidth(float h_width) {
		halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void ATexturedRect::setHalfHeight(float h_height) {
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void ATexturedRect::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void ATexturedRect::setHalfSize(float h_width, float h_height) {
		halfSize[0] = h_width;
		halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void ATexturedRect::setColour(const Colour & colour) {
		this->colour = colour;
		needsReconstructed = true;
	}
	void ATexturedRect::setUV(const UV & uv) {
		this->uvBounds = uv;
		needsReconstructed = true;
	}
	void ATexturedRect::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	Colour ATexturedRect::getColour() const {
		return colour;
	}
	UV ATexturedRect::getUV() const {
		return uvBounds;
	}
	GLBlendMode ATexturedRect::getBlendMode() const {
		return blendMode;
	}

}