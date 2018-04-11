#include "GUIFrame.h"

#include "RenderingEngine.h"

namespace Val {
	GUIFrame::Ptr GUIFrame::Create(int inputPriority) {
		return std::make_shared<GUIFrame>(inputPriority);
	}
	GUIFrame::GUIFrame(int inputPriority) : inputPriority(inputPriority) {
	}
	GUIFrame::~GUIFrame() {
	}
	void GUIFrame::setInputPriority(const int & inputPri) {
		inputPriority = inputPri;
	}
	int GUIFrame::getInputPriority() const {
		return inputPriority;
	}
	void GUIFrame::addedToWindow() {
		if (child[0] != nullptr) {
			child[0]->updateEventData(currentEventData, lastEventData);
		}
	}
	void GUIFrame::internalUpdate(const TimingType & deltaTime) {
	}
	void GUIFrame::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUIFrame::internalRecalculatePosition() {
		child[0]->setPosition({ 0.0f, 0.0f });
	}
	void GUIFrame::internalRecalculateSize() {
		if (child[0] != nullptr)
			halfSize = child[0]->getHalfSize();
		else
			halfSize = { 0.0f, 0.0f };
	}
	void GUIFrame::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		interactionArea.centerX = pos[0];
		interactionArea.centerY = pos[1];
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	bool GUIFrame::canAddChild(std::shared_ptr<GUIBase> child) {
		return child != nullptr && this->child[0] == nullptr;
	}

	GUIFrameR::Ptr Val::GUIFrameR::Create(int inputPriority, float edgeWidth, Colour edgeColour, Colour centerColour, FrameStyle style) {
		return std::make_shared<GUIFrameR>(inputPriority, edgeWidth, edgeColour, centerColour, style);
	}

	Val::GUIFrameR::GUIFrameR(int inputPriority, float edgeWidth, Colour edgeColour, Colour centerColour, FrameStyle style) : GUIFrame(inputPriority), frameRender(style, { 0, 0 }, { 0,0 }, edgeWidth, edgeColour, centerColour) {
	}

	GUIFrameR::~GUIFrameR() {
	}
	void Val::GUIFrameR::setFrameStyle(FrameStyle style) {
		frameRender.setFrameStyle(style);
	}
	void Val::GUIFrameR::setFrameColours(Colour edge, Colour center) {
		frameRender.setEdgeColour(edge);
		frameRender.setCenterColour(center);
	}
	Colour Val::GUIFrameR::getEdgeColour() {
		return frameRender.getEdgeColour();
	}
	Colour Val::GUIFrameR::getCenterColour() {
		return frameRender.getCenterColour();
	}
	FrameStyle Val::GUIFrameR::getFrameStyle() {
		return frameRender.getFrameStyle();
	}
	FrameRender * Val::GUIFrameR::getFrameRender() {
		return &frameRender;
	}
	void Val::GUIFrameR::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		frameRender.sendRenderInformation(engine->getGUIRenderer());
	}
	void Val::GUIFrameR::onRecalculateComplete() {
		GUIFrame::onRecalculateComplete();

		frameRender.setCenter(getAbsolutePosition());
		frameRender.setHalfSize(halfSize[0], halfSize[1]);
		frameRender.setEdgeDepth(getDepth() + 0.01f);
		frameRender.setCenterDepth(getDepth());
		frameRender.setCullSurface(cullAABB);
	}
}