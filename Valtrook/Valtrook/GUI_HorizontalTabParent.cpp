#include "GUI_HorizontalTabParent.h"

#include "TextureAsset.h"
#include <array>

namespace Val {

	GUI_HorizontalTabParent::GUI_HorizontalTabParent() : activeTab(1), activeTabID(0) {
	}


	GUI_HorizontalTabParent::~GUI_HorizontalTabParent() {
	}
	bool GUI_HorizontalTabParent::addTab(std::shared_ptr<GUIBase> child) {
		needsReconstructed = true;
		tabs.push_back(child);
		child->setParent(this);
		child->updateEventData(currentEventData, lastEventData);
		child->invalidate();

		return true;
	}
	bool GUI_HorizontalTabParent::setTab(unsigned int tab, std::shared_ptr<GUIBase> child) {
		if (tab < tabs.size()) {
			needsReconstructed = true;
			tabs[tab]->setParent(nullptr);
			tabs[tab] = child;
			child->setParent(this);
			child->updateEventData(currentEventData, lastEventData);
			child->invalidate();

			return true;
		}
		return false;
	}
	std::shared_ptr<GUIBase> GUI_HorizontalTabParent::getTab(unsigned int tab) const {
		return tabs[tab];
	}
	bool GUI_HorizontalTabParent::hasTab(std::shared_ptr<GUIBase> child) const {
		for (const auto& tab : tabs) {
			if (tab == child)
				return true;
		}
		return false;
	}
	bool GUI_HorizontalTabParent::removeTab(unsigned int tab) {
		if (tab < tabs.size()) {
			if (activeTabID == tab) {
				activeTabID--;
				activeTab[0] = nullptr;
			}
			tabs[tab]->setParent(nullptr);
			tabs.erase(tabs.begin() + tab);
			needsReconstructed = true;
			return true;
		}
		return false;
	}
	bool GUI_HorizontalTabParent::removeTab(std::shared_ptr<GUIBase> child) {
		unsigned int counter = 0;
		for (auto tab = tabs.begin(); tab != tabs.end(); tab++) {
			if (*tab == child) {
				if (counter == activeTabID) {
					activeTabID--;
					activeTab[0] = nullptr;
				}
				child->setParent(nullptr);
				tabs.erase(tab);
				needsReconstructed = true;
				return true;
			}
			counter++;
		}
		return false;
	}
	unsigned int GUI_HorizontalTabParent::numberOfTabs() const {
		return tabs.size();
	}
	void GUI_HorizontalTabParent::clearTabs() {
		tabs.clear();
		activeTab[0] = nullptr;
		activeTabID = 0;
		needsReconstructed = true;
	}
	std::vector<std::shared_ptr<GUIBase>> GUI_HorizontalTabParent::getTabs() const {
		return tabs;
	}
	std::shared_ptr<GUIBase> GUI_HorizontalTabParent::getActiveTab() const {
		return activeTab[0];
	}
	unsigned int GUI_HorizontalTabParent::getActiveTabID() const {
		return activeTabID;
	}
	bool GUI_HorizontalTabParent::setActiveTab(unsigned int tab) {
		if (tab < tabs.size()) {
			activeTab[0] = tabs[tab];
			activeTab[0]->invalidate();
			needsReconstructed = true;
			return true;
		}
		return false;
	}
	bool GUI_HorizontalTabParent::addChild(std::shared_ptr<GUIBase> child) {
		return addTab(child);
	}
	bool GUI_HorizontalTabParent::removeChild(std::shared_ptr<GUIBase> child) {
		return removeTab(child);
	}
	void GUI_HorizontalTabParent::clearChildren() {
		tabs.clear();
		activeTabID = 0;
	}
	std::vector<std::shared_ptr<GUIBase>> GUI_HorizontalTabParent::getChildren() const {
		return activeTab;
	}
	bool GUI_HorizontalTabParent::hasChildren() const {
		return tabs.size() != 0;
	}
	bool GUI_HorizontalTabParent::isParentTypeGUI() const {
		return true;
	}
	void GUI_HorizontalTabParent::preRecalculate() {
		if (tabs.size() == 0) {
			activeTabID = 0;
			activeTab[0] = nullptr;
		} else {
			activeTabID = Max<unsigned int>(activeTabID, tabs.size() - 1);
			if (activeTab[0] != tabs[activeTabID]) {
				tabs[activeTabID]->invalidate();
			}
			activeTab[0] = tabs[activeTabID];
		}
	}
	void GUI_HorizontalTabParent::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_HorizontalTabParent::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
	}
	void GUI_HorizontalTabParent::internalRecalculatePosition() {
	}
	void GUI_HorizontalTabParent::internalRecalculateSize() {
	}
	void GUI_HorizontalTabParent::onRecalculateComplete() {
	}
	TabStyle TabStyle::getDefaultStyle() {
		return TabStyle(TextureAsset::getTexture("gui/frameEdgeSide", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameEdgeTop", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameCorner", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameCenter", ".png", false, GL_REPEAT));
	}
	TabStyle TabStyle::getDefaultStyleCurved() {
		return TabStyle(TextureAsset::getTexture("gui/frameEdgeSide", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameEdgeTop", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameCornerCurve", ".png", false, GL_REPEAT),
						TextureAsset::getTexture("gui/frameCenter", ".png", false, GL_REPEAT));
	}
	TabStyle::TabStyle(const TextureResource & edgeSide, const TextureResource & edgeTop, const TextureResource & corner, const TextureResource & background) : edgeSide(edgeSide), edgeTop(edgeTop), corner(corner), background(background) {
	}
}