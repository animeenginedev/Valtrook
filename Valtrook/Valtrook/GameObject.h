#pragma once

#include "RegisterToScript.h"
#include "TimingType.h"

#include <array>

namespace Val {
	class RenderingEngine;
	class GameState;

	class GameObject : public RegisterToScript {
	public:
		GameObject();
		~GameObject();

		virtual void initialise();
		virtual void update(const TimingType delta);
		virtual void render(const TimingType delta, RenderingEngine* engine);

		bool isActive();
		void setActive(bool active);

		GameState* getOwningState();

		float getX();
		float getY();
		std::array<float, 2> getPosition();

		void registerToScript(chaiscript::ChaiScript* script) override;
	protected:
		GameState* owningState;
		std::array<float, 2> position;
	};
}