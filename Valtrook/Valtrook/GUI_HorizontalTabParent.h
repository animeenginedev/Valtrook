#pragma once

#include "GUIBase.h"

#include "InteractionArea.h"
#include "CollisionShapePolygon.h"
#include "SimpleRectangle.h"
#include <vector>

namespace Val {
	struct TabStyle {
		static TabStyle getDefaultStyle();
		static TabStyle getDefaultStyleCurved();

		TabStyle(const TextureResource& edgeSide, const TextureResource& edgeTop, const TextureResource& corner, const TextureResource& background);

		TextureResource edgeSide;
		TextureResource edgeTop;
		TextureResource corner;
		TextureResource background;
	};

	struct TabInteractionArea {
		TabStyle style;

		Colour inactiveEdgeColour, activeEdgeColour;
		Colour inactiveBackgroundColour, activeBackgroundColour;
		
		std::weak_ptr<GUIBase> tab;
//		GUI_HorizontalTabParent* tabMaster;
		SimpleRectangle tabRender;
		InteractionArea<CollisionShapePolygon> interactionZone;
	};

	class GUI_HorizontalTabParent : public GUIBase {
	public:
		GUI_HorizontalTabParent();
		~GUI_HorizontalTabParent();

		bool addTab(std::shared_ptr<GUIBase> child);
		bool setTab(unsigned int tab, std::shared_ptr<GUIBase> child);
		std::shared_ptr<GUIBase> getTab(unsigned int tab) const;
		bool hasTab(std::shared_ptr<GUIBase> child) const;
		bool removeTab(unsigned int tab);
		bool removeTab(std::shared_ptr<GUIBase> child);
		unsigned int numberOfTabs() const;
		void clearTabs();
		std::vector<std::shared_ptr<GUIBase>> getTabs() const;

		std::shared_ptr<GUIBase> getActiveTab() const;
		unsigned int getActiveTabID() const;
		bool setActiveTab(unsigned int tab);

		std::shared_ptr<GUIBase> operator[](unsigned int tab) {
			return tabs[tab];
		}

		bool addChild(std::shared_ptr<GUIBase> child) override;
		bool removeChild(std::shared_ptr<GUIBase> child) override;
		void clearChildren() override;
		std::vector<std::shared_ptr<GUIBase>> getChildren() const override;
		bool hasChildren() const override;
		bool isParentTypeGUI() const override;
	protected:
		std::vector<std::shared_ptr<GUIBase>> tabs;
		std::vector<std::shared_ptr<GUIBase>> activeTab;
		unsigned int activeTabID;

		void preRecalculate() override;
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;
	private:
	};
}