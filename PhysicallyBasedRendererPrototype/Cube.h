#pragma once

// Parent includes
#include "Mesh.h"

namespace TSFYP
{
	// Implements a simple 1x1 3D cube mesh in normalised device coordinates
	class Cube :
		public Mesh
	{
	public:
		Cube(const unsigned int vao, const unsigned int indexCount);
		virtual ~Cube();
	};

	Cube* CreateCube();
}