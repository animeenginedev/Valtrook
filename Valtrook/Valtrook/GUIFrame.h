#pragma once

#include "GUIBase.h"

#include "FrameRender.h"

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

	class GUIFrameR : public GUIFrame {
	public:
		typedef std::shared_ptr<GUIFrameR> Ptr;
		static GUIFrameR::Ptr Create(int inputPriority, float edgeWidth, Colour edgeColour, Colour centerColour, FrameStyle style = FrameStyle::getDefaultCurved());
		
		GUIFrameR(int inputPriority, float edgeWidth, Colour edgeColour,  Colour centerColour, FrameStyle style);
		~GUIFrameR();

		void setFrameStyle(FrameStyle style);
		void setFrameColours(Colour edge, Colour center);

		Colour getEdgeColour();
		Colour getCenterColour();
		FrameStyle getFrameStyle();
		FrameRender* getFrameRender();

		bool operator<(const GUIFrameR& rhs) {
			return inputPriority < rhs.inputPriority;
		}
		bool operator<(const GUIFrame& rhs) {
			return inputPriority < rhs.getInputPriority();
		}
	protected:
		int inputPriority;
		FrameRender frameRender;

		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;

		void onRecalculateComplete() override;
	};
}