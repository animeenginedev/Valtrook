#pragma once

#include "Conversion.h"
#include "AABB.h"

#include <glm\glm.hpp>
#include <array>

namespace Val {
	__declspec(align(16))
	class OrthographicCamera {
	public:
		OrthographicCamera();
		~OrthographicCamera();

		void initialise(Meter screenWidth, Meter screenHeight);

		void setPosition(Meter x, Meter y);
		void setPositionTarget(Meter x, Meter y);
		void setPositionLerpPercentage(float percentage);

		void setBounds(Meter width, Meter height);
		void setBoundsTarget(Meter width, Meter height);
		void setBoundsLerpPercentage(float percentage);

		void setScale(float scale);
		void setScaleTarget(float scale);
		void setScaleLerpPercentage(float percentage);

		Meter getX();
		Meter getY();
		Meter getWidth();
		Meter getHeight();
		float getScale();

		Meter getTargetX();
		Meter getTargetY();
		Meter getTargetWidth();
		Meter getTargetHeight();
		float getTargetScale();

		std::array<Meter, 2> convertScreenToWorld(Meter screenX, Meter screenY);
		std::array<Meter, 2> convertWorldToScreen(Meter worldX, Meter worldY);

		AABB<float> getCameraBounds(Meter extraPadding);

		glm::mat4 getMatrix();

		void update(float deltaTime);

		void* operator new(size_t i){ return _mm_malloc(i, 16); }
		void operator delete(void* p) { _mm_free(p); }
	private:

		std::array<Meter, 2> position;
		std::array<Meter, 2> targetPosition;
		std::array<Meter, 2> bounds;
		std::array<Meter, 2> targetBounds;

		float scale;
		float scaleTarget;

		float movementLerpPercentage;
		float boundsLerpPercentage;
		float scaleLerpPercentage;

		glm::mat4 cameraMatrix;
		glm::mat4 orthoMatrix;

		bool bNeedsUpdate;

		void updateMatrix();
	};
}