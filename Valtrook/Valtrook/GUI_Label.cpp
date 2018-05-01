#include "GUI_Label.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Label::Ptr GUI_Label::Create(const SimpleMultilineTextRectangle & rectangle) {
		return std::make_shared<GUI_Label>(rectangle);
	}
	GUI_Label::GUI_Label(const SimpleMultilineTextRectangle& rectangle) : textRect(rectangle) {
	}
	GUI_Label::~GUI_Label() {
	}
	void GUI_Label::setText(const std::string & text) {
		textRect.setText(text);
		needsReconstructed = true;
	}
	std::string GUI_Label::getText() const {
		return textRect.getText();
	}
	SimpleMultilineTextRectangle * GUI_Label::getTextRect() {
		return &textRect;
	}
	void GUI_Label::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_Label::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		textRect.sendRenderInformation(engine->getGUIRenderer());
	}
	void GUI_Label::internalRecalculatePosition() {
	}
	void GUI_Label::internalRecalculateSize() {
		textRect.reconstruct();

		halfSize = textRect.getHalfSize();
	}
	void GUI_Label::onRecalculateComplete() {
		textRect.setCenter(getAbsolutePosition());
		textRect.setDepth(getDepth());

		interactionArea.centerX = textRect.getX();
		interactionArea.centerY = textRect.getY();
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	void GUI_Label::onSetCullAABB(const AABB<float>& cullAABB) {
		textRect.setCullSurface(cullAABB);
	}
}