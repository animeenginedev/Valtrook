#pragma once

#include "TimingType.h"

#include <glm\glm.hpp>

namespace Val {
	enum CameraType {
		Invalid,
		Orthographic
	};

	class Camera {
	public:
		virtual glm::mat4 getMatrix() = 0;
		virtual void update(TimingType deltaTime) = 0;

		virtual CameraType getType() const { return Invalid; };
		template<typename T> 
		static T* Cast(Camera* tt) {
			return ((T*) tt);
		}
	};
}