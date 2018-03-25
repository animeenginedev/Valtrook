#pragma once

#include "GUIBase.h"

#include "SimpleRectangle.h"

namespace Val {
	class GUI_Button : public GUIParentSingle {
	public:
		typedef std::shared_ptr<GUI_Button> Ptr;
		static GUI_Button::Ptr Create(const TextureResource& defaultTex, const TextureResource& hoverTex, const TextureResource& downTex);

		GUI_Button(const TextureResource& defaultTex, const TextureResource& hoverTex, const TextureResource& downTex);
		~GUI_Button();

		void setDefaultTexture(const TextureResource& tex);
		void setHoverTexture(const TextureResource& tex);
		void setDownTexture(const TextureResource& tex);

		TextureResource getDefaultTexture() const;
		TextureResource getHoverTexture() const;
		TextureResource getDownTexture() const;

		void setButtonRender(const SimpleRectangle& buttonRender);
		SimpleRectangle getButtonRender() const;
	protected:
		SimpleRectangle buttonRender;
		TextureResource defaultTexture, hoverTexture, downTexture;

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