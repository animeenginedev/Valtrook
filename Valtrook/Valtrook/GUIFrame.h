#pragma once

#include "GUIBase.h"

namespace Val {
	class GUIFrame : public GUIParentSingle {
	public:
		typedef std::shared_ptr<GUIFrame> Ptr;
		static GUIFrame::Ptr Create(int inputPriority = 0);

		GUIFrame(int inputPriority);
		~GUIFrame();
		
		void setInputPriority(const int& inputPri);
		int getInputPriority() const;

		void addedToWindow();

		bool operator<(const GUIFrame& rhs) {
			return inputPriority < rhs.inputPriority;
		}
	protected:
		int inputPriority;

		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		bool canAddChild(std::shared_ptr<GUIBase> child) override;
	};
}