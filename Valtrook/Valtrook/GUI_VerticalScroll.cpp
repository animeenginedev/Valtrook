#include "GUI_VerticalScroll.h"

#include "Max.h"
#include "Min.h"
#include "RenderingEngine.h"

namespace Val {
	GUI_VerticalScroll::Ptr GUI_VerticalScroll::Create(std::array<float, 2> halfExtents, std::array<float, 2> buttonSize, TextureResource Up, TextureResource Down, TextureResource Hover) {
		return std::make_shared<GUI_VerticalScroll>(halfExtents, buttonSize, Up, Down, Hover);
	}
	GUI_VerticalScroll::GUI_VerticalScroll(std::array<float, 2> halfExtents, std::array<float, 2> buttonSize, TextureResource Up, TextureResource Down, TextureResource Hover) :
		cullAABB(0.0f, 0.0f, 0.0f, 0.0f), internalOverflow(false), Up(Up), Hover(Hover), Down(Down), scrollBarButton(0.0f, 0.0f, buttonSize[0], buttonSize[1]), scrollBarButtonRender(Up, 0.0f, 0.0f, 0.0f, buttonSize[0], buttonSize[1]), scrollBarDragging(false) {

		halfSize = halfExtents;
		cullAABB.halfWidth = halfSize[0];
		cullAABB.halfHeight = halfSize[1];
	}
	GUI_VerticalScroll::~GUI_VerticalScroll() {
	}
	void GUI_VerticalScroll::setExtents(std::array<float, 2> halfExtents) {
		halfSize = halfExtents;
		cullAABB.halfWidth = halfSize[0];
		cullAABB.halfHeight = halfSize[1];
	}
	std::array<float, 2> GUI_VerticalScroll::getHalfExtents() const {
		return halfSize;
	}
	void GUI_VerticalScroll::setVerticalPosition(float position) {
		this->verticalPosition = position;
		this->needsReconstructed = true;
	}
	float GUI_VerticalScroll::getVerticalPosition() const {
		return verticalPosition;
	}
	bool GUI_VerticalScroll::mouseWheel(int delta) {
		float step = 0.05f;

		verticalPosition += delta < 0 ? step : -step;
		verticalPosition = Min<float>(verticalPosition, 1.0f);
		verticalPosition = Max<float>(verticalPosition, 0.0f);
		needsReconstructed = true;

		return true;
	}
	void GUI_VerticalScroll::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_VerticalScroll::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		if (internalOverflow) {
			scrollBarButtonRender.sendRenderInformation(engine->getGUIRenderer());
		}
	}
	void GUI_VerticalScroll::internalRecalculatePosition() {
		float currentVerticalStep = halfSize[1];

		if (internalOverflow) {
			currentVerticalStep = ((verticalPosition + verticalPosition) * internalHalfSize[1]) + ((1 + (-verticalPosition - verticalPosition)) * halfSize[1]);
		}

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

		if (internalHalfSize[1] > halfSize[1]) {
			if (!internalOverflow) {
				internalOverflow = true;

				verticalPosition = 0.0f;
			}
		} else {
			internalOverflow = false;
			verticalPosition = 0.0f;
		}
	}
	void GUI_VerticalScroll::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		cullAABB.centerX = pos[0];
		cullAABB.centerY = pos[1];
		setCullAABB(cullAABB);

		if (internalOverflow) {
			scrollBarButtonRender.setDepth(getDepth());
			std::array<float, 2> scrollBarOffset = { 0.1f, 0.1f };

			//Width Center+HalfWidth - scrollBarOffset
			//Height Center+HalfHeight + scrollBarOffset + (verticalPosition * (halfSize[1] * 2) - (scrollBarOffset * 2))
			scrollBarButtonRender.setCenter((pos[0] + halfSize[0]) - scrollBarOffset[0],
											(pos[1] + halfSize[1]) - scrollBarOffset[1] - (verticalPosition * ((halfSize[1] + halfSize[1]) - (scrollBarOffset[1] + scrollBarOffset[1]))));
			scrollBarButton.setCenter(scrollBarButtonRender.getCenter());
		}

		interactionArea.centerX = pos[0];
		interactionArea.centerY = pos[1];
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];
	}
	void GUI_VerticalScroll::onProcessEvents() {
		auto mousePos = currentEventData->mouseWorldPosition;
		InteractionState scrollBarState = scrollBarButton.getState(mousePos[0], mousePos[1]);

		bool isMouseDown = (currentEventData->leftMouseDown || currentEventData->rightMouseDown || currentEventData->middleMouseDown);
		bool wasMouseDown = (lastEventData->leftMouseDown || lastEventData->rightMouseDown || lastEventData->middleMouseDown);

		if (!isMouseDown && wasMouseDown) {
			scrollBarDragging = false;
			scrollBarButtonRender.setTexture(Up);
		}
		auto pos = getAbsolutePosition();
		std::array<float, 2> scrollBarOffset = { 0.1f, 0.1f };
		switch (scrollBarState) {
			case IS_Just_Inside:
			case IS_Inside:
				if (isMouseDown && !wasMouseDown) {
					scrollBarDragging = true;
					scrollBarMouseOffset = scrollBarButtonRender.getY() - mousePos[1];
				}
				if (isMouseDown) {
					scrollBarButtonRender.setTexture(Down);
				} else {
					scrollBarButtonRender.setTexture(Hover);
				}
				break;
			case IS_Just_WasInside:
				if (!scrollBarDragging) {
					scrollBarButtonRender.setTexture(Up);
				}
			case IS_Outside:
				break;
		}
		if (scrollBarDragging) {
			verticalPosition = 1.0 - (((mousePos[1] + scrollBarMouseOffset) + pos[1] + halfSize[1]) - scrollBarOffset[1]) / ((halfSize[1] + halfSize[1]) - (scrollBarOffset[1] + scrollBarOffset[1]));
			currentEventData->inputUsed = true;
			this->needsReconstructed = true;
		}
		verticalPosition = Min<float>(verticalPosition, 1.0f);
		verticalPosition = Max<float>(verticalPosition, 0.0f);
	}

	bool GUI_VerticalScroll::canAddChild(std::shared_ptr<GUIBase> child) {
		if (child == nullptr)
			return false;
		return true;
	}
}