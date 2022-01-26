#include "Sphere.h"

// External includes
#include <math.h>

// Internal includes
#include "GeometryOperations.h"
#include "Ray.h"

namespace TSFYP
{
	bool Sphere::Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const
	{
        Vector3 originToCentre = transform.pos - Vector3(ray.origin);
        float tca = Dot(originToCentre, ray.dir);
        if (tca < 0)
            return false;

        float distanceSq = Dot(originToCentre, originToCentre) - (tca * tca);
        float radiusSq = radius * radius;
        if (distanceSq > radiusSq)
            return false; // If distance^2 is larger than radius^2, there is no intersection

        // There is a possible intersection; calculate possible solutions
        float thc = sqrtf(radiusSq - distanceSq);
        float t0 = tca - thc; // Solution 1
        float t1 = tca + thc; // Solution 2

        if (t0 < 0 && t1 < 0) // A negative solution is behind the ray origin
            return false; // If both solutions are negative, there is no intersection

        result->t = t0 < t1 ? t0 : t1; // Store the smallest solution
        result->intersectionPoint = Point3(Vector3(ray.origin) + ray.dir * result->t); // Store the point of intersection (travel t distance along the ray)
        result->intersectionNormal = Vector3(result->intersectionPoint) - transform.pos; // Calculate the surface normal at point of intersection
        Normalise(result->intersectionNormal);

        return true;
	}
}