#include "Rectangle.h"

#include "Conversion.h"
#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "Logger.h"
#include "Rectangle.h"

namespace Val {
	Rectangle::Rectangle(const TextureResource & texture) : Rectangle(texture, { 0.0f, 0.0f }, 0.5f, { 1.0f, 1.0f }, 0.0f, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation) : Rectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation) : Rectangle(texture, center, depth, halfSize, rotation, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	Rectangle::Rectangle(const TextureResource & texture, float x, float y, float depth, float halfWidth, float halfHeight, float rotation, Colour colour, const GLBlendMode & blendMode) : Rectangle(texture, { x, y }, depth, { halfWidth, halfHeight }, rotation, colour, blendMode) {
	}
	Rectangle::Rectangle(const TextureResource & texture, std::array<float, 2> center, float depth, std::array<float, 2> halfSize, float rotation, Colour colour, const GLBlendMode & blendMode) :
		texture(texture), center(center), depth(depth), halfSize(halfSize), rotation(rotation), renderColour(colour), blendMode(blendMode), uvBounds(texture.getBounds()), needsReconstructed(true) {
	}
	Rectangle::~Rectangle() {
	}

	void Rectangle::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<Rectangle>(), "Rectangle");

		script->add(chaiscript::constructor<Rectangle(TextureResource)>(), "Rectangle");
		script->add(chaiscript::constructor<Rectangle(TextureResource, float, float, float, float, float, float)>(), "Rectangle");
		script->add(chaiscript::constructor<Rectangle(TextureResource, std::array<float, 2>, float, std::array<float, 2>, float)>(), "Rectangle");
		script->add(chaiscript::constructor<Rectangle(TextureResource, float, float, float, float, float, float, Colour, GLBlendMode)>(), "Rectangle");
		script->add(chaiscript::constructor<Rectangle(TextureResource, std::array<float, 2>, float, std::array<float, 2>, float, Colour, GLBlendMode)>(), "Rectangle");

		script->add(chaiscript::fun(&Rectangle::setTexture), "setTexture");
		script->add(chaiscript::fun(&Rectangle::setX), "setX");
		script->add(chaiscript::fun(&Rectangle::setY), "setY");
		script->add(chaiscript::fun(&Rectangle::setDepth), "setDepth");
		script->add(chaiscript::fun(&Rectangle::setHalfWidth), "setHalfWidth");
		script->add(chaiscript::fun(&Rectangle::setHalfHeight), "setHalfHeight");
		script->add(chaiscript::fun(&Rectangle::setColour), "setColour");
		script->add(chaiscript::fun(&Rectangle::setRotation), "setRotation");
		script->add(chaiscript::fun(&Rectangle::setBlendMode), "setBlendMode");

		script->add(chaiscript::fun(&Rectangle::getTexture), "getTexture");
		script->add(chaiscript::fun(&Rectangle::getX), "getX");
		script->add(chaiscript::fun(&Rectangle::getY), "getY");
		script->add(chaiscript::fun(&Rectangle::getDepth), "getDepth");
		script->add(chaiscript::fun(&Rectangle::getCenter), "getCenter");
		script->add(chaiscript::fun(&Rectangle::getHalfWidth), "getHalfWidth");
		script->add(chaiscript::fun(&Rectangle::getHalfHeight), "getHalfHeight");
		script->add(chaiscript::fun(&Rectangle::getHalfSize), "getHalfSize");
		script->add(chaiscript::fun(&Rectangle::getColour), "getColour");
		script->add(chaiscript::fun(&Rectangle::getRotation), "getRotation");
		script->add(chaiscript::fun(&Rectangle::getBlendMode), "getBlendMode");


		script->add(chaiscript::fun<void, Rectangle, RenderingEngine*>(&Rectangle::sendRenderInformation), "sendRenderInformation");
		script->add(chaiscript::fun<void, Rectangle, VBOBatcher*>(&Rectangle::sendRenderInformation), "sendRenderInformation");
	}

	void Rectangle::setTexture(const TextureResource & texture) {
		this->texture = texture;
		this->uvBounds = texture.getBounds();
		needsReconstructed = true;
	}
	void Rectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void Rectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void Rectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void Rectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void Rectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void Rectangle::setHalfWidth(float h_width) {
		this->halfSize[0] = h_width;
		needsReconstructed = true;
	}
	void Rectangle::setHalfHeight(float h_height) {
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void Rectangle::setHalfSize(const std::array<float, 2>& halfSize) {
		this->halfSize = halfSize;
		needsReconstructed = true;
	}
	void Rectangle::setHalfSize(float h_width, float h_height) {
		this->halfSize[0] = h_width;
		this->halfSize[1] = h_height;
		needsReconstructed = true;
	}
	void Rectangle::setWidth(float width) {
		this->halfSize[0] = width / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setHeight(float height) {
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void Rectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
		needsReconstructed = true;
	}
	void Rectangle::setUV(const UV & uv) {
		this->uvBounds = uv;
		needsReconstructed = true;
	}
	void Rectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void Rectangle::setRotation(float rotation) {
		this->rotation = rotation;
		needsReconstructed = true;
	}
	TextureResource Rectangle::getTexture() const {
		return texture;
	}
	float Rectangle::getX() const {
		return center[0];
	}
	float Rectangle::getY() const {
		return center[1];
	}
	float Rectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> Rectangle::getCenter() const {
		return center;
	}
	float Rectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float Rectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> Rectangle::getHalfSize() const {
		return halfSize;
	}
	Colour Rectangle::getColour() const {
		return renderColour;
	}
	UV Rectangle::getUV() const {
		return uvBounds;
	}
	GLBlendMode Rectangle::getBlendMode() const {
		return blendMode;
	}
	float Rectangle::getRotation() const {
		return rotation;
	}

	std::array<TriangleGlyph, 2> Rectangle::getRenderGlyphs() {
		if (needsReconstructed)
			recalculateVertexes();
		return Glyph;
	}
	void Rectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void Rectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}

	void Rectangle::recalculateVertexes() {
		needsReconstructed = false;

		if (texture.getTexture() == nullptr) {
			Logger::Instance->logMessage(WARNING, "Invalid Texture on Rectangle");
			texture = Texture::errorTexture;
		}

		std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(center[0]), WorldToUnalignedPixel<float>(center[1]) };
		std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(halfSize[0]), WorldToUnalignedPixel<float>(halfSize[1]) };

		float cosAngle = static_cast<float>(cos(rotation));
		float sinAngle = static_cast<float>(sin(rotation));

		float hCos = halfSizeUPixel[1] * cosAngle;
		float wCos = halfSizeUPixel[0] * cosAngle;
		float hSin = halfSizeUPixel[1] * sinAngle;
		float wSin = halfSizeUPixel[0] * sinAngle;
		
		Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>({

			Vertex(centerUPixel[0] - wCos + hSin, centerUPixel[1] - hCos - wSin, depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] + wCos + hSin, centerUPixel[1] - hCos + wSin, depth, uvBounds.u, uvBounds.v, renderColour),
			Vertex(centerUPixel[0] - wCos - hSin, centerUPixel[1] + hCos - wSin , depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour),
			Vertex(centerUPixel[0] + wCos - hSin, centerUPixel[1] + hCos + wSin, depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour)

		}), &blendMode).dispose();
	}
}