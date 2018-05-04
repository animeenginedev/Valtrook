#include "GameObject.h"

namespace Val {
	GameObject::GameObject( GameState * state) : owningState(state), position({ 0.0f, 0.0f }) {
	}
	GameObject::~GameObject() {
	}
	void GameObject::create() {
		onCreate();
		bisValid = true;
	}
	void GameObject::initialise() {
		onInitialise();
		bIsActive = true;
	}
	void GameObject::destroy() {
		bisValid = false;
		if (onDestroy()) {
			bisValid = true;
		}
	}
	void GameObject::onCreate() {	}
	void GameObject::onInitialise() {	}
	void GameObject::update(const TimingType delta) {
	}
	void GameObject::render(const TimingType delta, RenderingEngine * engine) {
	}
	void GameObject::onActive(bool& activeStatus) {	}
	bool GameObject::onDestroy() {
		return false;
	}
	void GameObject::onNameSet(std::string& name) {	}
	void GameObject::setGameObjectName(std::string name) {
		objectName = name;
	}
	std::string GameObject::getGameObjectName() {
		return objectName;
	}
	bool GameObject::isActive() {
		return bIsActive;
	}
	void GameObject::setActive(bool active) {
		onActive(active);
		bIsActive = active;
	}
	bool GameObject::canBeReplaced() {
		return !bisValid;
	}
	bool GameObject::isValid() {
		return bisValid;
	}
	GameState * GameObject::getOwningState() {
		return owningState;
	}
	void GameObject::setX(float x) {
		position[0] = x;
	}
	void GameObject::setY(float y) {
		position[1] = y;
	}
	void GameObject::setPosition(float x, float y) {
		position[0] = x;
		position[1] = y;
	}
	void GameObject::setPosition(std::array<float, 2> pos) {
		position = pos;
	}
	float GameObject::getX() const {
		return position[0];
	}
	float GameObject::getY() const {
		return position[1];
	}
	std::array<float, 2> GameObject::getPosition() const {
		return position;
	}
}