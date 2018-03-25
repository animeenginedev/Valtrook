#include "GUI_Padding.h"

#include "ArrayMath.h"

namespace Val {
	GUI_Padding::Ptr GUI_Padding::Create(float widthPad, float heightPad) {
		return std::make_shared<GUI_Padding>(widthPad, heightPad);
	}
	GUI_Padding::GUI_Padding(float widthPad, float heightPad) : padding({ widthPad, heightPad }) {
	}
	GUI_Padding::~GUI_Padding() {
	}
	void GUI_Padding::setWidthPadding(const float & widthPad) {
		padding[0] = widthPad;
		needsReconstructed = true;
	}
	void GUI_Padding::setHeightPadding(const float & heightPad) {
		padding[1] = heightPad;
		needsReconstructed = true;
	}
	void GUI_Padding::setPadding(const std::array<float, 2>& padding) {
		this->padding = padding;
		needsReconstructed = true;
	}
	float GUI_Padding::getWidthPadding() const {
		return padding[0];
	}
	float GUI_Padding::getHeightPadding() const {
		return padding[1];
	}
	std::array<float, 2> GUI_Padding::getPadding() const {
		return padding;
	}
	void GUI_Padding::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_Padding::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_Padding::internalRecalculatePosition() {
		if (child[0] != nullptr) {
			child[0]->setPosition({ 0.0f, 0.0f });
		}
	}
	void GUI_Padding::internalRecalculateSize() {
		halfSize = padding + padding;
		if (child[0] != nullptr) {
			if(!child[0]->isHidden()) {
				halfSize = halfSize + child[0]->getHalfSize();
			}
		}
	}
	void GUI_Padding::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		interactionArea.centerX = pos[0];
		interactionArea.centerY = pos[1];
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	bool GUI_Padding::canAddChild(std::shared_ptr<GUIBase> child) {
		return child != nullptr;
	}
}