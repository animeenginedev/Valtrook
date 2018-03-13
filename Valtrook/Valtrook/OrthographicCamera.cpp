#include "OrthographicCamera.h"

#include "Conversion.h"
#include "Lerp.h"
#include <algorithm>
#include <glm/gtc/matrix_transform.hpp>

namespace Val {
	OrthographicCamera::OrthographicCamera() {
	}

	OrthographicCamera::~OrthographicCamera() {
	}

	void OrthographicCamera::initialise(Meter screenWidth, Meter screenHeight) {
		bNeedsUpdate = true;

		position = { 0, 0 };
		bounds = { screenWidth, screenHeight };
		scale = 1.0f;
		targetPosition = position;
		targetBounds = bounds;
		scaleTarget = scale;

		movementLerpPercentage = 1.0f;
		boundsLerpPercentage = 1.0f;
		scaleLerpPercentage = 1.0f;
	}

	void OrthographicCamera::setPosition(Meter x, Meter y) {
		position = { x, y };
		targetPosition = position;
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setPositionTarget(Meter x, Meter y) {
		targetPosition = { x, y };
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setPositionLerpPercentage(float percentage) {
		movementLerpPercentage = percentage;
	}

	void OrthographicCamera::setBounds(Meter width, Meter height) {
		bounds = { width, height };
		targetBounds = bounds;
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setBoundsTarget(Meter width, Meter height) {
		targetBounds = { width, height };
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setBoundsLerpPercentage(float percentage) {
		boundsLerpPercentage = percentage;
	}

	void OrthographicCamera::setScale(float scale) {
		this->scale = scale;
		this->scaleTarget = this->scale;
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setScaleTarget(float scale) {
		scaleTarget = scale;
		bNeedsUpdate = true;
	}

	void OrthographicCamera::setScaleLerpPercentage(float percentage) {
		scaleLerpPercentage = percentage;
	}

	Meter OrthographicCamera::getX() {
		return position[0];
	}

	Meter OrthographicCamera::getY() {
		return position[1];
	}

	Meter OrthographicCamera::getWidth() {
		return bounds[0];
	}

	Meter OrthographicCamera::getHeight() {
		return bounds[1];
	}

	float OrthographicCamera::getScale() {
		return scale;
	}

	Meter OrthographicCamera::getTargetX() {
		return targetPosition[0];
	}

	Meter OrthographicCamera::getTargetY() {
		return targetPosition[1];
	}

	Meter OrthographicCamera::getTargetWidth() {
		return targetBounds[0];
	}

	Meter OrthographicCamera::getTargetHeight() {
		return targetBounds[1];
	}

	float OrthographicCamera::getTargetScale() {
		return scaleTarget;
	}

	std::array<Meter, 2> OrthographicCamera::convertScreenToWorld(Pixel screenX, Pixel screenY) {
		std::array<Meter, 2> ret = {};
		ret[0] = ((PixelToWorld<Pixel, Meter>(screenX) - (bounds[0] / 2.0f)) / scale) + position[0];
		ret[1] = -(((PixelToWorld<Pixel, Meter>(screenY) - (bounds[1] / 2.0f)) / scale) - position[1]);
		return ret;
	}

	std::array<Meter, 2> OrthographicCamera::convertWorldToScreen(Meter worldX, Meter worldY) {
		std::array<Meter, 2> ret = {};
		ret[0] = ((worldX - position[0]) * scale) + (bounds[0] / 2.0f);
		ret[1] = ((worldY - position[1]) * scale) + (bounds[1] / 2.0f);
		return ret;
	}

	AABB<float> OrthographicCamera::getCameraBounds(Meter extraPadding) {
		return AABB<float>(position[0], position[1], bounds[0] + extraPadding, bounds[1] + extraPadding);
	}

	glm::mat4 OrthographicCamera::getMatrix() {
		if (bNeedsUpdate)
			updateMatrix();
		return cameraMatrix;
	}

	void OrthographicCamera::update(TimingType deltaTime) {
		if (!std::equal(position.begin(), position.end(), targetPosition.begin())) {
			position[0] = lerp<float>(position[0], targetPosition[0], movementLerpPercentage);
			position[1] = lerp<float>(position[1], targetPosition[1], movementLerpPercentage);
			bNeedsUpdate = true;
		}
		if (!std::equal(bounds.begin(), bounds.end(), targetBounds.begin())) {
			bounds[0] = lerp<float>(bounds[0], targetBounds[0], boundsLerpPercentage);
			bounds[1] = lerp<float>(bounds[1], targetBounds[1], boundsLerpPercentage);
			bNeedsUpdate = true;
		}
		if (scale != scaleTarget) {
			scale = lerp<float>(scale, scaleTarget, scaleLerpPercentage);
		}
	}

	void OrthographicCamera::updateMatrix() {
		int xPixel = WorldToPixel<int, float>(position[0]);
		int yPixel = WorldToPixel<int, float>(position[1]);
		int widthPixel = WorldToPixel<int, float>(bounds[0]);
		int heightPixel = WorldToPixel<int, float>(bounds[1]);

		orthoMatrix = glm::ortho(0.0f, static_cast<float>(widthPixel), 0.0f, static_cast<float>(heightPixel), -1.0f, 1.0f);

		cameraMatrix = glm::translate(orthoMatrix, glm::vec3(-xPixel + widthPixel / 2, -yPixel + heightPixel / 2, 0.0f));
		glm::vec3 scaleV3 = glm::vec3(scale, scale, 1.0f);
		cameraMatrix = glm::scale(glm::mat4(1.0f), scaleV3) * cameraMatrix;

		bNeedsUpdate = false;
	}
};