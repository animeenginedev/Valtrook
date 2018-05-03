#include "GLBlendMode.h"

namespace Val {
	GLBlendMode GLBlendMode::Blend_Default = GLBlendMode(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA, GL_ONE, GL_ONE_MINUS_SRC_ALPHA, 0);
	//GLBlendMode GLBlendMode::Blend_Additive = GLBlendMode(GL_SRC_ALPHA, GL_SRC_ALPHA, GL_SRC_ALPHA, GL_ONE, 1);
	GLBlendMode GLBlendMode::Blend_Additive = GLBlendMode(GL_SRC_ALPHA, GL_ONE, GL_SRC_ALPHA, GL_ONE, 1);

	void GLBlendMode::registerToScript(chaiscript::ChaiScript * script) {
		script->add(chaiscript::user_type<GLBlendMode>(), "BlendMode");

		script->add_global(chaiscript::var(GLBlendMode::Blend_Default), "GLBlend_Default");
		script->add_global(chaiscript::var(GLBlendMode::Blend_Additive), "GLBlend_Additive");

	}
}