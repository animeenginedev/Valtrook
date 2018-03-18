#pragma once

#include "TimingType.h"
#include <memory>
#include <set>

namespace Val {
	class InputManager;
	class RenderingEngine;
	class GUIFrame;
	struct EventData;
	class OrthographicCamera;

	class GUIWindow {
	public:
		GUIWindow();
		~GUIWindow();

		void intialise(InputManager const * inputBus, OrthographicCamera* camera);

		void setInput(InputManager const * inputManager);
		void setCamera(OrthographicCamera* camera);

		void addFrame(std::shared_ptr<GUIFrame> frame);
		bool removeFrame(std::shared_ptr<GUIFrame> frame);
		bool hasFrame(std::shared_ptr<GUIFrame> frame) const;
		void clearFrames();

		void update(const TimingType& deltaTime);
		void render(const TimingType& deltaTime, RenderingEngine* engine);
	protected:
		InputManager const * inputManager;
		OrthographicCamera* camera;

		EventData *current, *last;
		std::multiset<std::shared_ptr<GUIFrame>> Frames;
	};
}
