#pragma once

#include "Random.h"
#include <random>
#include <vector>

namespace Val {
	/*
		Weighted Uniform Distribution; Red = 1.0, Blue = 2.0, Yellow = 3.0 example output may be similar too { Yellow, Red, Blue, Yellow, Blue, Yellow }
		(Basically you hope to keep proportion between the inputs)
	*/
	template<typename T>
	class ProportionList {
	public:
		template<typename T>
		class Element {
		public:
			Element(const T& data, double rate) : data(data), rate(rate) {
			}
			Element(T&& data, double rate) : data(data), rate(rate) {
			}

			T data;
			double rate;
		};

		ProportionList();
		template<typename... Args>
		ProportionList(Args... args);
		~ProportionList();

		void insert(const T& element, double frequency);
		void insert(T&& element, double frequency);
		template<typename... Args>
		void insert(const T& element, double frequency, Args&&... args);
		template<typename... Args>
		void insert(T&& element, double frequency, Args&&... args);
		const T& get(Random* rng) const;

		void clear() const {
			elements.clear();
			totalFrequency = 0.0;
		}

		bool empty() const;
		std::size_t size() const;
		const T& operator[](std::size_t index) const;

	protected:
		std::vector<Element<T>> elements;
		double totalFrequency;
	};

	template<typename T>
	inline ProportionList<T>::ProportionList() : totalFrequency(0.0) {
	}

	template<typename T>
	template<typename... Args>
	inline ProportionList<T>::ProportionList(Args... args) : totalFrequency(0.0f) {
		insert(args...);
	}

	template<typename T>
	inline ProportionList<T>::~ProportionList() {
	}

	template<typename T>
	inline void ProportionList<T>::insert(const T & element, double frequency) {
		elements.push_back(Element<T>(element, frequency));
		totalFrequency += frequency;
	}

	template<typename T>
	inline void ProportionList<T>::insert(T && element, double frequency) {
		elements.push_back(Element<T>(element, frequency));
		totalFrequency += frequency;
	}

	template<typename T>
	inline const T & ProportionList<T>::get(Random* rng) const {
		double ChoosenValue = std::uniform_real<double>(0.0, static_cast<double>(totalFrequency))(rng->getGenerator());
		double totalValue = 0.0;
		for (unsigned int i = 0; i < elements.size(); ++i) {
			totalValue += elements[i].rate;
			if (totalValue >= ChoosenValue)
				return elements[i].data;
		}
		return elements.back().data;
	}

	template<typename T>
	inline bool ProportionList<T>::empty() const {
		return elements.empty();
	}

	template<typename T>
	inline std::size_t ProportionList<T>::size() const {
		return elements.size();
	}

	template<typename T>
	inline const T & ProportionList<T>::operator[](std::size_t index) const {
		return elements[index].data;
	}

	template<typename T>
	template<typename ...Args>
	inline void ProportionList<T>::insert(const T & element, double frequency, Args && ...args) {
		insert(element, frequency);
		insert(args...);
	}

	template<typename T>
	template<typename ...Args>
	inline void ProportionList<T>::insert(T && element, double frequency, Args && ...args) {
		insert(element, frequency);
		insert(args...);
	}
}