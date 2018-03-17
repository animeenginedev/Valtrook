#pragma once

#include "GUIBase.h"

namespace Val {
	class GUI_VerticalBox : public GUIParentVector {
	public:
		typedef std::shared_ptr<GUI_VerticalBox> Ptr;
		static GUI_VerticalBox::Ptr Create();

		GUI_VerticalBox();
		~GUI_VerticalBox();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		bool canAddChild(std::shared_ptr<GUIBase> child);
	};

}