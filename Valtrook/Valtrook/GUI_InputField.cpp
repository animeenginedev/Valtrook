#include "GUI_InputField.h"

namespace Val {
	GUI_InputField::Ptr GUI_InputField::Create(TextResource textRes, float textHalfHeight, Colour textColour, float highlightBonusHalfHeight, Colour highlightColour, Colour flickerColour) {
		return std::make_shared<GUI_InputField>(textRes, textHalfHeight, textColour, highlightBonusHalfHeight, highlightColour, flickerColour);
	}

	GUI_InputField::GUI_InputField(TextResource textRes, float textHalfHeight, Colour textColour, float highlightBonusHalfHeight, Colour highlightColour, Colour flickerColour) : 
		textHalfHeight(textHalfHeight), highlightBonusHalfHeight(highlightBonusHalfHeight), highlightPos({ 0, 0 }), highlightEnabled(true), cursorPos(textRes.getTextString().size()), 
		text(textRes, 0, 0, 0.0f, 0.0f, 0.0f, textColour), highlightRect(TextureAsset::getTexture("whiteTexture"),0.0f, 0.0f, 0.0f, 0.0f, 0.0f, highlightColour), 
		flickerLine(std::vector<std::array<float, 2>>(),0, 0, 0, flickerColour) {

		flickerLine.add({ 0.0f, textHalfHeight + highlightBonusHalfHeight });
		flickerLine.add({ 0.0f, -(textHalfHeight + highlightBonusHalfHeight) });
	}

	GUI_InputField::~GUI_InputField() {
	}
	void GUI_InputField::setText(const std::string & str) {
		text.setText(str);
		this->needsReconstructed = true;
	}
	void GUI_InputField::setCursorPosition(unsigned int position) {
		cursorPos = position;
		this->needsReconstructed = true;
	}
	void GUI_InputField::setHighlightPositions(unsigned int start, unsigned int end) {
		highlightPos[0] = start;
		highlightPos[1] = end;
		this->needsReconstructed = true;
	}
	void GUI_InputField::setTextColour(Colour c) {
		text.setColour(c);
	}
	void GUI_InputField::setHighlightColour(Colour c) {
		highlightRect.setColour(c);
	}
	void GUI_InputField::setLineFlickerColour(Colour c) {
		flickerLine.setColour(c);
	}
	std::string GUI_InputField::getText() const {
		return text.getText();
	}
	std::string GUI_InputField::getHighlightedText() const {
		if (highlightPos[0] == 0 && highlightPos[1] == 0)
			return "";
		auto str = text.getText();
		return str.substr(highlightPos[0], highlightPos[1] - highlightPos[0]);
	}
	unsigned int GUI_InputField::getCursorPosition() const {
		return cursorPos;
	}
	std::array<unsigned int, 2> GUI_InputField::getHighlightPositions() const {
		return highlightPos;
	}
	Colour GUI_InputField::getTextColour() const {
		return text.getColour();
	}
	Colour GUI_InputField::getHighlightColour() const {
		return highlightRect.getColour();
	}
	Colour GUI_InputField::getLineFlickerColour() const {
		return flickerLine.getColour();
	}
	void GUI_InputField::setHighlightEnabled(bool enableHighlight) {
		if (!highlightEnabled && enableHighlight) {
			highlightPos = { 0, 0 };
		}
		this->highlightEnabled = enableHighlight;
	}
	bool GUI_InputField::isHighlightEnabled() const {
		return highlightEnabled;
	}
	void Val::GUI_InputField::leftMouseDown() {
	}
	void Val::GUI_InputField::middleMouseDown() {
	}
	void Val::GUI_InputField::rightMouseDown() {
	}
	void Val::GUI_InputField::leftMouseUp() {
	}
	void Val::GUI_InputField::middleMouseUp() {
	}
	void Val::GUI_InputField::rightMouseUp() {
	}
	void Val::GUI_InputField::hoverStart() {
	}
	void Val::GUI_InputField::hoverEnd() {
	}
	void Val::GUI_InputField::internalUpdate(const TimingType & deltaTime) {
	}
	void Val::GUI_InputField::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void Val::GUI_InputField::internalRecalculatePosition() {
	}
	void Val::GUI_InputField::internalRecalculateSize() {
	}
	void Val::GUI_InputField::onRecalculateComplete() {
	}
	void Val::GUI_InputField::onSetCullAABB(const AABB<float>& cullAABB) {
		text.setCullSurface(cullAABB);
		highlightRect.setCullSurface(cullAABB);
		flickerLine.setCullSurface(cullAABB);
	}
};