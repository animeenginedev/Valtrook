#include "GameObject.h"

namespace Val {
	GameObject::GameObject(ScriptWrapper script, GameState * state) : script(script), owningState(state), position({ 0.0f, 0.0f }), isRenderFuncSafe(false), isUpdateFuncSafe(false) {
	}
	GameObject::~GameObject() {
	}
	void GameObject::create() {
		script.reload();
		script.getScript()->add_global(chaiscript::var(this), "This");
		onCreate();

		script.safetlyRunFunc(script.getFunction<void()>("onCreate"));
		bisValid = true;
		if (!script.isValid())
			return;
	}
	void GameObject::initialise() {
		onInitialise();
		script.safetlyRunFunc(script.getFunction<void()>("onInitialise"));
		bIsActive = true;
	}
	void GameObject::destroy() {
		bisValid = false;
		if (onDestroy()) {
			bisValid = true;
		}
		script.safetlyRunFunc(script.getFunction<void()>("onDestroy"));
	}
	void GameObject::onCreate() {	}
	void GameObject::onInitialise() {	}
	void GameObject::update(const TimingType delta) {
		if (!script.isValid())
			return;
		if (bisValid && bIsActive) {
			if (isUpdateFuncSafe) {
				updateFunc(delta);
			} else {
				updateFunc = script.getFunction<void(float)>("update");
				script.safetlyRunFunc(updateFunc, delta);
				if (updateFunc) {
					isUpdateFuncSafe = true;
				}
			}
		}
	}
	void GameObject::render(const TimingType delta, RenderingEngine * engine) {
		if (!script.isValid())
			return;
		if (bisValid && bIsActive) {
			if (isRenderFuncSafe) {
				renderFunc(delta, engine);
			} else {
				renderFunc = script.getFunction<void(float, RenderingEngine*)>("render");
				script.safetlyRunFunc(renderFunc, delta, engine);
				if (renderFunc) {
					isRenderFuncSafe = true;
				}
			}
		}
	}
	void GameObject::onActive(bool& activeStatus) {	}
	bool GameObject::onDestroy() {
		return false;
	}
	void GameObject::onNameSet(std::string& name) {	}
	void GameObject::setGameObjectName(std::string name) {
		objectName = name;
		//onNameSet(name);
		if(name != "")
			script.safetlyRunFunc(script.getFunction<void(std::string)>("objectNameSet"), name);
	}
	std::string GameObject::getGameObjectName() {
		return objectName;
	}
	bool GameObject::isActive() {
		return bIsActive;
	}
	void GameObject::setActive(bool active) {
		onActive(active);
		script.safetlyRunFunc(script.getFunction<void(bool)>("onActive"), active);
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
	std::string GameObject::getScriptName() {
		return script.getFileName();
	}
	ScriptWrapper * GameObject::getScriptWrapper() {
		return &script;
	}
	void GameObject::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<GameObject>(), "GameObject");

		script->add(chaiscript::fun(&GameObject::setX), "setX");
		script->add(chaiscript::fun(&GameObject::setY), "setY");
		script->add(chaiscript::fun<void, GameObject, std::array<float, 2>>(&GameObject::setPosition), "setPosition");
		script->add(chaiscript::fun<void, GameObject, float, float>(&GameObject::setPosition), "setPosition");

		script->add(chaiscript::fun(&GameObject::getX), "getX");
		script->add(chaiscript::fun(&GameObject::getY), "getY");
		script->add(chaiscript::fun(&GameObject::getPosition), "getPosition");
		script->add(chaiscript::fun(&GameObject::getOwningState), "getOwningState");
		script->add(chaiscript::fun(&GameObject::getGameObjectName), "getName");
	}
}