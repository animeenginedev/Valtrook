#include "GUI_Label.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Label::Ptr GUI_Label::Create(const SimpleTextRectangle & rectangle) {
		auto ptr = std::make_shared<GUI_Label>(GUI_Label());
		ptr->initailise(rectangle);
		return ptr;
	}
	GUI_Label::GUI_Label() {
	}
	GUI_Label::~GUI_Label() {
	}
	void GUI_Label::initailise(const SimpleTextRectangle & rectangle) {
		this->textRect = rectangle;
		needsReconstructed = true;
	}
	void GUI_Label::setText(const std::string & text) {
		textRect.setText(text);
		needsReconstructed = true;
	}
	std::string GUI_Label::getText() const {
		return textRect.getText();
	}
	SimpleTextRectangle * GUI_Label::getTextRect() {
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
	}
}