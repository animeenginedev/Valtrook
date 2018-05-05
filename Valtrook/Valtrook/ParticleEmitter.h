#pragma once

#include "Colour.h"
#include "ParticleAttribute.h"
#include "Particle.h"
#include "Rectangle.h"
#include "Texture.h"
#include "Glyph.h"

#include <functional>
#include <vector>
#include <array>

namespace Val {
	class ParticleEmitter {
	public:
		ParticleEmitter(OriginParticle Prototype, float SpawnsPerSecond);
		~ParticleEmitter();

		void update(float deltaTime);

		void giveRenderGlyphs(Rectangle& Renderable, std::vector<TriangleGlyph>* Glyphs, unsigned int offset);

		void SetEmitterPosition(float X, float Y);
		std::array<float, 2> GetEmitterPosition();

		void SetPrototype(OriginParticle p);
		OriginParticle* GetPrototype();

		void SetTexture(TextureResource tex);
		TextureResource GetTexture();

		void SetSpawnsPerSecond(float SpawnsPerSecond);
		float GetSpawnPerSecond();

		unsigned int GetParticleCount();

		void SetEmitterOrbitalFunctionPoint(float X, float Y);
		std::array<float, 2> GetEmitterOrbitalFuncitonPOint();

		std::vector<Particle>* GetAllParticles();
	protected:
		TextureResource Texture;
		OriginParticle Prototype;

		std::vector<Particle> Particles;

		float OffsetX, OffsetY;
		float OrbitX, OrbitY;

		//Particle Spawning
		float SpawnTimer;
		float SpawnPerSecondInverse;
	};
}