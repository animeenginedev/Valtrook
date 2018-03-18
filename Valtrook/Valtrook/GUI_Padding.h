#pragma once

#include "GUIBase.h"

#include <array>

namespace Val {
	class GUI_Padding : public GUIParentSingle {
	public:
		typedef std::shared_ptr<GUI_Padding> Ptr;
		static GUI_Padding::Ptr Create(float widthPad, float heightPad);

		GUI_Padding();
		~GUI_Padding();

		void setWidthPadding(const float& widthPad);
		void setHeightPadding(const float& heightPad);
		void setPadding(const std::array<float, 2>& padding);
		float getWidthPadding() const;
		float getHeightPadding() const;
		std::array<float, 2> getPadding() const;
	protected:
		std::array<float, 2> padding;

		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		bool canAddChild(std::shared_ptr<GUIBase> child) override;
	};
}