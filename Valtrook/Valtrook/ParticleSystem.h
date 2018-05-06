#pragma once

#include "ParticleEmitter.h"
#include "Rectangle.h"

#include <vector>

namespace Val {
	class ParticleSystem {
	public:
		ParticleSystem();
		~ParticleSystem();

		void AddEmitter(ParticleEmitter emitter);

		void update(float deltaSeconds);
		void sendRenderingInformation(VBOBatcher* renderer);
		void sendRenderingInformation(RenderingEngine* renderer);

		void SetOffset(float x, float y);
	protected:
		Rectangle Render;
		std::vector<ParticleEmitter> Emitters;

		std::vector<TriangleGlyph> RenderGlyphs;
	};
}