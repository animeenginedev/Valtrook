#include "GUI_VerticalBox.h"

#include "Max.h"

namespace Val {
	GUI_VerticalBox::Ptr GUI_VerticalBox::Create() {
		return std::make_shared<GUI_VerticalBox>(GUI_VerticalBox());
	}
	GUI_VerticalBox::GUI_VerticalBox() {
	}
	GUI_VerticalBox::~GUI_VerticalBox() {
	}
	void GUI_VerticalBox::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_VerticalBox::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_VerticalBox::internalRecalculatePosition() {
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
						calculatedHorizontal =  largestHalfWidth - childHalfSize[0] ;
						break;
				}
			}

			child->setPosition( { calculatedHorizontal , currentVerticalStep });

			currentVerticalStep -= childHalfSize[1];
		}
	}
	void GUI_VerticalBox::internalRecalculateSize() {
		halfSize[0] = 0;
		halfSize[1] = 0;
		for (auto child : children) {
			if(child->isHidden())
				continue;
			halfSize[0] = Max<float>(fabs(child->getHalfSize()[0]), halfSize[0]);
			halfSize[1] += fabs(child->getHalfSize()[1]);
		}
	}
	void GUI_VerticalBox::onRecalculateComplete() {
	}
	bool GUI_VerticalBox::canAddChild(std::shared_ptr<GUIBase> child) {
		if (child == nullptr)
			return false;
		return true;
	}
}