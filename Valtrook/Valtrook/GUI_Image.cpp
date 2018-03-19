#include "GUI_Image.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Image::Ptr GUI_Image::Create(const SimpleRectangle & rectangle) {
		auto ptr = std::make_shared<GUI_Image>(GUI_Image());
		ptr->initailise(rectangle);
		return ptr;
	}
	GUI_Image::GUI_Image() {
	}
	GUI_Image::~GUI_Image() {
	}
	void GUI_Image::initailise(const SimpleRectangle & rectangle) {
		this->image = rectangle;
		halfSize = rectangle.getHalfSize();
		needsReconstructed = true;
	}
	SimpleRectangle * GUI_Image::getImage() {
		return &image;
	}
	void GUI_Image::internalUpdate(const TimingType & deltaTime) {
		//We don't care about our internal update
	}
	void GUI_Image::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		image.sendRenderInformation(engine->getGUIRenderer());
	}
	void GUI_Image::internalRecalculatePosition() {
		//We don't have children so we don't care
	}
	void GUI_Image::internalRecalculateSize() {
		this->halfSize = image.getHalfSize();
	}
	void GUI_Image::onRecalculateComplete() {
		image.setCenter(getAbsolutePosition());
		image.setDepth(getDepth());
	}
}