#include "ARect.h"

namespace Val {
	ARect::ARect(std::array<float, 2> center, std::array<float, 2> halfSize, float depth) : center(center), depth(depth), halfSize(halfSize), needsReconstructed(true) {
	}
	ARect::~ARect() {
	}
	void ARect::setX(float x) {
		center[0] = x;
		needsReconstructed = true;
	}
	void ARect::setY(float y) {
		center[1] = y;
		needsReconstructed = true;
	}
	void ARect::setCenter(float x, float y) {
		center[0] = x;
		center[1] = y;
		needsReconstructed = true;
	}
	void ARect::setCenter(std::array<float, 2> center) {
		this->center = center;
		needsReconstructed = true;
	}
	void ARect::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	float ARect::getX() const {
		return center[0];
	}
	float ARect::getY() const {
		return center[1];
	}
	std::array<float, 2> ARect::getCenter() const {
		return center;
	}
	float ARect::getDepth() const {
		return depth;
	}
	float ARect::getHalfWidth() const {
		return halfSize[0];
	}
	float ARect::getHalfHeight() const {
		return halfSize[1];
	}
	std::array<float, 2> ARect::getHalfSize() const {
		return halfSize;
	}
}