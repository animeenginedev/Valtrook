#pragma once

#include "TimingType.h"

#include <array>

namespace Val {
	class RenderingEngine;
	class GameState;

	class GameObject {
	public:
		GameObject(GameState* state);
		~GameObject();
		
		void create();
		void initialise();
		void destroy();

		virtual void onCreate();
		virtual void onInitialise();
		virtual void update(const TimingType delta);
		virtual void render(const TimingType delta, RenderingEngine* engine);
		virtual void onActive(bool& activeStatus);
		virtual bool onDestroy();
		virtual void onNameSet(std::string& name);

		void setGameObjectName(std::string name);
		std::string getGameObjectName();

		//virtual void onApplicationExit();
		
		bool isActive();
		void setActive(bool active);

		bool canBeReplaced();
		bool isValid();

		GameState* getOwningState();

		void setX(float x);
		void setY(float y);
		void setPosition(float x, float y);
		void setPosition(std::array<float, 2> pos);

		float getX() const;
		float getY() const;
		std::array<float, 2> getPosition() const;
	protected:
		GameState* owningState;
		std::string objectName;
		bool bisValid;
		bool bIsActive;

		std::array<float, 2> position;
	};
}