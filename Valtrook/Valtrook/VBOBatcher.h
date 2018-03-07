#pragma once

#include "VBO.h"
#include "Glyph.h"
#include "GLBlendMode.h"
#include "Vertex.h"

#include <array>
#include <vector>
#include <unordered_map>
#include <type_traits>

namespace Val {
	template<typename GlyphType>
	struct GlyphContainer {
		GlyphContainer() : texture(0), glyphs() {}
		GlyphContainer(GlyphType glyph) : texture(glyph.textureId), glyphs() {
			glyphs.push_back(glyph);
		}

		void addGlyph(GlyphType glyph) {
			glyphs.push_back(glyph);
		}

		void clear() {
			glyphs.clear();
		}

		unsigned int texture;
		std::vector<GlyphType> glyphs;
	};

	struct RenderBatch {
		RenderBatch() : BlendMode(nullptr), texture(0), renderMode(0), offset(0), size(0) {}
		RenderBatch(GLBlendMode* BlendMode, unsigned inttexture, GLenum renderMode, unsigned int offset = 0, unsigned int size = 0) : BlendMode(BlendMode), texture(texture), renderMode(renderMode), offset(offset), size(size) {
		}

		GLBlendMode* BlendMode;
		unsigned int texture;
		GLenum renderMode;

		unsigned int offset;
		unsigned int size;
	};

	class VBOBatcher {
	public:
		VBOBatcher();
		~VBOBatcher();

		void initialise();

		void add(LineGlyph glyph);
		void add(RectangleGlyph glyph);
		void add(TriangleGlyph glyph);

		template<std::size_t Count>
		void add(std::array<LineGlyph, Count> glyphs);
		template<std::size_t Count>
		void add(std::array<RectangleGlyph, Count> glyphs);
		template<std::size_t Count>
		void add(std::array<TriangleGlyph, Count> glyphs);

		void add(std::vector<LineGlyph> glyph);
		void add(std::vector<RectangleGlyph> glyph);
		void add(std::vector<TriangleGlyph> glyph);


		void prepare();
		bool isPrepared() const;

		void render();
		void render(const GLBlendMode& forceBlendMode);

		void renderLines();
		void renderLines(const GLBlendMode& forceBlendMode);

		void clear();

		unsigned int VertexCount() {
			return verticies.size();
		}
	private:
		GLBlendMode lastBlendMode;
		void updateBlendMode(const GLBlendMode& mode);

		VBO vbo;
		std::vector<Vertex> verticies;
		
		std::unordered_map<GLBlendMode*, std::unordered_map<unsigned int, GlyphContainer<TriangleGlyph>>> glyphs;
		GlyphContainer<LineGlyph> lineGlyphs;

		std::vector<RenderBatch> batches;
		RenderBatch lineBatch;

		bool bPrepared;
		unsigned int vertexCount;
	};

	template<std::size_t Count>
	inline void VBOBatcher::add(std::array<LineGlyph, Count> glyphs) {
		for (unsigned int i = 0; i < Count; i++)
			add(glyphs[i]);
	}
	template<std::size_t Count>
	inline void VBOBatcher::add(std::array<RectangleGlyph, Count> glyphs) {
		for (unsigned int i = 0; i < Count; i++)
			add(glyphs[i]);
	}
	template<std::size_t Count>
	inline void VBOBatcher::add(std::array<TriangleGlyph, Count> glyphs) {
		for (unsigned int i = 0; i < Count; i++)
			add(glyphs[i]);
	}
}