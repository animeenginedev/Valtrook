#include "ATexturedRect.h"


namespace Val {
	void ATexturedRect::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Renderable>(), "Renderable");

		script->add(chaiscript::fun<void, Renderable, RenderingEngine*>(&Renderable::sendRenderInformation), "sendRenderInformation");
		script->add(chaiscript::fun<void, Renderable, VBOBatcher*>(&Renderable::sendRenderInformation), "sendRenderInformation");

		script->add(chaiscript::user_type<ARect>(), "ARect");
		script->add(chaiscript::base_class<Renderable, ARect>());

		script->add(chaiscript::fun(&ARect::setX), "setX");
		script->add(chaiscript::fun(&ARect::setY), "setY");
		script->add(chaiscript::fun(&ARect::setDepth), "setDepth");
		script->add(chaiscript::fun(&ARect::getX), "getX");
		script->add(chaiscript::fun(&ARect::getY), "getY");
		script->add(chaiscript::fun(&ARect::getDepth), "getDepth");
		script->add(chaiscript::fun(&ARect::getCenter), "getCenter");
		script->add(chaiscript::fun(&ARect::getHalfWidth), "getHalfWidth");
		script->add(chaiscript::fun(&ARect::getHalfHeight), "getHalfHeight");
		script->add(chaiscript::fun(&ARect::getHalfSize), "getHalfSize");


		script->add(chaiscript::user_type<ATexturedRect>(), "ATexturedRect");
		script->add(chaiscript::base_class<ARect, ATexturedRect>());

		script->add(chaiscript::fun(&ATexturedRect::setTexture), "setTexture");
		script->add(chaiscript::fun(&ATexturedRect::setHalfWidth), "setHalfWidth");
		script->add(chaiscript::fun(&ATexturedRect::setHalfHeight), "setHalfHeight");
		script->add(chaiscript::fun(&ATexturedRect::setColour), "setColour");
		script->add(chaiscript::fun(&ATexturedRect::setBlendMode), "setBlendMode");

		script->add(chaiscript::fun(&ATexturedRect::getColour), "getColour");
		script->add(chaiscript::fun(&ATexturedRect::getBlendMode), "getBlendMode");
	}
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