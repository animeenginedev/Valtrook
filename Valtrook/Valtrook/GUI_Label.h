#pragma once

#include "GUIBase.h"

#include "SimpleMultilineTextRectangle.h"

namespace Val {
	class GUI_Label : public GUIBase {
	public:
		typedef std::shared_ptr<GUI_Label> Ptr;
		static GUI_Label::Ptr Create(const SimpleMultilineTextRectangle& rectangle);

		GUI_Label(const SimpleMultilineTextRectangle& rectangle);
		~GUI_Label();

		void setText(const std::string& text);
		std::string getText() const;
		SimpleMultilineTextRectangle* getTextRect();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		void onSetCullAABB(const AABB<float>& cullAABB) override;

		SimpleMultilineTextRectangle textRect;
	};

}
