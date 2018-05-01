#pragma once

#include "GUIBase.h"
#include "Glyph.h"
#include "SimpleRectangle.h"
#include "SimpleTextRectangle.h"
#include "Line.h"

namespace Val {
	class GUI_InputField : public GUIBase {
	public:
		typedef std::shared_ptr<GUI_InputField> Ptr;
		static GUI_InputField::Ptr Create(TextResource textRes, float textHalfHeight, Colour textColour = Colour(0, 0, 0, 255), float highlightBonusHalfHeight = 0.05f, Colour highlightColour = Colour(0, 0, 255, 255), Colour flickerColour = Colour(255, 255, 255, 255));

		GUI_InputField(TextResource textRes, float textHalfHeight, Colour textColour, float highlightBonusHalfHeight, Colour highlightColour, Colour flickerColour);
		~GUI_InputField();

		void setText(const std::string& str);
		void setCursorPosition(unsigned int position);
		void setHighlightPositions(unsigned int start, unsigned int end);
		void setTextColour(Colour c);
		void setHighlightColour(Colour c);
		void setLineFlickerColour(Colour c);

		std::string getText() const;
		std::string getHighlightedText() const;
		unsigned int getCursorPosition() const;
		std::array<unsigned int, 2> getHighlightPositions() const;
		Colour getTextColour() const;
		Colour getHighlightColour() const;
		Colour getLineFlickerColour() const;
		
		void setHighlightEnabled(bool enableHighlight);
		bool isHighlightEnabled() const;
	protected:
		void leftMouseDown() override;
		void middleMouseDown() override;
		void rightMouseDown() override;
		void leftMouseUp() override;
		void middleMouseUp() override;
		void rightMouseUp() override;
		void hoverStart() override;
		void hoverEnd() override;

		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		void onSetCullAABB(const AABB<float>& cullAABB) override;

		float textHalfHeight, highlightBonusHalfHeight;

		std::array<unsigned int, 2> highlightPos;
		bool highlightEnabled;
		unsigned int cursorPos;
		SimpleTextRectangle text;
		SimpleRectangle highlightRect;
		Line flickerLine;
	};
}