#include "GUI_Image.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Image::Ptr GUI_Image::Create(const SimpleRectangle & rectangle) {
		return std::make_shared<GUI_Image>(rectangle);
	}
	GUI_Image::GUI_Image(const SimpleRectangle& rectangle) : image(rectangle) {
		halfSize = rectangle.getHalfSize();
		needsReconstructed = true;
	}
	GUI_Image::~GUI_Image() {
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

		interactionArea.centerX = image.getX();
		interactionArea.centerY = image.getY();
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	void GUI_Image::onSetCullAABB(const AABB<float>& cullAABB) {
		image.setCullSurface(cullAABB);
	}
}