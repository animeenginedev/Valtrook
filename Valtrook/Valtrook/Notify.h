#pragma once

#include <vector>
#include <array>
#include <functional>

namespace Val {
	//forward dec...
	template<typename T>
	class Notify;

	//Extend this to recieve a single notify directly to a class; I'd like to make a multireciever but i don't know how to approach it with template arguements Args...
	template<typename T>
	class NotifyReciever {
	public:
		Notify<T>* Source;

		virtual void notifyChanged() = 0;
		virtual void notifyDestroyed() = 0;
	};

	//Create a delegate, easiest way to work with it is to pass a lambda of some sort to funcOn<X>
	template<typename T>
	class NotifyDelegate : public NotifyReciever<T> {
	public:
		NotifyDelegate() {}

		void funcOnChange(std::function<void(T)> OnChange);
		void funcOnDestroy(std::function<void(T)> OnDestroy);
	protected:
		std::function<void(T)> OnChange;
		std::function<void(T)> OnDestroy;

		void notifyChanged() override;
		void notifyDestroyed() override;
	};

	//The core thing
	template<typename T>
	class Notify {
	public:

		Notify(T data);
		~Notify();

		void set(T data);
		T get() const;
		void addReciever(NotifyReciever<T>* Type);
		void removeReciever(NotifyReciever<T>* Type);
		std::size_t numRecievers() const;
		void clearRecievers();

		NotifyReciever<T>* operator[](const std::size_t& index) {
			return Recievers[index];
		}		
	private:
		T Data;
		std::vector<NotifyReciever<T>*> Recievers;
	};

	//Template implementation below here, enter at your own risk
	template<typename T>
	inline Notify<T>::Notify(T data) : Data(data) {
	}
	template<typename T>
	inline Notify<T>::~Notify() {
		for (unsigned int i = 0; i < Recievers.size(); ++i) {
			Recievers[i]->notifyDestroyed();
		}
	}
	template<typename T>
	inline void Notify<T>::set(T data) {
		Data = data;

		for (unsigned int i = 0; i < Recievers.size(); ++i) {
			Recievers[i]->notifyChanged();
		}
	}
	template<typename T>
	inline T Notify<T>::get() const {
		return Data;
	}

	template<typename T>
	inline void Notify<T>::addReciever(NotifyReciever<T>* Type) {
		Type->Source = this;
		Recievers.push_back(Type);
	}
	template<typename T>
	inline void Notify<T>::removeReciever(NotifyReciever<T>* Type) {
		for (auto iter = Recievers.begin(); iter != Recievers.end(); iter++) {
			if (iter == Type) {
				iter->notifyDestroyed();
				Recievers.erase(iter);
				break;
			}
		}
	}
	template<typename T>
	inline std::size_t Notify<T>::numRecievers() const {
		return Recievers.size();
	}
	template<typename T>
	inline void Notify<T>::clearRecievers() {
		for (unsigned int i = 0; i < Recievers.size(); ++i) {
			Recievers[i]->notifyDestroyed();
		}
		Recievers.clear();
	}
	template<typename T>
	inline void NotifyDelegate<T>::funcOnChange(std::function<void(T)> OnChange) {
		this->OnChange = OnChange;
	}
	template<typename T>
	inline void NotifyDelegate<T>::funcOnDestroy(std::function<void(T)> OnDestroy) {
		this->OnDestroy = OnDestroy;
	}
	template<typename T>
	inline void NotifyDelegate<T>::notifyChanged() {
		if (OnChange) {
			OnChange(Source->get());
			return;
		}
	}
	template<typename T>
	inline void NotifyDelegate<T>::notifyDestroyed() {
		if (OnDestroy) {
			OnDestroy(Source->get());
			return;
		}
	}
}