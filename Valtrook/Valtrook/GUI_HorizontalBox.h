#pragma once

#include "GUIBase.h"

namespace Val {
	class GUI_HorizontalBox : public GUIParentVector {
	public:
		typedef std::shared_ptr<GUI_HorizontalBox> Ptr;
		static GUI_HorizontalBox::Ptr Create();

		GUI_HorizontalBox();
		~GUI_HorizontalBox();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		bool canAddChild(std::shared_ptr<GUIBase> child);
	};

}