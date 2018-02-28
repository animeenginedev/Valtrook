#include "UV.h"

Val::UV::UV(unsigned int left, unsigned int top, unsigned int width, unsigned int height, unsigned int textureWidth, unsigned int textureHeight) : 
u(static_cast<float>(left) / static_cast<float>(textureWidth)),
v(static_cast<float>(top) / static_cast<float>(textureHeight)), 
uWidth(static_cast<float>(width) / static_cast<float>(textureWidth)),
vHeight(static_cast<float>(height) / static_cast<float>(textureHeight)) {
}

Val::UV::UV(float u, float v, float uWidth, float vHeight) :
u(u),
v(v),
uWidth(uWidth),
vHeight(vHeight) {
}

Val::UV::~UV() {
}
