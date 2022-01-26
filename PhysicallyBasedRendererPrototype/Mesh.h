#pragma once

#include "IShape.h"

#include "Vector2.h"

namespace TSFYP
{
	// Stores the data of a vertex in a triangle-based mesh
	struct TriangleVertex
	{
		Point3 pos;
		Vector3 tangent;
		Vector3 normal;
		Point2 texCoords;
	};

	// Defines a triangle-based 3D mesh. Will later have an intersection function fully implemented to allow for raytracing
	class Mesh
		: public IShape
	{
	public:
		Mesh(const unsigned int noTriangles,
			const int* vertexIndices,
			const unsigned int noVertices,
			const Point3* vertexPositions,
			const Vector3* vertexTangents = nullptr,
			const Vector3* vertexNormals = nullptr,
			const Point2* vertexTexCoords = nullptr);

		virtual bool Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const override;

	private:
		const unsigned int noTriangles, noVertices;
	};
}