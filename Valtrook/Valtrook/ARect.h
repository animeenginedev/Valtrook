#pragma once

#include <array>
#include "Renderable.h"

namespace Val {
	class ARect : public Renderable {
	public:
		ARect(std::array<float, 2> center, std::array<float, 2> halfSize, float depth);
		~ARect();

		void setX(float x);
		void setY(float y);
		void setCenter(float x, float y);
		void setCenter(std::array<float, 2> center);
		void setDepth(float depth);
		
		float getX() const;
		float getY() const;
		std::array<float, 2> getCenter() const;
		float getDepth() const;
		float getHalfWidth() const;
		float getHalfHeight() const;
		std::array<float, 2> getHalfSize() const;

	protected:
		std::array<float, 2> center;
		float depth;
		std::array<float, 2> halfSize;

		bool needsReconstructed;
	};
}
