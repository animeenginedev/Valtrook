#pragma once


namespace Val {
	class RenderingEngine;
	class VBOBatcher;

	class Renderable {
	public:
		virtual void sendRenderInformation(RenderingEngine* engine) = 0;
		virtual void sendRenderInformation(VBOBatcher* batcher) = 0;
	};
}