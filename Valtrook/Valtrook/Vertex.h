#pragma once

#include <GL\glew.h>
#include "Colour.h"

namespace Val {
	struct Vertex {
		Vertex() : x(0), y(0), z(0), u(0.0f), v(0.0f), c(0, 0, 0, 0) {}
		Vertex(float x, float y, float z) : x(x), y(y), z(z), u(0.0f), v(0.0f), c(0, 0, 0, 0) {}
		Vertex(float x, float y, float z, float u, float v) : x(x), y(y), z(z), u(u), v(v), c(0, 0, 0, 0) {}
		Vertex(float x, float y, float z, float u, float v, GLubyte r, GLubyte g, GLubyte b, GLubyte a) : x(x), y(y), z(z), u(u), v(v), c(r, g, b, a) {}
		Vertex(float x, float y, float z, float u, float v, Colour c) : x(x), y(y), z(z), u(u), v(v), c(c) {}
		
		float x, y, z;
		float u, v;
		Colour c;

		static void SetAttributes() {
			glEnableVertexAttribArray(0);
			glEnableVertexAttribArray(1);
			glEnableVertexAttribArray(2);

			glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, x));
			glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, u));
			glVertexAttribPointer(2, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (void*)offsetof(Vertex, c));
		}
	};
}