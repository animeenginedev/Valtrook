#pragma once

#include "GUIBase.h"

#include "SimpleRectangle.h"

namespace Val {
	class GUI_Image : public GUIBase {
	public:
		typedef std::shared_ptr<GUI_Image> Ptr;
		static GUI_Image::Ptr Create(const SimpleRectangle& rectangle);

		GUI_Image(const SimpleRectangle& rectangle);
		~GUI_Image();
		
		SimpleRectangle* getImage();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		void onSetCullAABB(const AABB<float>& cullAABB) override;

		SimpleRectangle image;
	};

}