#include "Sphere.h"

// External includes
#include <math.h>

// Internal includes
#include "GeometryOperations.h"
#include "Ray.h"

namespace TSFYP
{
	bool Sphere::Intersects(const Ray& ray, IntersectionResult* result)
	{
        Vector3 originToCentre = this->transform.pos - ray.origin;
        float tca = Dot(originToCentre, ray.dir);

        float distanceSq = Dot(originToCentre, originToCentre) - (tca * tca);
        float radiusSq = radius * radius;
        if (distanceSq > radiusSq)
            return false; // If distance^2 is larger than radius^2, there is no intersection

        // There is a possible intersection; calculate possible solutions
        float thc = sqrt(radiusSq - distanceSq);
        float t0 = tca - thc; // Solution 1
        float t1 = tca + thc; // Solution 2

        if (t0 < 0 && t1 < 0) // A negative solution is behind the ray origin
            return false; // If both solutions are negative, there is no intersection

        result = new IntersectionResult();
        result->t = t0 < t1 ? t0 : t1; // Store the smallest solution
        result->intersectionPoint = ray.origin + ray.dir * result->t;

        return true;
	}
}