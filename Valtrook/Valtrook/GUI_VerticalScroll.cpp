#include "GUI_VerticalScroll.h"

#include "Max.h"

namespace Val {
	GUI_VerticalScroll::Ptr GUI_VerticalScroll::Create() {
		return GUI_VerticalScroll::Ptr();
	}
	GUI_VerticalScroll::GUI_VerticalScroll() : cullAABB(0.0f, 0.0f, 0.0f, 0.0f) {
	}
	GUI_VerticalScroll::~GUI_VerticalScroll() {
	}
	void GUI_VerticalScroll::setExtents(std::array<float, 2> halfExtents) {
		halfSize = halfExtents;
		cullAABB.halfWidth = halfSize[0];
		cullAABB.halfHeight = halfSize[1];
	}
	std::array<float, 2> GUI_VerticalScroll::getHalfExtents() {
		return halfSize;
	}
	void GUI_VerticalScroll::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_VerticalScroll::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_VerticalScroll::internalRecalculatePosition() {
		

		float currentVerticalStep = halfSize[1];
		float calculatedHorizontal = 0.0f;
		std::array<float, 2> childHalfSize;
		
		float largestHalfWidth = 0.0f;
		for (auto child : children) {
			if (child->isHidden())
				continue;

			largestHalfWidth = Max<float>(largestHalfWidth, fabs(child->getHalfSize()[0]));
		}

		for (auto child : children) {
			if (child->isHidden())
				continue;
			childHalfSize = child->getHalfSize();

			currentVerticalStep -= childHalfSize[1];

			calculatedHorizontal = 0.0f;
			if (childHalfSize[0] < largestHalfWidth) {
				switch (child->getHorizontalJustification()) {
					case LEFT:
						calculatedHorizontal = -largestHalfWidth + childHalfSize[0];
						break;
					case hCENTER:
						break;
					case RIGHT:
						calculatedHorizontal = largestHalfWidth - childHalfSize[0];
						break;
				}
			}

			child->setPosition({ calculatedHorizontal , currentVerticalStep });

			currentVerticalStep -= childHalfSize[1];
		}
	}
	void GUI_VerticalScroll::internalRecalculateSize() {
		internalHalfSize[0] = 0;
		internalHalfSize[1] = 0;
		for (auto child : children) {
			if (child->isHidden())
				continue;
			internalHalfSize[0] = Max<float>(fabs(child->getHalfSize()[0]), internalHalfSize[0]);
			internalHalfSize[1] += fabs(child->getHalfSize()[1]);
		}

		if (internalHalfSize[0] > halfSize[0]) {

		}
		if (internalHalfSize[1] > halfSize[1]) {

		}
	}
	void GUI_VerticalScroll::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		cullAABB.centerX = pos[0];
		cullAABB.centerY = pos[1];
		setCullAABB(cullAABB);	
	}
	bool GUI_VerticalScroll::canAddChild(std::shared_ptr<GUIBase> child) {
		if (child == nullptr)
			return false;
		return true;
	}
}