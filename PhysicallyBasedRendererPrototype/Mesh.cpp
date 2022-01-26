#include "Mesh.h"

namespace TSFYP
{
	Mesh::Mesh(const unsigned int noTriangles,
		const int* vertexIndices,
		const unsigned int noVertices,
		const Point3* vertexPositions,
		const Vector3* vertexTangents,
		const Vector3* vertexNormals,
		const Point2* vertexTexCoords)
		: noTriangles(noTriangles)
		, noVertices(noVertices)
	{}

	bool Mesh::Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const
	{
		return false;
	}
}