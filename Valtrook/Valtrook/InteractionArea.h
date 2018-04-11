#pragma once

#include "AABB.h"
#include <array>

namespace Val {
	enum InteractionState {
		IS_Just_Inside,
		IS_Just_WasInside,

		IS_Inside,
		IS_Outside
	};

	class InteractionArea {
	public:
		InteractionArea(float x, float y, float halfWidth, float halfHeight);
		InteractionArea(AABB<float> interactionArea);
		~InteractionArea();

		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> center);
		void setHalfWidth(float halfWidth);
		void setHalfHeight(float halfHeight);
		void setHalfSize(float halfWidth, float halfHeight);
		void setHalfSize(std::array<float, 2> halfSize);

		InteractionState getState(float x, float y) const;
	protected:
		AABB<float> interactionArea;

		mutable InteractionState state;
	};
}