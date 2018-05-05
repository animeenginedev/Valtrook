#pragma once

#include "ParticleAttribute.h"

namespace Val {
	struct TrueColour;

	struct Particle {
		Particle(float LifeSpan, ParticleData<float> Position_X, ParticleData<float> Position_Y, ParticleData<float> Position_Z, ParticleData<float> Rotation, ParticleData<float> Width, ParticleData<float> Height, ParticleData<TrueColour> ParticleColour, ParticleData<float> Radial, ParticleData<float> Tangential);

		float LifeSpanInitial, LifeSpanRemaining;
		ParticleData<float> Position_X, Position_Y, Position_Z;
		ParticleData<float> Rotation;
		ParticleData<float> Width, Height;
		ParticleData<TrueColour> ParticleColour;
		ParticleData<float> Radial, Tangential;

		inline float GetLifeSpanPercentage() { return LifeSpanRemaining / LifeSpanInitial; };
	};

	struct OriginParticle {
		OriginParticle();

		//ParticleAttributes
		float LifeSpanInitial;
		ParticleAttribute<float> Position_X, Position_Y, Position_Z;
		ParticleAttribute<float> Rotation;
		ParticleAttribute<float> Width, Height;
		ParticleAttribute<TrueColour> ParticleColour;
		ParticleAttribute<float> Radial, Tangential;

		Particle CreateParticle();
	};
}