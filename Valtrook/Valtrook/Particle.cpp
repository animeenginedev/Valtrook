#include "Particle.h"

#include "ParticleFunctions.h"
#include "Colour.h"

namespace Val {
	Particle::Particle(float LifeSpan, ParticleData<float> Position_X, ParticleData<float> Position_Y, ParticleData<float> Position_Z, ParticleData<float> Rotation, ParticleData<float> Width, ParticleData<float> Height, ParticleData<TrueColour> ParticleColour, ParticleData<float> Radial, ParticleData<float> Tangential) : LifeSpanInitial(LifeSpan), LifeSpanRemaining(LifeSpan), Position_X(Position_X), Position_Y(Position_Y), Position_Z(Position_Z), Rotation(Rotation), Width(Width), Height(Height), ParticleColour(ParticleColour), Radial(Radial), Tangential(Tangential) {
	}


	OriginParticle::OriginParticle() {
		Position_X.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f });
		Position_Y.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f });
		Position_Z.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f });
		Rotation.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f });
		Width.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.25f });
		Height.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.25f });
		ParticleColour.Setup(ParticleFunctions::Functions_TrueColour.Function_Static, { TrueColour(255, 255, 255, 255) });
		Radial.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f }, false);
		Tangential.Setup(ParticleFunctions::Functions_Float.Function_Static, { 0.0f }, false);
	}

	Particle OriginParticle::CreateParticle() {
		return Particle(LifeSpanInitial, Position_X.GetParticleData(), Position_Y.GetParticleData(), Position_Z.GetParticleData(),
						Rotation.GetParticleData(), Width.GetParticleData(), Height.GetParticleData(), ParticleColour.GetParticleData(), Radial.GetParticleData(), Tangential.GetParticleData());
	}
}