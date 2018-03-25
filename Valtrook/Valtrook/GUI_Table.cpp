#include "GUI_Table.h"
#include "Max.h"

namespace Val {
	GUI_Table::Ptr GUI_Table::Create(unsigned int width, unsigned int height) {
		return std::make_shared<GUI_Table>(width, height);
	}
	GUI_Table::GUI_Table(unsigned int width, unsigned int height) : width(width), height(height) {
		children.resize(width * height);
		halfWidths.resize(width);
		halfHeights.resize(height);
	}
	GUI_Table::~GUI_Table() {
	}
	bool GUI_Table::addChild(std::shared_ptr<GUIBase> child, unsigned int x, unsigned int y) {
		if (canAddChild(child, x, y)) {
			children[(width * y) + x] = child;
			child->setParent(this);
			return true;
		}
		return false;
	}
	std::shared_ptr<GUIBase> GUI_Table::getChild(unsigned int x, unsigned int y) {
		return children[(width * y) + x];
	}
	bool GUI_Table::removeChild(unsigned int x, unsigned int y) {
		if (hasChild(x, y)) {
			children[(width * y) + x] = nullptr;
			return true;
		}
		children[(width * y) + x] = nullptr;
		return false;
	}
	bool GUI_Table::hasChild(unsigned x, unsigned int y) {
		return children[(width * y) + x] != nullptr;
	}
	void GUI_Table::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_Table::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_Table::internalRecalculatePosition() {
		unsigned int index = 0;
		float currentX = 0.0f;
		float currentY = halfSize[1];
		std::array<float, 2> childPos;
		std::array<float, 2> childHalfSize;
		for (unsigned int y = 0; y < height; ++y) {
			currentX = -halfSize[0];
			currentY -= halfHeights[y];
			for (unsigned int x = 0; x < width; ++x) {

				currentX += halfWidths[x];

				if (children[index] != nullptr) {
					if (!children[index]->isHidden()) {
						childHalfSize = children[index]->getHalfSize();

						childPos = { currentX, currentY };

						if (childHalfSize[0] < halfWidths[x]) {
							switch (children[index]->getHorizontalJustification()) {
								case LEFT:
									childPos[0] -= (halfWidths[x] - childHalfSize[0]);
									break;
								case hCENTER:
									break;
								case RIGHT:
									childPos[0] += (halfWidths[x] - childHalfSize[0]);
									break;
							}
						}
						if (childHalfSize[1] < halfHeights[y]) {
							switch (children[index]->getVerticalJustification()) {
								case TOP:
									childPos[1] += (halfHeights[y] - childHalfSize[1]);
									break;
								case vCENTER:
									break;
								case BOTTOM:
									childPos[1] -= (halfHeights[y] - childHalfSize[1]);
									break;
							}
						}

						children[index]->setPosition(childPos);
					}
				}

				currentX += halfWidths[x];
				++index;
			}
			currentY -= halfHeights[y];
		}
	}
	void GUI_Table::internalRecalculateSize() {
		unsigned int index = 0;
		float largestHalf;
		halfSize[0] = 0;
		halfSize[1] = 0;

		//Two passes might be slower but its less effort to think of an effective algorithm for both in one pass without recording a hundred memes
		for (unsigned int y = 0; y < height; ++y) {
			largestHalf = 0.0f;
			for (unsigned int x = 0; x < width; ++x) {
				if (children[index] != nullptr) {
					if (!children[index]->isHidden()) {
						largestHalf = Max<float>(fabs(children[index]->getHalfSize()[1]), largestHalf);
					}
				}
				++index;
			}
			halfHeights[y] = largestHalf;
			halfSize[1] += largestHalf;
		}

		for (unsigned int x = 0; x < width; ++x) {
			index = x;
			largestHalf = 0.0f;
			for (unsigned int y = 0; y < height; ++y) {
				if (children[index] != nullptr) {
					if (!children[index]->isHidden()) {
						largestHalf = Max<float>(fabs(children[index]->getHalfSize()[0]), largestHalf);
					}
				}
				index += width;
			}
			halfWidths[x] = largestHalf;
			halfSize[0] += largestHalf;
		}
	}
	void GUI_Table::onRecalculateComplete() {
		auto pos = getAbsolutePosition();
		interactionArea.centerX = pos[0];
		interactionArea.centerY = pos[1];
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	bool GUI_Table::canAddChild(std::shared_ptr<GUIBase> child) {
		return false;
	}
	bool GUI_Table::canAddChild(std::shared_ptr<GUIBase> child, unsigned int x, unsigned int y) {
		if (!hasChild(x, y) && child != nullptr)
			return true;
		return false;
	}
}