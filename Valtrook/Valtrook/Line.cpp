#include "Line.h"

#include "RenderingEngine.h"
#include "GlyphCuller.h"

namespace Val {
	Line::Line(std::vector<std::array<float, 2>> positions, float x, float y, float depth, Colour colour) : Line(positions, { x, y }, depth, colour) {
	}
	Line::Line(std::vector<std::array<float, 2>> positions, std::array<float, 2> center, float depth, Colour colour) : positions(positions), center(center), depth(depth), lineColour(colour), blendMode(GLBlendMode::Blend_Default), hasCullAABB(false), needsReconstructed(true), connectFirstAndLast(false) {
	}
	Line::~Line() {
	}
	void Line::add(std::array<float, 2> pos) {
		positions.push_back(pos);
		needsReconstructed = true;
	}
	void Line::clear() {
		positions.clear();
		needsReconstructed = true;
	}
	unsigned int Line::size() const {
		return positions.size();
	}
	void Line::setDepth(float depth) {
		this->depth = depth;
		needsReconstructed = true;
	}
	void Line::setColour(Colour c) {
		lineColour = c;
		needsReconstructed = true;
	}
	void Line::setBlendMode(const GLBlendMode & blendMode) {
		this->blendMode = blendMode;
		needsReconstructed = true;
	}
	void Line::setCullSurface(AABB<float> cullAABB) {
		static AABB<float> noCullSurface = { 0.0f, 0.0f, 0.0f, 0.0f };
		this->cullAABB = cullAABB;
		hasCullAABB = !(cullAABB == noCullSurface);
		needsReconstructed = true;
	}
	float Line::getDepth() const {
		return depth;
	}
	Colour Line::getColour() const {
		return lineColour;
	}
	GLBlendMode Line::getBlendMode() const {
		return blendMode;
	}
	AABB<float> Line::getCullSurface() const {
		return cullAABB;
	}
	std::vector<LineGlyph> Line::getRenderGlyphs() {
		if (needsReconstructed)
			reconstructVertexes();

		return lines;
	}
	void Line::sendRenderInformation(RenderingEngine * engine) {
		if (needsReconstructed)
			reconstructVertexes();
		engine->getRenderer()->add(lines);
	}
	void Line::sendRenderInformation(VBOBatcher * batcher) {
		if (needsReconstructed)
			reconstructVertexes();
		batcher->add(lines);
	}
	void Line::setFirstAndLastConnect(bool connectFirstAndLast) {
		this->connectFirstAndLast = connectFirstAndLast;
		needsReconstructed = true;
	}
	bool Line::doesFirstAndLastConnect() const {
		return connectFirstAndLast;
	}
	void Line::reconstructVertexes() {
		needsReconstructed = false;

		lines.clear();
		lines.reserve(connectFirstAndLast ? positions.size() : positions.size() - 1);

		for (unsigned int i = 0; i < positions.size() - 1; i++) {
			lines.push_back(LineGlyph(0, {
			Vertex(positions[i][0], positions[i][1], depth, 0.0f, 0.0f, lineColour),
			Vertex(positions[i + 1][0], positions[i + 1][1], depth, 0.0f, 0.0f, lineColour)
			}, &blendMode));
		}

		if (connectFirstAndLast) {
			lines.push_back(LineGlyph(0, {
				Vertex(positions[0][0], positions[0][1], depth, 0.0f, 0.0f, lineColour),
				Vertex(positions[positions.size() - 1][0], positions[positions.size() - 1][1], depth, 0.0f, 0.0f, lineColour)
			}, &blendMode));
		}

		if (hasCullAABB) {
			lines = GlyphCuller::cullLines(lines, cullAABB);
		}
	}
}