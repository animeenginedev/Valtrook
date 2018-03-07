#include "VBOBatcher.h"

Val::VBOBatcher::VBOBatcher() : lastBlendMode(0, 0, 0, 0, INT32_MAX) {
}

Val::VBOBatcher::~VBOBatcher() {
}

void Val::VBOBatcher::initialise() {
	vbo.initialise();
	batches.reserve(100);
	bPrepared = false;
}

void Val::VBOBatcher::add(LineGlyph glyph) {
	vertexCount += glyph.vertexes.size();
	lineGlyphs.addGlyph(glyph);
}

void Val::VBOBatcher::add(RectangleGlyph glyph) {
	std::array<TriangleGlyph, 2> tri = glyph.dispose();
	add(tri[0]);
	add(tri[1]);
}

void Val::VBOBatcher::add(TriangleGlyph glyph) {
	vertexCount += glyph.vertexes.size();
	auto blendMode = glyphs.find(glyph.blendMode);
	if (blendMode == glyphs.end()) {
		glyphs.insert(std::make_pair(glyph.blendMode, std::unordered_map<unsigned int, GlyphContainer<TriangleGlyph>>()));
		blendMode = glyphs.find(glyph.blendMode);
	}
	auto textureInsert = blendMode->second.find(glyph.textureId);
	if (textureInsert == blendMode->second.end()) {
		blendMode->second.insert(std::make_pair(glyph.textureId, GlyphContainer<TriangleGlyph>(glyph)));
	} else {
		textureInsert->second.addGlyph(glyph);
	}
}

void Val::VBOBatcher::add(std::vector<LineGlyph> glyph) {
	if (glyph.size() == 0)
		return;

	vertexCount += (2 * glyph.size());

	for (unsigned int e = 0; e < glyph.size(); e++) {
		lineGlyphs.addGlyph(glyph[e]);
	}
}
void Val::VBOBatcher::add(std::vector<RectangleGlyph> glyph) {
	if (glyph.size() == 0)
		return;

	vertexCount += (6 * glyph.size());
	std::array<TriangleGlyph, 2> cache(glyph[0].dispose());

	for (unsigned int e = 0; e < glyph.size(); e++) {
		cache = glyph[e].dispose();
		for (unsigned int i = 0; i < 2; i++) {
			auto blendMode = glyphs.find(glyph[e].blendMode);
			if (blendMode == glyphs.end()) {
				glyphs.insert(std::make_pair(glyph[e].blendMode, std::unordered_map<unsigned int, GlyphContainer<TriangleGlyph>>()));
				blendMode = glyphs.find(glyph[e].blendMode);
			}
			auto textureInsert = blendMode->second.find(glyph[e].textureId);
			if (textureInsert == blendMode->second.end()) {
				blendMode->second.insert(std::make_pair(glyph[e].textureId, GlyphContainer<TriangleGlyph>(cache[i])));
			} else {
				textureInsert->second.addGlyph(cache[i]);
			}
		}
	}
}
void Val::VBOBatcher::add(std::vector<TriangleGlyph> glyph) {
	if (glyph.size() == 0)
		return;

	vertexCount += (3 * glyph.size());

	for (unsigned int e = 0; e < glyph.size(); e++) {
		auto blendMode = glyphs.find(glyph[e].blendMode);
		if (blendMode == glyphs.end()) {
			glyphs.insert(std::make_pair(glyph[e].blendMode, std::unordered_map<unsigned int, GlyphContainer<TriangleGlyph>>()));
			blendMode = glyphs.find(glyph[e].blendMode);
		}
		auto textureInsert = blendMode->second.find(glyph[e].textureId);
		if (textureInsert == blendMode->second.end()) {
			blendMode->second.insert(std::make_pair(glyph[e].textureId, GlyphContainer<TriangleGlyph>(glyph[e])));
		} else {
			textureInsert->second.addGlyph(glyph[e]);
		}
	}
}

void Val::VBOBatcher::prepare() {
	if (glyphs.size() == 0 && lineGlyphs.glyphs.size() == 0) {
		bPrepared = true;
		return;
	}

	verticies.resize(0);
	verticies.resize(vertexCount);

	unsigned int offset = 0;
	unsigned int cv = 0;

	if (glyphs.size() != 0) {
		//Get triangle glyphs in
		for (auto cc : glyphs) {
			for (auto it : cc.second) {
				//Vertexes in a triangle = 3
				batches.push_back(RenderBatch(cc.first, it.first, GL_TRIANGLES, offset, it.second.glyphs.size() * 3));
				offset += it.second.glyphs.size() * 3;

				for (auto v = it.second.glyphs.begin(); v != it.second.glyphs.end(); v++) {
					verticies[cv++] = v->vertexes[0];
					verticies[cv++] = v->vertexes[1];
					verticies[cv++] = v->vertexes[2];
				}
			}
		}
	}

	if (lineGlyphs.glyphs.size() != 0) {
		//Get line glyphs in
		//Vertexes in a line == 2
		lineBatch = (RenderBatch(&GLBlendMode::Blend_Default, lineGlyphs.texture, GL_LINES, offset, lineGlyphs.glyphs.size() * 2));
		offset += lineGlyphs.glyphs.size() * 2;
		for (auto v : lineGlyphs.glyphs) {
			verticies[cv++] = v.vertexes[0];
			verticies[cv++] = v.vertexes[1];
		}
	}

	if (vertexCount == 0) {
		bPrepared = true;
		return;
	}

	vbo.bufferData(&verticies[0], vertexCount);
	bPrepared = true;
}

bool Val::VBOBatcher::isPrepared() const {
	return bPrepared;
}

void Val::VBOBatcher::render() {
	vbo.bindVertexArray();
	for (RenderBatch batch : batches) {
		updateBlendMode(*batch.BlendMode);
		glBindTexture(GL_TEXTURE_2D, batch.texture);

		vbo.render(batch.offset, batch.size, batch.renderMode);
	}
}

void Val::VBOBatcher::render(const GLBlendMode & forceBlendMode) {
	vbo.bindVertexArray();
	updateBlendMode(forceBlendMode);
	for (RenderBatch batch : batches) {
		glBindTexture(GL_TEXTURE_2D, batch.texture);

		vbo.render(batch.offset, batch.size, batch.renderMode);
	}
}

void Val::VBOBatcher::renderLines() {
	if (lineBatch.size != 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
		updateBlendMode(*lineBatch.BlendMode);
		vbo.bindVertexArray();
		vbo.render(lineBatch.offset, lineBatch.size, GL_LINES);
	}
}

void Val::VBOBatcher::renderLines(const GLBlendMode & forceBlendMode) {
	if (lineBatch.size != 0) {
		glBindTexture(GL_TEXTURE_2D, 0);
		updateBlendMode(forceBlendMode);
		vbo.bindVertexArray();
		vbo.render(lineBatch.offset, lineBatch.size, GL_LINES);
	}
}

void Val::VBOBatcher::clear() {
	for (auto it = glyphs.begin(); it != glyphs.end(); it++)
		it->second.clear();
	lineGlyphs.clear();
	lineBatch.size = 0;
	batches.clear();
	vertexCount = 0;
	bPrepared = false;
}

void Val::VBOBatcher::updateBlendMode(const GLBlendMode & mode) {
	if (mode != lastBlendMode) {
		glBlendFuncSeparate((mode).SrcColour, (mode).DstColour, (mode).SrcAlpha, (mode).DstAlpha);
		lastBlendMode = mode;
	}
}
