#include "GUI_Button.h"

#include "RenderingEngine.h"

namespace Val {
	GUI_Button::Ptr GUI_Button::Create(float edgeWidth, Colour edgeDefault, Colour edgeHover, Colour edgeDown, Colour centerDefault, Colour centerHover, Colour centerDown, FrameStyle style) {
		return std::make_shared<GUI_Button>(edgeWidth, edgeDefault, edgeHover, edgeDown, centerDefault, centerHover, centerDown, style);
	}
	GUI_Button::GUI_Button(float edgeWidth, Colour edgeDefault, Colour edgeHover, Colour edgeDown, Colour centerDefault, Colour centerHover, Colour centerDown, FrameStyle style) :
		buttonRender(style, { 0, 0 }, { 1.0f, 1.0f }, edgeWidth),
		
		downEdge(edgeDown), hoverEdge(edgeHover), defaultEdge(edgeDefault), downCenter(centerDown), hoverCenter(centerHover), defaultCenter(centerDefault) {
		buttonRender.setEdgeColour(defaultEdge);
		buttonRender.setCenterColour(defaultCenter);
	}
	GUI_Button::~GUI_Button() {
	}
	FrameRender GUI_Button::getButtonRender() const {
		return buttonRender;
	}
	void GUI_Button::leftMouseDown() {
		buttonRender.setEdgeColour(downEdge);
		buttonRender.setCenterColour(downCenter);
	}
	void GUI_Button::middleMouseDown() {
		buttonRender.setEdgeColour(downEdge);
		buttonRender.setCenterColour(downCenter);
	}
	void GUI_Button::rightMouseDown() {
		buttonRender.setEdgeColour(downEdge);
		buttonRender.setCenterColour(downCenter);
	}
	void GUI_Button::leftMouseUp() {
		buttonRender.setEdgeColour(hoverEdge);
		buttonRender.setCenterColour(hoverCenter);
	}
	void GUI_Button::middleMouseUp() {
		buttonRender.setEdgeColour(hoverEdge);
		buttonRender.setCenterColour(hoverCenter);
	}
	void GUI_Button::rightMouseUp() {
		buttonRender.setEdgeColour(hoverEdge);
		buttonRender.setCenterColour(hoverCenter);
	}
	void GUI_Button::hoverStart() {
		buttonRender.setEdgeColour(hoverEdge);
		buttonRender.setCenterColour(hoverCenter);
	}
	void GUI_Button::hoverEnd() {
		buttonRender.setEdgeColour(defaultEdge);
		buttonRender.setCenterColour(defaultCenter);
	}
	void GUI_Button::internalUpdate(const TimingType & deltaTime) {
	}
	void GUI_Button::internalRender(const TimingType & deltaTime, RenderingEngine * engine) {
		buttonRender.sendRenderInformation(engine->getGUIRenderer());
	}
	void GUI_Button::internalRecalculatePosition() {
		if (child[0] != nullptr) {
			child[0]->setPosition({ 0.0f, 0.0f });
		}
	}
	void GUI_Button::internalRecalculateSize() {
		halfSize = { 0.0f, 0.0f };
		if (child[0] != nullptr) {
			if (!child[0]->isHidden()) {
				halfSize = child[0]->getHalfSize();
			}
		}
	}
	void GUI_Button::onRecalculateComplete() {
		buttonRender.setCenter(getAbsolutePosition());
		buttonRender.setHalfSize(halfSize[0], halfSize[1]);
		buttonRender.setEdgeDepth(getDepth() + 0.01f);
		buttonRender.setCenterDepth(getDepth());
		interactionArea.centerX = buttonRender.getX();
		interactionArea.centerY = buttonRender.getY();
		interactionArea.halfWidth = halfSize[0];
		interactionArea.halfHeight = halfSize[1];

		if (hasCullAABB) {
			interactionArea = interactionArea.getCulledAABB(cullAABB);
		}
	}
	void GUI_Button::onSetCullAABB(const AABB<float>& cullAABB) {
		buttonRender.setCullSurface(cullAABB);
	}
	bool GUI_Button::canAddChild(std::shared_ptr<GUIBase> child) {
		return this->child[0] == nullptr && child != nullptr;
	}
}