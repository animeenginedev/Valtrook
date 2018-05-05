#pragma once

#include "ParticleFunction.h"
#include "Colour.h"

#include <vector>
#include <type_traits>
#include <random>

namespace Val {

	template<typename Type>
	class ParticleData {
	public:
		ParticleData(std::vector<Type> Values) : Values(Values) {}

		std::vector<Type> Values;
	};

	enum ParticleCreationType {
		PC_DEFAULT,
		PC_RANGE,
		PC_VARIANCE
	};

	template<typename Type>
	class ParticleAttribute {
		ParticleFunction<Type>* Function;
	public:
		ParticleAttribute();

		void Setup(ParticleFunction<Type>* Function, std::vector<Type> Values, bool Valid = true);
		void SetupVariance(ParticleFunction<Type>* Function, std::vector<Type> Values, std::vector<Type> Variance, bool Valid = true);
		void SetupRange(ParticleFunction<Type>* Function, std::vector<Type> ValuesLow, std::vector<Type> ValuesHigh, bool Valid = true);

		ParticleData<Type> GetParticleData();

		void UpdateCurrent(std::vector<Type> *Data, float deltaTime, float percentage, bool interpDeltaAddition = false, float offset = 0.0f) {
			Function->UpdateValue(deltaTime, percentage, Data);
		}

		bool SetUseNormalDistributionForVariance(bool useNormalDistribution) { UseNormalDistributionForVariance = useNormalDistribution; }
		bool IsValid() { return valid; }
	protected:
		ParticleCreationType AttributeCreationType;
		bool valid;
		//Held values for particle spawning
		std::vector<Type> Spawn_A, Spawn_B;

		ParticleData<Type> GetAsDefault();
		ParticleData<Type> GetAsVariance();
		ParticleData<Type> GetAsRange();

		bool UseNormalDistributionForVariance;

		float GetRandom();
	};


	template<typename Type>
	inline ParticleAttribute<Type>::ParticleAttribute() : valid(true), UseNormalDistributionForVariance(true) {
	}

	template<typename Type>
	inline void ParticleAttribute<Type>::Setup(ParticleFunction<Type>* Function, std::vector<Type> Values, bool Valid) {
		AttributeCreationType = PC_DEFAULT;
		this->Function = Function;
		Spawn_A = Values;
		valid = Valid;
	}

	template<typename Type>
	inline void ParticleAttribute<Type>::SetupVariance(ParticleFunction<Type>* Function, std::vector<Type> Values, std::vector<Type> Variance, bool Valid) {
		AttributeCreationType = PC_VARIANCE;
		this->Function = Function;
		Spawn_A = Values;
		Spawn_B = Variance;
		valid = Valid;
	}

	template<typename Type>
	inline void ParticleAttribute<Type>::SetupRange(ParticleFunction<Type>* Function, std::vector<Type> ValuesLow, std::vector<Type> ValuesHigh, bool Valid) {
		AttributeCreationType = PC_RANGE;
		this->Function = Function;
		Spawn_A = ValuesLow;
		Spawn_B = ValuesHigh;
		valid = Valid;
	}

	template<typename Type>
	inline ParticleData<Type> ParticleAttribute<Type>::GetParticleData() {
		switch (AttributeCreationType) {
			case PC_DEFAULT:
				return GetAsDefault();
			case PC_RANGE:
				return GetAsRange();
			case PC_VARIANCE:
				return GetAsVariance();
		}
		return GetAsDefault();
	}

	template<typename Type>
	inline ParticleData<Type> ParticleAttribute<Type>::GetAsDefault() {
		return ParticleData<Type>(Spawn_A);
	}

	template<typename Type>
	inline ParticleData<Type> ParticleAttribute<Type>::GetAsVariance() {
		std::vector<Type> VarianceResult = std::vector<Type>(Spawn_A.size());
		for (unsigned int i = 0; i < VarianceResult.size(); i++) {
			VarianceResult[i] = Spawn_A[i] - (Spawn_B[i] / 2.0f) + ((Spawn_B[i] * (((float)rand()) / RAND_MAX)));
		}
		return ParticleData<Type>(VarianceResult);
	}

	template<typename Type>
	inline ParticleData<Type> ParticleAttribute<Type>::GetAsRange() {
		std::vector<Type> RangeResult = std::vector<Type>(Spawn_A.size());
		for (unsigned int i = 0; i < RangeResult.size(); i++) {
			RangeResult[i] = Spawn_A[i] + ((Spawn_B[i] - Spawn_A[i]) * GetRandom());
		}
		return ParticleData<Type>(RangeResult);
	}

	template<typename Type>
	inline float ParticleAttribute<Type>::GetRandom() {
		static std::default_random_engine generator;
		static std::normal_distribution<float> normalDist = std::normal_distribution<float>(0.25f, 0.5f);
		if (UseNormalDistributionForVariance) {
			return normalDist(generator);
		} else {
			return (((float)rand()) / RAND_MAX);
		}
	}
}