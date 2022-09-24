#pragma once

// Parent includes
#include "Mesh.h"

namespace TSFYP
{
	// Implements a simple 1x1 2D quad mesh in normalised device coordinates
	class Quad :
		public Mesh
	{
	public:
		Quad(const unsigned int vao, const unsigned int indexCount);
		virtual ~Quad();
	};

	Quad* CreateQuad();
}