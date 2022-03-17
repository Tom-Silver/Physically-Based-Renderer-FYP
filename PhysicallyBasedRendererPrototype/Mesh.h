#pragma once

#include "Transform.h"

namespace TSFYP
{
	struct Mesh
	{
		Mesh(const unsigned int _vao, const unsigned int _indexCount)
		: vao(_vao)
		, indexCount(_indexCount)
		{}
		virtual ~Mesh() {}

		const unsigned int vao;
		const unsigned int indexCount;
	};
}