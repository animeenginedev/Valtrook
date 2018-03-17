#pragma once

#include "GUIBase.h"

#include "TextRectangle.h"

namespace Val {
	class GUI_Label : public GUIBase {
	public:
		typedef std::shared_ptr<GUI_Label> Ptr;
		static GUI_Label::Ptr Create(const SimpleTextRectangle& rectangle);

		GUI_Label();
		~GUI_Label();
		void initailise(const SimpleTextRectangle& rectangle);

		void setText(const std::string& text);
		std::string getText() const;
		SimpleTextRectangle* getTextRect();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		SimpleTextRectangle textRect;
	};

}
