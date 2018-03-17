#pragma once

#include "GUIBase.h"

#include "Rectangle.h"

namespace Val {
	class GUI_Image : public GUIBase {
	public:
		typedef std::shared_ptr<GUI_Image> Ptr;
		static GUI_Image::Ptr Create(const SimpleRectangle& rectangle);

		GUI_Image();
		~GUI_Image();

		void initailise(const SimpleRectangle& rectangle);
		
		SimpleRectangle* getImage();
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		SimpleRectangle image;
	};

}