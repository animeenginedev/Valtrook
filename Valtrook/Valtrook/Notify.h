#pragma once

#include <vector>
#include <array>

namespace Val {
	//forward dec...
	template<typename T>
	class Notify;

	//Extend this to recieve a single notify directly to a class; I'd like to make a multireciever but i don't know how to approach it with template arguements Args...
	template<typename T>
	class NotifyReciever {
	public:
		Notify<T>* Source;

		virtual void NotifyChanged() = 0;
		virtual void NotifyDestroyed() = 0;
	};

	//Create this to recieve multiple types of changes, but only when you check
	template<typename T>
	class NotifyDelegate : public NotifyReciever<T> {
	public:
		NotifyDelegate() : bHasChanged(false), bHasBeenDestroyed(false) {}

		bool hasChanged() const;
		void resetHasChanged();

		bool hasBeenDestroyed() const;
		void resetHasBeenDestroyed();
	protected:
		bool bHasChanged;
		bool bHasBeenDestroyed;

		virtual void NotifyChanged();
		virtual void NotifyDestroyed();
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


	template<typename T>
	inline Notify<T>::Notify(T data) : Data(data) {
	}

	template<typename T>
	inline Notify<T>::~Notify() {
		for (unsigned int i = 0; i < Recievers.size(); ++i) {
			Recievers[i]->NotifyDestroyed();
		}
	}

	template<typename T>
	inline void Notify<T>::set(T data) {
		Data = data;

		for (unsigned int i = 0; i < Recievers.size(); ++i) {
			Recievers[i]->NotifyChanged();
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
				iter->NotifyDestroyed();
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
			Recievers[i]->NotifyDestroyed();
		}
		Recievers.clear();
	}
	template<typename T>
	inline bool NotifyDelegate<T>::hasChanged() const {
		return bHasChanged;
	}
	template<typename T>
	inline void NotifyDelegate<T>::resetHasChanged() {
		bHasChanged = false;
	}
	template<typename T>
	inline bool NotifyDelegate<T>::hasBeenDestroyed() const {
		return bHasBeenDestroyed;
	}
	template<typename T>
	inline void NotifyDelegate<T>::resetHasBeenDestroyed() {
		bHasBeenDestroyed = false;
	}
	template<typename T>
	inline void NotifyDelegate<T>::NotifyChanged() {
		bHasChanged = true;
	}
	template<typename T>
	inline void NotifyDelegate<T>::NotifyDestroyed() {
		bHasBeenDestroyed = true;
	}
}