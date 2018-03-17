#pragma once

#include "Rectangle.h"
#include "TextResource.h"

namespace Val {

	class SimpleTextRectangle : public SimpleRectangle {
	public:
		SimpleTextRectangle();
		~SimpleTextRectangle();

		virtual void initialise(TextResource text, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const Colour& colour = Colour(255, 255, 255, 255), const GLBlendMode& blendMode = GLBlendMode::Blend_Default);

		void setText(const std::string& contents);
		std::string getText() const;		
		void changeFont(FontAsset* newFont);
		FontAsset* getFont() const;

		void reconstruct();

		void setScaleTextToHeight(const bool& scaleTextToHeight);
		bool doesScaleTextToHeight() const;
	protected:
		void initialise(const TextureResource& texture, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const Colour& colour = Colour(255, 255, 255, 255), const GLBlendMode& blendMode = GLBlendMode::Blend_Default) override;

		TextResource textResource;
		bool scaleTextToHeight;

		void recalculateVertexes() override;
	};

	class TextRectangle : public SimpleTextRectangle {
	public:
		TextRectangle();
		~TextRectangle();

		virtual void initialise(TextResource text, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const float& rotation, const Colour& colour = Colour(255, 255, 255, 255), const GLBlendMode& blendMode = GLBlendMode::Blend_Default);

		void setRotation(const float& rotation);

		float getRotation() const;
	protected:
		float rotation;
		
		void recalculateVertexes() override;

		void initialise(TextResource text, const float& x, const float& y, const float& depth, const float& halfWidth, const float& halfHeight, const Colour& colour = Colour(255, 255, 255, 255), const GLBlendMode& blendMode = GLBlendMode::Blend_Default) override;
	};
}