#include "ParticleSystem.h"

#include "TaskMaster.h"
#include "RenderingEngine.h"

namespace Val {
	ParticleSystem::ParticleSystem() : Render(TextureAsset::getTexture("arrow.png"), { 0, 0 }, 0.5f, { 1.0f, 1.0f }, 0.0f, Colour(255, 255, 255, 255), &GLBlendMode::Blend_Additive) {
	}

	ParticleSystem::~ParticleSystem() {
	}

	void ParticleSystem::AddEmitter(ParticleEmitter emitter) {
		Emitters.push_back(emitter);
	}

	void ParticleSystem::update(float deltaSeconds) {
		//Multithreaded emitters are disabled right now as they don't work for some reason.
		/*if (Emitters.size() > 1) {
			//Multithreading masses of trig operations does give a reasonable speed increase ~(10% with 10K particles using trig) without any noticiable synchronization issues. The speed increase is more negligible when using purely linear particles.
			for (unsigned int i = 0; i < Emitters.size(); i++) {
				TaskMaster::addTask(MakeTask(std::bind(&ParticleEmitter::update, &Emitters[i], deltaSeconds)));
			}

			while (TaskMaster::getNumberOfTasks() > 0);
		} else {*/
			for (ParticleEmitter& emitter : Emitters) {
				emitter.update(deltaSeconds);
			}
		//}
	}

	void ParticleSystem::sendRenderingInformation(VBOBatcher * renderer) {
		RenderGlyphs.resize(0);
		if (Emitters.size() > 1) {
			unsigned int ParticleCount = 0;
			for (unsigned int i = 0; i < Emitters.size(); i++) {
				ParticleCount += Emitters[i].GetParticleCount();
			}

			RenderGlyphs.resize(ParticleCount * 2);
			ParticleCount = 0;

			for (unsigned int i = 0; i < Emitters.size(); i++) {
				Emitters[i].giveRenderGlyphs(Render, &RenderGlyphs, ParticleCount * 2);
				ParticleCount += Emitters[i].GetParticleCount();
			}
			renderer->add(RenderGlyphs);
		} else {
			RenderGlyphs.resize(Emitters[0].GetParticleCount() * 2);

			Emitters[0].giveRenderGlyphs(Render, &RenderGlyphs, 0);

			renderer->add(RenderGlyphs);
		}
	}

	void ParticleSystem::sendRenderingInformation(RenderingEngine * renderer) {
		RenderGlyphs.resize(0);
		if (Emitters.size() > 1) {
			unsigned int ParticleCount = 0;
			for (unsigned int i = 0; i < Emitters.size(); i++) {
				ParticleCount += Emitters[i].GetParticleCount();
			}

			RenderGlyphs.resize(ParticleCount * 2);
			ParticleCount = 0;

			for (unsigned int i = 0; i < Emitters.size(); i++) {
				Emitters[i].giveRenderGlyphs(Render, &RenderGlyphs, ParticleCount * 2);
				ParticleCount += Emitters[i].GetParticleCount();
			}
			renderer->getRenderer()->add(RenderGlyphs);
		} else {
			RenderGlyphs.resize(Emitters[0].GetParticleCount() * 2);

			Emitters[0].giveRenderGlyphs(Render, &RenderGlyphs, 0);

			renderer->getRenderer()->add(RenderGlyphs);
		}
	}

	void ParticleSystem::SetOffset(float x, float y) {
		for (ParticleEmitter& emitter : Emitters) {
			emitter.SetEmitterPosition(x, y);
		}
	}
}