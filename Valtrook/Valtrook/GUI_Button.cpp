#include "GUI_Button.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Button::Ptr GUI_Button::Create(const TextureResource & defaultTex, const TextureResource & hoverTex, const TextureResource & downTex) {
		return std::make_shared<GUI_Button>(defaultTex, hoverTex, downTex);
	}
	GUI_Button::GUI_Button(const TextureResource& defaultTex, const TextureResource& hoverTex, const TextureResource& downTex) : buttonRender(defaultTex), defaultTexture(defaultTex), hoverTexture(hoverTex), downTexture(downTex) {
	}
	GUI_Button::~GUI_Button() {
	}
	void GUI_Button::setDefaultTexture(const TextureResource & tex) {
		defaultTexture = tex;
		this->needsReconstructed = true;
	}
	void GUI_Button::setHoverTexture(const TextureResource & tex) {
		hoverTexture = tex;
		this->needsReconstructed = true;
	}
	void GUI_Button::setDownTexture(const TextureResource & tex) {
		downTexture = tex;
		this->needsReconstructed = true;
	}
	TextureResource GUI_Button::getDefaultTexture() const {
		return defaultTexture;
	}
	TextureResource GUI_Button::getHoverTexture() const {
		return hoverTexture;
	}
	TextureResource GUI_Button::getDownTexture() const {
		return downTexture;
	}
	void GUI_Button::setButtonRender(const SimpleRectangle & buttonRender) {
		this->buttonRender = buttonRender;
		this->needsReconstructed = true;
	}
	SimpleRectangle GUI_Button::getButtonRender() const {
		return buttonRender;
	}
	void GUI_Button::leftMouseDown() {
		buttonRender.setTexture(downTexture);
	}
	void GUI_Button::middleMouseDown() {
		buttonRender.setTexture(downTexture);
	}
	void GUI_Button::rightMouseDown() {
		buttonRender.setTexture(downTexture);
	}
	void GUI_Button::leftMouseUp() {
		buttonRender.setTexture(hoverTexture);
	}
	void GUI_Button::middleMouseUp() {
		buttonRender.setTexture(hoverTexture);
	}
	void GUI_Button::rightMouseUp() {
		buttonRender.setTexture(hoverTexture);
	}
	void GUI_Button::hoverStart() {
		buttonRender.setTexture(hoverTexture);
	}
	void GUI_Button::hoverEnd() {
		buttonRender.setTexture(defaultTexture);
	}
	void GUI_Button::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_Button::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		buttonRender.sendRenderInformation(engine->getGUIRenderer());
	}
	void GUI_Button::internalRecalculatePosition() {
		if (child[0] != nullptr) {
			child[0]->setPosition({ 0.0f, 0.0f });
		}
	}
	void GUI_Button::internalRecalculateSize() {
		if (child[0] != nullptr) {
			if (!child[0]->isHidden()) {
				halfSize = child[0]->getHalfSize();
				return;
			}
		}
		halfSize = { 0.0f, 0.0f };
	}
	void GUI_Button::onRecalculateComplete() {
		buttonRender.setCenter(getAbsolutePosition());
		buttonRender.setHalfSize(halfSize[0], halfSize[1]);
		buttonRender.setDepth(getDepth());
	}
	bool GUI_Button::canAddChild(std::shared_ptr<GUIBase> child) {
		return this->child[0] == nullptr && child != nullptr;
	}
}