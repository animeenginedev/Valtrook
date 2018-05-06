#include "ParticleEmitter.h"


namespace Val {
	ParticleEmitter::ParticleEmitter(OriginParticle Prototype, float SpawnsPerSecond) : Prototype(Prototype), SpawnPerSecondInverse(1.0f / SpawnsPerSecond), SpawnTimer(0.0f), Texture(nullptr, ResourceLocation("Invalid", "", "")), OffsetX(0.0f), OffsetY(0.0f), OrbitX(0.0f), OrbitY(0.0f) {
	}


	ParticleEmitter::~ParticleEmitter() {
	}

	void ParticleEmitter::update(float deltaTime) {
		SpawnTimer -= deltaTime;
		while (SpawnTimer < 0.0f) {
			Particles.push_back(Prototype.CreateParticle());
			Particles.back().Position_X.Values[0] += OffsetX;
			Particles.back().Position_Y.Values[0] += OffsetY;

			SpawnTimer += SpawnPerSecondInverse;
		}

		for (unsigned int i = 0; i < Particles.size(); i++) {
			Particles[i].LifeSpanRemaining -= deltaTime;
			if (Particles[i].LifeSpanRemaining <= 0.0f) {
				if (i != Particles.size()) {
					Particles[i] = Particles.back();
					--i;
				}
				Particles.pop_back();
				continue;
			}

			float ParticleLifePercentage = 1.0f - Particles[i].GetLifeSpanPercentage();

			Prototype.Position_X.UpdateCurrent(&Particles[i].Position_X.Values, deltaTime, ParticleLifePercentage);
			Prototype.Position_Y.UpdateCurrent(&Particles[i].Position_Y.Values, deltaTime, ParticleLifePercentage);
			Prototype.Position_Z.UpdateCurrent(&Particles[i].Position_Z.Values, deltaTime, ParticleLifePercentage);
			Prototype.Width.UpdateCurrent(&Particles[i].Width.Values, deltaTime, ParticleLifePercentage);
			Prototype.Height.UpdateCurrent(&Particles[i].Height.Values, deltaTime, ParticleLifePercentage);
			Prototype.ParticleColour.UpdateCurrent(&Particles[i].ParticleColour.Values, deltaTime, ParticleLifePercentage);

			if (Prototype.Radial.IsValid() && Prototype.Tangential.IsValid()) {
				Prototype.Radial.UpdateCurrent(&Particles[i].Radial.Values, deltaTime, ParticleLifePercentage);
				Prototype.Tangential.UpdateCurrent(&Particles[i].Tangential.Values, deltaTime, ParticleLifePercentage);

				float RadialDir = std::atan2f(Particles[i].Position_Y.Values[0] - OrbitY, Particles[i].Position_X.Values[0] - OrbitX);
				float TangentialDir = RadialDir + 1.5708f;

				Particles[i].Position_X.Values[0] += (std::cos(TangentialDir) * Particles[i].Tangential.Values[0] * deltaTime) + (std::cos(RadialDir) * Particles[i].Radial.Values[0] * deltaTime);
				Particles[i].Position_Y.Values[0] += (std::sin(TangentialDir) * Particles[i].Tangential.Values[0] * deltaTime) + (std::sin(RadialDir) * Particles[i].Radial.Values[0] * deltaTime);

			} else if (Prototype.Radial.IsValid()) {
				Prototype.Radial.UpdateCurrent(&Particles[i].Radial.Values, deltaTime, ParticleLifePercentage);

				float RadialDir = std::atan2f(Particles[i].Position_Y.Values[0] - OrbitY, Particles[i].Position_X.Values[0] - OrbitX);

				Particles[i].Position_X.Values[0] += (std::cos(RadialDir) * Particles[i].Radial.Values[0] * deltaTime);
				Particles[i].Position_Y.Values[0] += (std::sin(RadialDir) * Particles[i].Radial.Values[0] * deltaTime);

			} else if (Prototype.Tangential.IsValid()) {
				Prototype.Tangential.UpdateCurrent(&Particles[i].Tangential.Values, deltaTime, ParticleLifePercentage);

				float TangentialDir = std::atan2f(Particles[i].Position_Y.Values[0] - OrbitY, Particles[i].Position_X.Values[0] - OrbitX) + 1.5708f;

				Particles[i].Position_X.Values[0] += (std::cos(TangentialDir) * Particles[i].Tangential.Values[0] * deltaTime);
				Particles[i].Position_Y.Values[0] += (std::sin(TangentialDir) * Particles[i].Tangential.Values[0] * deltaTime);
			}
		}
	}

	void ParticleEmitter::giveRenderGlyphs(Rectangle Renderable, std::vector<TriangleGlyph>* Glyphs, unsigned int offset) {
		Renderable.setTexture(Texture);
		std::array<TriangleGlyph, 2> Glyph = { TriangleGlyph(Texture.getGLTexture()->getTextureID(), {}, nullptr), TriangleGlyph(Texture.getGLTexture()->getTextureID(), {}, nullptr) };
		for (unsigned int i = 0; i < Particles.size(); i++) {
			Renderable.setCenter(Particles[i].Position_X.Values[0], Particles[i].Position_Y.Values[0]);
			Renderable.setSize(Particles[i].Width.Values[0], Particles[i].Height.Values[0]);
			Renderable.setRotation(Particles[i].Rotation.Values[0]);
			Renderable.setDepth(Particles[i].Position_Z.Values[0]);
			Renderable.setColour(Particles[i].ParticleColour.Values[0]);

			Glyph = Renderable.getRenderGlyphs();
			(*Glyphs)[offset++] = Glyph[0];
			(*Glyphs)[offset++] = Glyph[1];
		}
	}
	void ParticleEmitter::SetEmitterPosition(float X, float Y) {
		OffsetX = X;
		OffsetY = Y;
	}

	std::array<float, 2> ParticleEmitter::GetEmitterPosition() {
		return{ OffsetX, OffsetY };
	}

	void ParticleEmitter::SetPrototype(OriginParticle p) {
		Prototype = p;
	}

	OriginParticle * ParticleEmitter::GetPrototype() {
		return &Prototype;
	}

	void ParticleEmitter::SetTexture(TextureResource tex) {
		Texture = tex;
	}

	TextureResource ParticleEmitter::GetTexture() {
		return Texture;
	}

	void ParticleEmitter::SetSpawnsPerSecond(float SpawnsPerSecond) {
		SpawnPerSecondInverse = (1.0f / SpawnsPerSecond);
	}

	float ParticleEmitter::GetSpawnPerSecond() {
		return 1.0f / SpawnPerSecondInverse;
	}

	unsigned int ParticleEmitter::GetParticleCount() {
		return Particles.size();
	}
	void ParticleEmitter::SetEmitterOrbitalFunctionPoint(float X, float Y) {
		this->OrbitX = X;
		this->OrbitY = Y;
	}
	std::array<float, 2> ParticleEmitter::GetEmitterOrbitalFuncitonPOint() {
		return{ OrbitX, OrbitY };
	}
	std::vector<Particle>* ParticleEmitter::GetAllParticles() {
		return &Particles;
	}
}