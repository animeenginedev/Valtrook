#include "GUIWindow.h"

#include "RenderingEngine.h"
#include "GUIBase.h"
#include "GUIFrame.h"
#include "InputManager.h"
#include "OrthographicCamera.h"

namespace Val {
	GUIWindow::GUIWindow() : current(new EventData()), last(new EventData()), camera(nullptr), inputManager(nullptr) {
	}

	GUIWindow::~GUIWindow() {
	}
	void GUIWindow::intialise(InputManager const * inputBus, OrthographicCamera * camera) {
		inputManager = inputBus;
		this->camera = camera;
	}
	void GUIWindow::setInput(InputManager const * inputManager) {
		this->inputManager = inputManager;
	}
	void GUIWindow::setCamera(OrthographicCamera * camera) {
		this->camera = camera;
	}
	void GUIWindow::addFrame(std::shared_ptr<GUIFrame> frame) {
		Frames.insert(frame);
		frame->updateEventData(current, last);
		frame->addedToWindow();
	}
	bool GUIWindow::removeFrame(std::shared_ptr<GUIFrame> frame) {
		if (hasFrame(frame)) {
			Frames.erase(frame);
			return true;
		}
		return false;
	}
	bool GUIWindow::hasFrame(std::shared_ptr<GUIFrame> frame) const {
		return Frames.find(frame) != Frames.end();
	}
	void GUIWindow::clearFrames() {
		Frames.clear();
	}
	void GUIWindow::update(const TimingType & deltaTime) {
		*last = *current;
		auto mousePos = inputManager->getMouseLocation();
		*current = EventData(inputManager->getLeftMouseState().isKeyPressed(), inputManager->getLeftMouseState().isKeyPressed(), inputManager->getLeftMouseState().isKeyPressed(), Camera::Cast<OrthographicCamera>(camera)->convertScreenToWorld(mousePos[0], mousePos[1]));

		for (auto f : Frames)
			f->update(deltaTime);
	}
	void GUIWindow::render(const TimingType & deltaTime, RenderingEngine * engine) {
		for (auto f : Frames)
			f->render(deltaTime, engine);
	}
}