#include "GUI_HorizontalBox.h"

#include "Max.h"

namespace Val {
	GUI_HorizontalBox::Ptr GUI_HorizontalBox::Create() {
		return std::make_shared<GUI_HorizontalBox>(GUI_HorizontalBox());
	}
	GUI_HorizontalBox::GUI_HorizontalBox() {
	}
	GUI_HorizontalBox::~GUI_HorizontalBox() {
	}
	void GUI_HorizontalBox::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_HorizontalBox::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_HorizontalBox::internalRecalculatePosition() {
		float currentHorizontalStep = -halfSize[0];
		float calculatedVertical = 0.0f;
		std::array<float, 2> childHalfSize;


		float largestHalfHeight = 0.0f;
		for (auto child : children) {
			if (child->isHidden())
				continue;

			largestHalfHeight = Max<float>(largestHalfHeight, fabs(child->getHalfSize()[1]));
		}

		for (auto child : children) {
			if (child->isHidden())
				continue;
			childHalfSize = child->getHalfSize();

			currentHorizontalStep += childHalfSize[0];

			calculatedVertical = 0.0f;
			if (childHalfSize[1] < largestHalfHeight) {
				switch (child->getVerticalJustification()) {
					case TOP:
						calculatedVertical = largestHalfHeight - childHalfSize[1];
						break;
					case vCENTER:
						break;
					case BOTTOM:
						calculatedVertical = -largestHalfHeight + childHalfSize[1];
						break;
				}
			}

			child->setPosition({ currentHorizontalStep , calculatedVertical });

			currentHorizontalStep += childHalfSize[0];
		}
	}
	void GUI_HorizontalBox::internalRecalculateSize() {
		halfSize[0] = 0;
		halfSize[1] = 0;
		for (auto child : children) {
			if (child->isHidden())
				continue;
			halfSize[0] += fabs(child->getHalfSize()[0]);
			halfSize[1] = Max<float>(fabs(child->getHalfSize()[1]), halfSize[1]);
		}
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];
	}
	void GUI_HorizontalBox::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		interactionArea.centerX = pos[0];
		interactionArea.centerY = pos[1];
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	bool GUI_HorizontalBox::canAddChild(std::shared_ptr<GUIBase> child) {
		if (child == nullptr)
			return false;
		return true;
	}
}