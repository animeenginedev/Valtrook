#include "SimpleMultilineTextRectangle.h"

#include "RenderingEngine.h"
#include "VBOBatcher.h"
#include "TextRenderer.h"
#include "Logger.h"
#include "Engine.h"
#include "GlyphCuller.h"
namespace Val {
	SimpleMultilineTextRectangle::SimpleMultilineTextRectangle(const TextResource & texture, float x, float y, float depth, float maxWidthPerLine, float heightPerLine) : SimpleMultilineTextRectangle(texture, { x, y }, depth, maxWidthPerLine, heightPerLine, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleMultilineTextRectangle::SimpleMultilineTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, float maxWidthPerLine, float heightPerLine) : SimpleMultilineTextRectangle(texture, center, depth, maxWidthPerLine, heightPerLine, Colour(255, 255, 255, 255), GLBlendMode::Blend_Default) {
	}
	SimpleMultilineTextRectangle::SimpleMultilineTextRectangle(const TextResource & texture, float x, float y, float depth, float maxWidthPerLine, float heightPerLine, Colour colour, const GLBlendMode & blendMode) : SimpleMultilineTextRectangle(texture, { x, y }, depth, maxWidthPerLine, heightPerLine, colour, blendMode) {
	}
	SimpleMultilineTextRectangle::SimpleMultilineTextRectangle(const TextResource & texture, std::array<float, 2> center, float depth, float maxWidthPerLine, float heightPerLine, Colour colour, const GLBlendMode & blendMode) : 
	    textResourceMaster(texture), center(center), depth(depth), halfWidthPerLine(maxWidthPerLine), halfHeightPerLine(heightPerLine), singleLineMode(false), justification(hCENTER), renderColour(colour), blendMode(blendMode), needsReconstructed(true), needsTextReconstructed(true) {
	}
	SimpleMultilineTextRectangle::~SimpleMultilineTextRectangle() {
	}
	void SimpleMultilineTextRectangle::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<SimpleMultilineTextRectangle>(), "SimpleMultilineTextRectangle");

		script->add(chaiscript::constructor<SimpleMultilineTextRectangle(TextResource, float, float, float, float, float)>(), "SimpleMultilineTextRectangle");
		script->add(chaiscript::constructor<SimpleMultilineTextRectangle(TextResource, std::array<float, 2>, float, float, float)>(), "SimpleMultilineTextRectangle");
		script->add(chaiscript::constructor<SimpleMultilineTextRectangle(TextResource, float, float, float, float, float, Colour, GLBlendMode)>(), "SimpleMultilineTextRectangle");
		script->add(chaiscript::constructor<SimpleMultilineTextRectangle(TextResource, std::array<float, 2>, float, float, float, Colour, GLBlendMode)>(), "SimpleMultilineTextRectangle");

		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setTextResource), "setTextResource");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setText), "setText");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setFont), "setFont");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setX), "setX");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setY), "setY");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setDepth), "setDepth");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setHalfWidthPerLine), "setHalfWidthPerLine");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setHalfHeightPerLine), "setHalfHeightPerLine");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setColour), "setColour");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setBlendMode), "setBlendMode");

		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getTextResource), "getTextResource");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getText), "getText");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getFont), "getFont");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getX), "getX");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getY), "getY");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getDepth), "getDepth");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getCenter), "getCenter");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfWidth), "getHalfWidth");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfHeight), "getHalfHeight");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfSize), "getHalfSize");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfWidthPerLine), "getHalfWidthPerLine");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfHeightPerLine), "getHalfHeightPerLine");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getHalfSizePerLine), "getHalfSizePerLine");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getColour), "getColour");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getBlendMode), "getBlendMode");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::getJustification), "getJustification");

		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::reconstruct), "reconstruct");

		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::setSingleLineMode), "setSingleLineMode");
		script->add(chaiscript::fun(&SimpleMultilineTextRectangle::isSingleLineMode), "isSingleLineMode");


		script->add(chaiscript::fun<void, SimpleMultilineTextRectangle, RenderingEngine*>(&SimpleMultilineTextRectangle::sendRenderInformation), "sendRenderInformation");
		script->add(chaiscript::fun<void, SimpleMultilineTextRectangle, VBOBatcher*>(&SimpleMultilineTextRectangle::sendRenderInformation), "sendRenderInformation");
	}
	void SimpleMultilineTextRectangle::setTextResource(const TextResource & text) {
		textResourceMaster = text;
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setText(const std::string & contents) {
		textResourceMaster = TextResource(textResourceMaster.getFont(), contents);
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setFont(FontAsset * newFont) {
		textResourceMaster = TextResource(newFont, textResourceMaster.getTextString());
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setX(float x) {
		this->center[0] = x;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setY(float y) {
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setCenter(const std::array<float, 2>& center) {
		this->center = center;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setCenter(float x, float y) {
		this->center[0] = x;
		this->center[1] = y;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setHalfWidthPerLine(float hwpl) {
		halfWidthPerLine = hwpl;
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setHalfHeightPerLine(float hhpl) {
		halfHeightPerLine = hhpl;
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setHalfSizePerLine(const std::array<float, 2>& hspl) {
		halfWidthPerLine = hspl[0];
		halfHeightPerLine = hspl[1];
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setSize(float width, float height) {
		this->halfSize[0] = width / 2.0f;
		this->halfSize[1] = height / 2.0f;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setColour(const Colour & colour) {
		this->renderColour = colour;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		bHasCullSurface = !(cullAABB == noCullSurface);
		needsReconstructed = true;
	}
	void SimpleMultilineTextRectangle::setJustification(HorizontalJustification justification) {
		this->justification = justification;
		this->needsReconstructed = true;
	}
	TextResource SimpleMultilineTextRectangle::getTextResource() const {
		return textResourceMaster;
	}
	std::string SimpleMultilineTextRectangle::getText() const {
		return textResourceMaster.getTextString();
	}
	FontAsset * SimpleMultilineTextRectangle::getFont() const {
		return textResourceMaster.getFont();
	}
	float SimpleMultilineTextRectangle::getX() const {
		return center[0];
	}
	float SimpleMultilineTextRectangle::getY() const {
		return center[1];
	}
	float SimpleMultilineTextRectangle::getDepth() const {
		return depth;
	}
	std::array<float, 2> SimpleMultilineTextRectangle::getCenter() const {
		return center;
	}
	float SimpleMultilineTextRectangle::getHalfWidth() const {
		return halfSize[0];
	}
	float SimpleMultilineTextRectangle::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> SimpleMultilineTextRectangle::getHalfSize() const {
		return halfSize;
	}
	float SimpleMultilineTextRectangle::getHalfWidthPerLine() const {
		return halfWidthPerLine;
	}
	float SimpleMultilineTextRectangle::getHalfHeightPerLine() const {
		return halfHeightPerLine;
	}
	std::array<float, 2> SimpleMultilineTextRectangle::getHalfSizePerLine() const {
		return{halfWidthPerLine, halfHeightPerLine};
	}
	Colour SimpleMultilineTextRectangle::getColour() const {
		return renderColour;
	}
	GLBlendMode SimpleMultilineTextRectangle::getBlendMode() const {
		return blendMode;
	}
	AABB<float> SimpleMultilineTextRectangle::getCullSurface() const {
		return cullAABB;
	}
	HorizontalJustification SimpleMultilineTextRectangle::getJustification() const {
		return justification;
	}
	void SimpleMultilineTextRectangle::reconstruct() {
		recalculateVertexes();
	}
	void SimpleMultilineTextRectangle::setSingleLineMode(bool singlieLineMode) {
		this->singleLineMode = singleLineMode;
		needsReconstructed = true;
		needsTextReconstructed = true;
	}
	bool SimpleMultilineTextRectangle::isSingleLineMode() const {
		return singleLineMode;
	}
	std::vector<std::array<TriangleGlyph, 2>> SimpleMultilineTextRectangle::getRenderGlyphs() {
		return Glyph;
	}
	void SimpleMultilineTextRectangle::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			recalculateVertexes();

		engine->getRenderer()->add(Glyph);
	}
	void SimpleMultilineTextRectangle::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			recalculateVertexes();

		batcher->add(Glyph);
	}
	void SimpleMultilineTextRectangle::recalculateVertexes() {
		if (textResourceMaster.getTextString() == "")
			return;

		needsReconstructed = false;
		//Break the master text thing into multiple lines.
		if (needsTextReconstructed) {
			needsTextReconstructed = false;
			textResourcePerLine.clear();

			WordString fullString = textResourceMaster.getTextString();
			WordString attemptString = fullString;
		
			float attemptWidth = 0.0f;
			unsigned int count = 0;
			bool usingWords = true;
			while (fullString.getNumberOfWords() != 0) {
				attemptString = fullString;
				attemptWidth = textResourceMaster.getScaledTextWidth(attemptString.concatenate(), halfHeightPerLine);

				usingWords = true;
				count = attemptString.getNumberOfWords();
				if (!singleLineMode) {
					while (attemptWidth > halfWidthPerLine) {
						if (usingWords) {
							attemptString = attemptString.concatenateWords(count);
						} else {
							attemptString = attemptString.concatenateCharacters(count);
						}
						attemptWidth = textResourceMaster.getScaledTextWidth(attemptString.concatenate(), halfHeightPerLine);
						count--;
						if (count == 0) {
							usingWords = false;
							count = attemptString.getNumberOfCharacters();
						}
					}

					//Give up if we can't fit a character on a line.
					if (attemptString.getNumberOfCharacters() == 0)
						return;
				}
				fullString.removeFromFront(attemptString);

				textResourcePerLine.push_back(TextResource(textResourceMaster.getFont(), attemptString.concatenate()));
			}
		}

		halfSize[0] = halfWidthPerLine;
		halfSize[1] = (textResourcePerLine.size()) * halfHeightPerLine;
		//Construct Glyphs
		Glyph.clear();
		float glyphHeight = center[1] + ((textResourcePerLine.size()) * halfHeightPerLine);
		for (auto& textResource : textResourcePerLine) {
			glyphHeight -= halfHeightPerLine;
			auto texture = TextRenderer::getTexture(textResource);
			Colour renderColour = this->renderColour;

			if (texture.getTexture() == nullptr) {
				Logger::Instance->logMessage(WARNING, "Invalid Texture on Simple MultiText Rectangle");
				texture = Texture::errorTexture;
				renderColour = Colour(255, 255, 255, this->renderColour.a);
			}

			auto uvBounds = texture.getBounds();

			float glyphX = 0.0f;
			float glyphWidth = textResource.getScaledTextWidth(halfHeightPerLine);
			switch (justification) {
				case LEFT:
					glyphX = center[0] + (glyphWidth - halfWidthPerLine);
					break;
				case hCENTER:
					glyphX = center[0];
					break;
				case RIGHT:
					glyphX = center[0] - (glyphWidth - halfWidthPerLine);
					break;
			}

			std::array<float, 2> centerUPixel = { WorldToUnalignedPixel<float>(glyphX), WorldToUnalignedPixel<float>(glyphHeight) };
			std::array<float, 2> halfSizeUPixel = { WorldToUnalignedPixel<float>(glyphWidth), WorldToUnalignedPixel<float>(halfHeightPerLine) };

			auto Glyph = RectangleGlyph(texture.getGLTexture()->getTextureID(), std::array<Vertex, 4>( {

				Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v, renderColour),
				Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] + halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v, renderColour),
				Vertex(centerUPixel[0] - halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u, uvBounds.v + uvBounds.vHeight, renderColour),
				Vertex(centerUPixel[0] + halfSizeUPixel[0], centerUPixel[1] - halfSizeUPixel[1], depth, uvBounds.u + uvBounds.uWidth, uvBounds.v + uvBounds.vHeight, renderColour)

			}), &blendMode);

			this->Glyph.push_back(bHasCullSurface ? GlyphCuller::cullRectangle(Glyph, cullAABB).dispose() : Glyph.dispose());

			glyphHeight -= halfHeightPerLine;
		}
	}
}