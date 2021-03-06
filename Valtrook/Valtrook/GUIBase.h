#pragma once

#include <iterator>
#include <memory>
#include <array>
#include <vector>
#include <unordered_map>
#include <functional>
#include <type_traits>
#include "TimingType.h"
#include "AABB.h"
#include "Justification.h"

namespace Val {
	class RenderingEngine;


	const constexpr unsigned int GUIEventCount() {
		return 8;
	}

	enum GUIEventType : unsigned int {
		MouseLeft_Down = 0,
		MouseLeft_Up = 1,
		MouseMiddle_Down = 2,
		MouseMiddle_Up = 3,
		MouseRight_Down = 4, 
		MouseRight_Up =5,
		Hover_Enter = 6,
		Hover_Exit = 7
	};

	struct EventData {
		EventData() : EventData(false, false, false, 0, { 0.0f, 0.0f }) {
			inputUsed = true;
		};
		EventData(bool leftMouseStatus, bool middleMouseStatus, bool rightMouseStatus, int mouseWheelDelta, std::array<float, 2> mouseWorldPosition) : leftMouseDown(leftMouseStatus), middleMouseDown(middleMouseStatus), rightMouseDown(rightMouseStatus), mouseWheelDelta(mouseWheelDelta), mouseWorldPosition(mouseWorldPosition), inputUsed(false) {};

		bool leftMouseDown;
		bool middleMouseDown;
		bool rightMouseDown;

		int mouseWheelDelta;

		std::array<float, 2> mouseWorldPosition;

		bool inputUsed;
	};
	
	class GUIBase {
	public:
		GUIBase();
		~GUIBase();

		void reCalculate();
		bool needsRecalculated() const;

		void update(const TimingType& deltaTime);
		void render(const TimingType& deltaTime, RenderingEngine* engine);

		std::array<float, 2> getPosition() const;
		std::array<float, 2> getHalfSize() const;
		//Offsets from each parent added together
		std::array<float, 2> getAbsolutePosition() const;
		//Depth from each parent adjusted forwards
		float getDepth() const;

		bool hasParent() const;
		GUIBase* getParent() const;

		//Justification Stuff
		void setHorizontalJustification(const HorizontalJustification& horizontal);
		void setVerticalJustification(const VerticalJustification& vertical);
		void setJustification(const HorizontalJustification& horizontal, const VerticalJustification& vertical);
		HorizontalJustification getHorizontalJustification() const;
		VerticalJustification getVerticalJustification() const;
		std::pair<HorizontalJustification, VerticalJustification> getJustification() const;

		//do we process events on this gui object at all
		void setRecievesInputs(const bool& bInput);
		bool recievesInputs() const;
		
		//Stuff For Parent GUIs
		virtual bool addChild(std::shared_ptr<GUIBase> child);
		virtual bool removeChild(std::shared_ptr<GUIBase> child);
		virtual void clearChildren();
		
		virtual std::vector<std::shared_ptr<GUIBase>> getChildren() const;

		virtual bool hasChildren() const;
		virtual bool isParentTypeGUI() const;

		void setParent(GUIBase* parent);
		void setPosition(const std::array<float, 2>& position);

		void setHidden(const bool& hide);
		bool isHidden() const;

		void setEventCallback(std::function<void()> callback, GUIEventType eventType);
		void removeEventCallback(GUIEventType eventType);
		std::function<void()> getEventCallback(GUIEventType eventType) const;
		//The bool return will; if true set the events mouseDelta to 0 so the input is eaten
		void setEventMouseWheelCallback(std::function<bool(int)> callback);
		void removeEventMouseWheelCallback();
		std::function<bool(int)> getEventMouseWheelCallback() const;

		void clearEventCallbacks();
		//If we process events, do we use the input and stop other elements from seeing it.
		void setUsesInput(const bool& usesInput);
		bool usesInput() const;

		void updateEventData(EventData* current, EventData* last);
		void invalidate();

		void setCullAABB(const AABB<float>& cullAABB);
	protected:
		std::array<std::function<void()>, GUIEventCount()> eventCallbacks;
		std::function<bool(int)> eventMouseWheelCallback;
		EventData *currentEventData, *lastEventData;
		bool bUsesInput;
		void processEvents();
		bool isInsideToggle;

		virtual void preRecalculate();
		//Do changes you need to do on update, child updateing is handled for you
		virtual void internalUpdate(const TimingType& deltaTime) = 0;
		//Do your rendering, child rendering is handled for you
		virtual void internalRender(const TimingType& deltaTime, RenderingEngine* engine) = 0;
		//Places children within your size, they should all fit since you've already calculated your size based on theirs
		virtual void internalRecalculatePosition() = 0;
		//Calculate your size based on the childrens size
		virtual void internalRecalculateSize() = 0;

		virtual void onProcessEvents();

		virtual void leftMouseDown();
		virtual void middleMouseDown();
		virtual void rightMouseDown();
		virtual void leftMouseUp();
		virtual void middleMouseUp();
		virtual void rightMouseUp();
		virtual void hoverStart();
		virtual void hoverEnd();
		virtual bool mouseWheel(int delta);

		virtual void onSetCullAABB(const AABB<float>& cullAABB);

		void recalculateComplete();
		virtual void onRecalculateComplete() = 0;

		HorizontalJustification horizontal;
		VerticalJustification vertical;

		GUIBase* parent;

		std::array<float, 2> position;

		bool bHidden;
		mutable bool bJustHidden;

		std::array<float, 2> halfSize;

		AABB<float> interactionArea;
		AABB<float> cullAABB;
		bool hasCullAABB;

		bool bRecievesInputs;


		bool needsReconstructed;
	};

	class GUIParentSingle : public GUIBase {
	public:
		GUIParentSingle();

		bool addChild(std::shared_ptr<GUIBase> child) override;
		bool removeChild();
		bool removeChild(std::shared_ptr<GUIBase> child) override;
		void clearChildren() override;

		std::vector<std::shared_ptr<GUIBase>> getChildren() const override;

		bool hasChildren() const override;
		bool isParentTypeGUI() const override;
	protected:
		std::vector<std::shared_ptr<GUIBase>> child;

		virtual bool canAddChild(std::shared_ptr<GUIBase> child) = 0;
	};

	class GUIParentVector : public GUIBase {
	public:
		bool addChild(std::shared_ptr<GUIBase> child) override;
		bool removeChild(std::shared_ptr<GUIBase> child) override;
		void clearChildren() override;

		std::vector<std::shared_ptr<GUIBase>> getChildren() const override;
		bool hasChildren() const override;
		bool isParentTypeGUI() const override;

	protected:
		std::vector<std::shared_ptr<GUIBase>> children;

		virtual bool canAddChild(std::shared_ptr<GUIBase> child) = 0;
	};
}