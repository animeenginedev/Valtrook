#include "Engine.h"

#include "Notify.h"
#include "StopWatch.h"

namespace Val {
	Engine::Engine() {

		Notify<int> test = Notify<int>(12);
		NotifyDelegate<int> vv = NotifyDelegate<int>();
		test.addReciever(&vv);
		if (vv.hasChanged())
			exit(-2);
		test.set(13);
		if (vv.hasChanged())
			exit(-1);


	}

	Engine::~Engine() {
	}

	void Engine::start() {
		//We don't want some retard to call start from inside its own function just because it's dumb.
		if (running == false) {
			running = true;
			initialise();
			run();
		}
	}

	void Engine::stop() {
		running = false;
	}

	bool Engine::isRunning() const {
		return running;
	}

	void Engine::setTargetFrameRate(unsigned int frameRate) {
		targetFrameRate = frameRate;
		accumlateFrameRate = static_cast<TimingType>(1.0) / static_cast<TimingType>(frameRate);
	}

	void Engine::setTargetUpdateRate(unsigned int updateRate) {
		targetUpdateRate = updateRate;
		accumlateUpdateRate = static_cast<TimingType>(1.0) / static_cast<TimingType>(updateRate);
	}

	unsigned int Engine::getTargetFrameRate() const {
		return targetFrameRate;
	}

	unsigned int Engine::getTargetUpdateRate() const {
		return targetUpdateRate;
	}

	void Engine::initialise() {
		setTargetFrameRate(120);
		setTargetUpdateRate(60);
	}

	void Engine::run() {

		StopWatch updateTimer, renderingTimer;
		updateTimer.update();
		renderingTimer.update();
		TimingType updateAccumlation = TimingType(), renderAccumlation = TimingType();
		while (running) {
			updateTimer.update();
			updateAccumlation += updateTimer.getCurrentDeltaSecond<TimingType>();
			if (updateAccumlation >= accumlateUpdateRate) {
				updateAccumlation -= accumlateUpdateRate;
				//Update Here


			}

			renderingTimer.update();
			renderAccumlation += renderingTimer.getCurrentDeltaSecond<TimingType>();
			if (renderAccumlation >= accumlateFrameRate) {
				renderAccumlation -= accumlateFrameRate;
				//Render Here


			}



		}
	}
}