#include "InteractionArea.h"

namespace Val {
	InteractionArea::InteractionArea(float x, float y, float halfWidth, float halfHeight) : interactionArea(x, y, halfWidth, halfHeight), state(IS_Outside) {
	}
	InteractionArea::InteractionArea(AABB<float> interactionArea) : interactionArea(interactionArea), state(IS_Outside) {
	}
	InteractionArea::~InteractionArea() {
	}
	void InteractionArea::setX(float x) {
		interactionArea.centerX = x;
	}
	void InteractionArea::setY(float y) {
		interactionArea.centerY = y;
	}
	void InteractionArea::setCenter(float x, float y) {
		interactionArea.setCenter(x, y);
	}
	void InteractionArea::setCenter(std::array<float, 2> center) {
		interactionArea.setCenter(center[0], center[1]);
	}
	void InteractionArea::setHalfWidth(float halfWidth) {
		interactionArea.halfWidth = halfWidth;
	}
	void InteractionArea::setHalfHeight(float halfHeight) {
		interactionArea.halfHeight = halfHeight;
	}
	void InteractionArea::setHalfSize(float halfWidth, float halfHeight) {
		interactionArea.halfWidth = halfWidth;
		interactionArea.halfHeight = halfHeight;
	}
	void InteractionArea::setHalfSize(std::array<float, 2> halfSize) {
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];
	}
	InteractionState InteractionArea::getState(float x, float y) const {
		bool isInside = interactionArea.containsPoint(x, y);
		
		if (state == IS_Just_Inside) {
			if (isInside) {
				state = IS_Inside;
			} else {
				state = IS_Just_WasInside;
			}
		} else if(state == IS_Just_WasInside) {
			if (isInside) {
				state = IS_Just_Inside;
			} else {
				state = IS_Outside;
			}
		} else if (state == IS_Inside) {
			if (!isInside) {
				state = IS_Just_WasInside;
			}
		} else if (state == IS_Outside) {
			if (isInside) {
				state = IS_Just_Inside;
			}
		} 

		return state;
	}
}