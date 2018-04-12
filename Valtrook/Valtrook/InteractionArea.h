#pragma once

#include "CollisionShape2D.h"
#include "AABB.h"
#include <array>
#include <type_traits>
namespace Val {
	enum InteractionState {
		IS_Just_Inside,
		IS_Just_WasInside,

		IS_Inside,
		IS_Outside
	};

	template<class T, typename = std::enable_if_t<std::is_convertible<T, CollisionShape2D>::value>>
	class InteractionArea {
	public:
		InteractionArea(T CollisionShape) : interactionArea(CollisionShape), state(IS_Outside) {}
		~InteractionArea() {}

		void setInteractionArea(T area) { interactionArea = area; };
		T* getInteractionArea() { return &interactionArea; };


		InteractionState getState(float x, float y) const {
			bool isInside = interactionArea.isPointInside(x, y);

			if (state == IS_Just_Inside) {
				if (isInside) {
					state = IS_Inside;
				} else {
					state = IS_Just_WasInside;
				}
			} else if (state == IS_Just_WasInside) {
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
	protected:
		T interactionArea;

		mutable InteractionState state;
	};
}