#pragma once

#include "GUIBase.h"

#include "Texture.h"
#include "SimpleRectangle.h"
#include "InteractionArea.h"

namespace Val {
	class GUI_VerticalScroll : public GUIParentVector {
	public:
		typedef std::shared_ptr<GUI_VerticalScroll> Ptr;
		static GUI_VerticalScroll::Ptr Create(std::array<float, 2> halfExtents, std::array<float, 2> buttonSize, TextureResource Up, TextureResource Down, TextureResource Hover);

		GUI_VerticalScroll(std::array<float, 2> halfExtents, std::array<float, 2> buttonSize, TextureResource Up, TextureResource Down, TextureResource Hover);
		~GUI_VerticalScroll();

		void setExtents(std::array<float, 2> halfExtents);
		std::array<float, 2> getHalfExtents() const;

		void setVerticalPosition(float position);
		float getVerticalPosition() const;
	protected:
		AABB<float> cullAABB;
		//halfSize is actually halfExtents, 
		std::array<float, 2> internalHalfSize;
		float verticalPosition;
		bool internalOverflow;

		TextureResource Hover, Down, Up;
		SimpleRectangle scrollBarButtonRender;
		InteractionArea scrollBarButton;
		bool scrollBarDragging;
		float scrollBarMouseOffset;

		bool mouseWheel(int delta) override;

		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		void onProcessEvents() override;

		bool canAddChild(std::shared_ptr<GUIBase> child);
	};
}