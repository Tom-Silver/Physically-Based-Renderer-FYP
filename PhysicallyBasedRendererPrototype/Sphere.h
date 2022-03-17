#pragma once

// Parent includes
#include "Mesh.h"

namespace TSFYP
{
	// Implements a simple 3D sphere mesh
	class Sphere :
		public Mesh
	{
	public:
		Sphere(const unsigned int vao, const unsigned int indexCount, const float _radius)
			: radius(_radius)
			, radiusSq(_radius * _radius)
			, Mesh(vao, indexCount)
		{}
		virtual ~Sphere() {}

	private:
		float radius, radiusSq;
	};

	Sphere* CreateSphere(const float radius);
}