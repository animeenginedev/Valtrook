#include "GUIFrame.h"

namespace Val {
	GUIFrame::Ptr GUIFrame::Create(int inputPriority) {
		auto ptr = std::make_shared<GUIFrame>(GUIFrame());
		ptr->initialise(inputPriority);
		return ptr;
	}
	GUIFrame::GUIFrame() {
	}
	GUIFrame::~GUIFrame() {
	}
	void GUIFrame::initialise(int inputPriority) {
		this->inputPriority=inputPriority;
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
	}
	bool GUIFrame::canAddChild(std::shared_ptr<GUIBase> child) {
		return child != nullptr && this->child[0] == nullptr;
	}
}