#include "GUIBase.h"

#include "ArrayMath.h"

namespace Val {
	const constexpr float DepthAdvancement() {
		return 0.02f;
	}

	GUIBase::GUIBase() : eventCallbacks({ nullptr }), lastEventData(nullptr), currentEventData(nullptr), horizontal(hCENTER), vertical(vCENTER), parent(nullptr), position({ 0.0f, 0.0f }), depth(0.0f), bHidden(false),
		bJustHidden(false), halfSize({ 0.0f, 0.0f }), bRecievesInputs(true), needsReconstructed(true) {
	}
	GUIBase::~GUIBase() {
	}
	void GUIBase::reCalculate() {
		//Size then position
		if (bHidden) {
			return;
		}
		//If we're OKAY we don't need to recalculate; remeber to check children aswell.
		if (! this->needsRecalculated())
			return;

		//All our childs sizes, and their childrens offsets
		if (isParentTypeGUI()) {
			for (auto child : getChildren()) {
				if (child != nullptr)
					child->reCalculate();
			}
		}
		//Our size based on our children
		internalRecalculateSize();

		//Place out children at appropate places
		internalRecalculatePosition();

		if (hasParent()) {
			depth = parent->getDepth() + DepthAdvancement();
		}

		recalculateComplete();

		needsReconstructed = false;
	}
	bool GUIBase::needsRecalculated() const {
		if (bJustHidden) {
			bJustHidden = false;
			return true;
		}
		if (isParentTypeGUI()) {
			for (auto child : getChildren()) {
				if (child != nullptr)
					if (child->needsRecalculated()) {
						return true;
				}
			}
		}

		return needsReconstructed;
	}
	void GUIBase::update(const TimingType & deltaTime) {
		//Only recalculate on update, since we should only process events there.
		if (bHidden)
			return;

		reCalculate();

		if (isParentTypeGUI()) {
			for (auto child : getChildren()) {
				if (child != nullptr)
					if (!child->isHidden())
						child->update(deltaTime);
			}
		}

		processEvents();

		internalUpdate(deltaTime);
	}
	void GUIBase::render(const TimingType & deltaTime, RenderingEngine * engine) {
		if (bHidden)
			return;
		if (isParentTypeGUI()) {
			for (auto child : getChildren()) {
				if(child != nullptr)
					if (!child->isHidden())
						child->render(deltaTime, engine);
			}
		}

		internalRender(deltaTime, engine);
	}
	std::array<float, 2> GUIBase::getPosition() const {
		return position;
	}
	std::array<float, 2> GUIBase::getHalfSize() const {
		return halfSize;
	}
	std::array<float, 2> GUIBase::getAbsolutePosition() const {
		if (hasParent()) {
			return position + parent->getAbsolutePosition();
		}
		return position;
	}
	float GUIBase::getDepth() const {
		return depth;
	}
	bool GUIBase::hasParent() const {
		return parent != nullptr;
	}
	GUIBase * GUIBase::getParent() const {
		return parent;
	}
	void GUIBase::setHorizontalJustification(const HorizontalJustification & horizontal) {
		this->horizontal = horizontal;
		needsReconstructed = true;
	}
	void GUIBase::setVerticalJustification(const VerticalJustification & vertical) {
		this->vertical = vertical;
		needsReconstructed = true;
	}
	void GUIBase::setJustification(const HorizontalJustification & horizontal, const VerticalJustification & vertical) {
		this->horizontal = horizontal;
		this->vertical = vertical;
		needsReconstructed = true;
	}
	HorizontalJustification GUIBase::getHorizontalJustification() const {
		return horizontal;
	}
	VerticalJustification GUIBase::getVerticalJustification() const {
		return vertical;
	}
	std::pair<HorizontalJustification, VerticalJustification> GUIBase::getJustification() const {
		return std::make_pair(horizontal, vertical);
	}
	void GUIBase::setRecievesInputs(const bool & bInput) {
		bRecievesInputs = bInput;
		needsReconstructed = true;
	}
	bool GUIBase::recievesInputs() const {
		return bRecievesInputs;
	}

	bool GUIBase::addChild(std::shared_ptr<GUIBase> child) {
		return false;
	}

	bool GUIBase::removeChild(std::shared_ptr<GUIBase> child) {
		return false;
	}

	void GUIBase::clearChildren() {
	}

	std::vector<std::shared_ptr<GUIBase>> GUIBase::getChildren() const {
		return std::vector<std::shared_ptr<GUIBase>>();
	}
	
	bool GUIBase::hasChildren() const {
		return false;
	}

	bool GUIBase::isParentTypeGUI() const {
		return false;
	}

	void GUIBase::setPosition(const std::array<float, 2>& position) {
		this->position = position;
	}

	void GUIBase::setHidden(const bool & hide) {
		bHidden = hide;
		bJustHidden = true;
		needsReconstructed = true;
	}

	bool GUIBase::isHidden() const {
		return bHidden;
	}

	void GUIBase::setEventCallback(std::function<void()> callback, GUIEventType eventType) {
		eventCallbacks[eventType] = callback;
	}

	void GUIBase::removeEventCallback(GUIEventType eventType) {
		eventCallbacks[eventType] = nullptr;
	}

	std::function<void()> GUIBase::getEventCallback(GUIEventType eventType) const {
		return eventCallbacks[eventType];
	}

	void GUIBase::clearEventCallbacks() {
		for (unsigned int i = 0; i < GUIEventCount(); ++i) {
			eventCallbacks[i] = nullptr;
		}
	}

	void GUIBase::setUsesInput(const bool & usesInput) {
		bUsesInput = true;
	}

	bool GUIBase::usesInput() const {
		return bUsesInput;
	}

