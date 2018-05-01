#pragma once

#include "Glyph.h"
#include "AABB.h"
#include <vector>

namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class Line {
	public:
		Line(std::vector<std::array<float, 2>> positions, float x, float y, float depth, Colour colour);
		Line(std::vector<std::array<float, 2>> positions, std::array<float, 2> center, float depth, Colour colour);
		~Line();

		void add(std::array<float, 2> pos);
		template<typename... Args>
		void add(std::array<float, 2> pos, Args... args) {
			add(pos);
			add(args...);
		};
		void clear();
		unsigned int size() const;

		void setDepth(float depth);
		void setColour(Colour c);
		void setBlendMode(const GLBlendMode& blendMode);
		void setCullSurface(AABB<float> cullAABB);

		float getDepth() const;
		Colour getColour() const;
		GLBlendMode getBlendMode() const;
		AABB<float> getCullSurface() const;

		std::vector<LineGlyph> getRenderGlyphs();
		void sendRenderInformation(RenderingEngine* engine);
		void sendRenderInformation(VBOBatcher* batcher);

		void setFirstAndLastConnect(bool connectFirstAndLast);
		bool doesFirstAndLastConnect() const;
	protected:
		std::vector<LineGlyph> lines;

		std::array<float, 2> center;
		std::vector<std::array<float, 2>> positions;
		float depth;
		Colour lineColour;
		GLBlendMode blendMode;
		bool connectFirstAndLast;
		bool needsReconstructed;

		AABB<float> cullAABB;
		bool hasCullAABB;

		void reconstructVertexes();
	};
}