#pragma once

#include "GUIBase.h"

#include "FrameRender.h"

namespace Val {
	class GUI_Button : public GUIParentSingle {
	public:
		typedef std::shared_ptr<GUI_Button> Ptr;
		static GUI_Button::Ptr Create(float edgeWidth, Colour edgeDefault, Colour edgeHover, Colour edgeDown, Colour centerDefault, Colour centerHover, Colour centerDown, FrameStyle style = FrameStyle::getDefaultCurved());

		GUI_Button(float edgeWidth, Colour edgeDefault, Colour edgeHover, Colour edgeDown, Colour centerDefault, Colour centerHover, Colour centerDown, FrameStyle style);
		~GUI_Button();

		FrameRender getButtonRender() const;
	protected:
		FrameRender buttonRender;
		Colour defaultEdge, defaultCenter;
		Colour hoverEdge, hoverCenter;
		Colour downEdge, downCenter;

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

		bool canAddChild(std::shared_ptr<GUIBase> child) override;
	};
}