	void GUIBase::updateEventData(EventData * current, EventData * last) {
		currentEventData = current;
		lastEventData = last;

		for (auto child : getChildren()) {
			if (child != nullptr)
				child->updateEventData(current, last);
		}
	}

	void GUIBase::processEvents() {
		if (!bRecievesInputs)
			return;
		if (lastEventData == nullptr || currentEventData == nullptr)
			return;

		EventData currentEventData = *(this->currentEventData);
		EventData lastEventData = *(this->lastEventData);

		bool leftJustDown = currentEventData.leftMouseDown && !lastEventData.leftMouseDown;
		bool middleJustDown = currentEventData.middleMouseDown && !lastEventData.middleMouseDown;
		bool rightJustDown = currentEventData.rightMouseDown && !lastEventData.rightMouseDown;
		bool leftJustUp = !currentEventData.leftMouseDown && lastEventData.leftMouseDown;
		bool middleJustUp = !currentEventData.middleMouseDown && lastEventData.middleMouseDown;
		bool rightJustUp = !currentEventData.rightMouseDown && lastEventData.rightMouseDown;

		std::array<float, 2> UpperLeft = getAbsolutePosition() - halfSize;
		std::array<float, 2> LowerRight = UpperLeft + halfSize + halfSize;
		std::array<float, 2> MouseLast = lastEventData.mouseWorldPosition;
		std::array<float, 2> MouseNow = currentEventData.mouseWorldPosition;

		bool isInside = (!(MouseNow[0] > LowerRight[0] ||
						   MouseNow[0] < UpperLeft[0] ||
						   MouseNow[1] < UpperLeft[1] ||
						   MouseNow[1] > LowerRight[1]));
		bool wasInside = (!(MouseLast[0] > LowerRight[0] ||
							MouseLast[0] < UpperLeft[0] ||
							MouseLast[1] < UpperLeft[1] ||
							MouseLast[1] > LowerRight[1]));

		if (isInside && !wasInside) {
			if (eventCallbacks[Hover_Enter].operator bool()) {
				eventCallbacks[Hover_Enter]();
			}
		} else if (wasInside && !isInside) {
			if (eventCallbacks[Hover_Exit].operator bool()) {
				eventCallbacks[Hover_Exit]();
			}
		}

		if (isInside) {
			if (!currentEventData.inputUsed) {
				if (leftJustDown && eventCallbacks[MouseLeft_Down].operator bool()) {
					eventCallbacks[MouseLeft_Down]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
				if (middleJustDown && eventCallbacks[MouseMiddle_Down].operator bool()) {
					eventCallbacks[MouseMiddle_Down]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
				if (rightJustDown && eventCallbacks[MouseRight_Down].operator bool()) {
					eventCallbacks[MouseRight_Down]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
				if (leftJustUp && eventCallbacks[MouseLeft_Up].operator bool()) {
					eventCallbacks[MouseLeft_Up]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
				if (middleJustUp && eventCallbacks[MouseMiddle_Up].operator bool()) {
					eventCallbacks[MouseMiddle_Up]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
				if (rightJustUp && eventCallbacks[MouseRight_Up].operator bool()) {
					eventCallbacks[MouseRight_Up]();
					if (bUsesInput) {
						currentEventData.inputUsed = true;
					}
				}
			}
		}
		
	}
	
	void GUIBase::recalculateComplete() {
		onRecalculateComplete();
		for (auto child : getChildren()) {
			if (child != nullptr)
				if (!child->isHidden())
					child->recalculateComplete();
		}
	}

	void GUIBase::setParent(GUIBase * parent) {
		needsReconstructed = true;
		this->parent = parent;
	}



	/*
		
	GUI Parent -- Vector

	Lists of children in a row or something.
	
	*/
	bool GUIParentVector::addChild(std::shared_ptr<GUIBase> child) {
		if (canAddChild(child)) {
			needsReconstructed = true;
			child->setParent(this);
			children.push_back(child);
			child->updateEventData(currentEventData, lastEventData);
			return true;
		}
		return false;
	}
	bool GUIParentVector::removeChild(std::shared_ptr<GUIBase> child) {
		for (auto iter = children.begin(); iter != children.end(); iter++) {
			if (*iter == child) {
				needsReconstructed = true;
				children.erase(iter);
				return true;
			}
		}
		return false;
	}
	void GUIParentVector::clearChildren() {
		needsReconstructed = true;
		children.clear();
	}
	std::vector<std::shared_ptr<GUIBase>> GUIParentVector::getChildren() const {
		return children;
	}
	bool GUIParentVector::hasChildren() const {
		return children.size() > 0;
	}
	bool GUIParentVector::isParentTypeGUI() const {
		return true;
	}
	GUIParentSingle::GUIParentSingle() {
		child.resize(1);
	}
	bool GUIParentSingle::addChild(std::shared_ptr<GUIBase> child) {
		if (canAddChild(child)) {
			this->child[0] = child;
			child->setParent(this);
			child->updateEventData(currentEventData, lastEventData);
			return true;
		}
		return false;
	}
	bool GUIParentSingle::removeChild() {
		if (child[0] == nullptr) {
			return false;
		}
		child[0] = nullptr;
		return true;
	}
	bool GUIParentSingle::removeChild(std::shared_ptr<GUIBase> child) {
		if (child == this->child[0]) {
			this->child[0] = nullptr;
			return true;
		}
		return false;
	}
	void GUIParentSingle::clearChildren() {
		child[0] = nullptr;
	}
	std::vector<std::shared_ptr<GUIBase>> GUIParentSingle::getChildren() const {
		return child;
	}
	bool GUIParentSingle::hasChildren() const {
		return child[0] != nullptr;
	}
	bool GUIParentSingle::isParentTypeGUI() const {
		return true;
	}
}