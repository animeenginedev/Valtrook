#pragma once

#include "Vertex.h"
#include "GLTexture.h"
#include "GLBlendMode.h"
#include <array>

namespace Val {
	template<unsigned int Vertexes>
	struct Glyph {
		Glyph(unsigned int text, std::array<Vertex, Vertexes> verts) : texture(text), vertexes(verts) {};
		Glyph(unsigned int text, std::array<Vertex, Vertexes> verts, GLBlendMode* customBlend) : texture(text), vertexes(verts), blendeMode(customBlend) {};

		unsigned int textureId;
		std::array<Vertex, Vertexes> vertexes;
		GLBlendMode* blendMode = &GLBlendMode::Blend_Default;
	};

	struct TriangleGlyph : public Glyph<3> {
		TriangleGlyph(unsigned int text, std::array<Vertex, 3> verts, GLBlendMode* customBlend) : Glyph(text, verts, customBlend) {};
	};
	struct RectangleGlyph : public Glyph<6> {
		RectangleGlyph(unsigned int text, std::array<Vertex, 4> verts, GLBlendMode* customBlend) : Glyph(text, { verts[0], verts[1], verts[2], verts[2], verts[3], verts[1] }, customBlend) {};
		RectangleGlyph(unsigned int text, std::array<Vertex, 6> verts, GLBlendMode* customBlend) : Glyph(text, verts, customBlend) {};

		std::array<TriangleGlyph, 2> dispose() {
			return std::array<TriangleGlyph, 2>({ TriangleGlyph(textureId, std::array<Vertex, 3>({ vertexes[0], vertexes[1], vertexes[2] }), blendMode),
												  TriangleGlyph(textureId, std::array<Vertex, 3>({ vertexes[3], vertexes[4], vertexes[5] }), blendMode) });
		}
	};
	struct LineGlyph : public Glyph<2> {
		LineGlyph(unsigned int text, std::array<Vertex, 2> verts, GLBlendMode* customBlend) : Glyph(text, verts, customBlend) {};
	};
}