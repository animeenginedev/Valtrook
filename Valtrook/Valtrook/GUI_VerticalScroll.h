#pragma once

#include "GUIBase.h"
namespace Val {
	class GUI_VerticalScroll : public GUIParentVector {
	public:
		typedef std::shared_ptr<GUI_VerticalScroll> Ptr;
		static GUI_VerticalScroll::Ptr Create();

		GUI_VerticalScroll();
		~GUI_VerticalScroll();

		void setExtents(std::array<float, 2> halfExtents);
		std::array<float, 2> getHalfExtents();
	protected:
		AABB<float> cullAABB;
		//halfSize is actually halfExtents, 
		std::array<float, 2> internalHalfSize;
		float verticalPosition;

		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		bool canAddChild(std::shared_ptr<GUIBase> child);
	};
}