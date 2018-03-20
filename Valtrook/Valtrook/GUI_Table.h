#pragma once

#include "GUIBase.h"

namespace Val {
	class GUI_Table : public GUIParentVector {
	public:
		typedef std::shared_ptr<GUI_Table> Ptr;
		static GUI_Table::Ptr Create(unsigned int width, unsigned int height);

		GUI_Table(unsigned int width, unsigned int height);
		~GUI_Table();

		bool addChild(std::shared_ptr<GUIBase> child, unsigned int x, unsigned int y);
		std::shared_ptr<GUIBase> getChild(unsigned int x, unsigned int y);
		bool removeChild(unsigned int x, unsigned int y);
		bool hasChild(unsigned x, unsigned int y);
	protected:
		void internalUpdate(const TimingType& deltaTime) override;
		void internalRender(const TimingType& deltaTime, RenderingEngine* engine) override;
		void internalRecalculatePosition() override;
		void internalRecalculateSize() override;
		void onRecalculateComplete() override;

		unsigned int width, height;

		std::vector<float> halfHeights, halfWidths;

		bool canAddChild(std::shared_ptr<GUIBase> child);
		bool canAddChild(std::shared_ptr<GUIBase> child, unsigned int x, unsigned int y);
	};

}