#pragma once

namespace Val {
	struct UV {
		UV() : u(0.0f), v(0.0f), uWidth(1.0f), vHeight(1.0f) {};
		UV(unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int textureWidth, unsigned int textureHeight);
		UV(float u, float v, float uWidth, float vHeight);
		~UV();

		float u, v;
		float uWidth, vHeight;
	};
}