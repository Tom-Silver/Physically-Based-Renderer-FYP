#include "AACube.h"

// External includes
#include <algorithm>

// Internal includes
#include "GeometryOperations.h"
#include "Ray.h"

namespace TSFYP
{
    bool AACube::Intersects(const Ray& ray, const Transform& transform, IntersectionResult* result) const
    {
        // Transform object space bounds to world space
        Vector3 transformedBounds[2];
        transformedBounds[0] = bounds[0] + transform.pos;
        transformedBounds[1] = bounds[1] + transform.pos;

        // Precalculate inverted ray direction and the sign (represented by 0 (+) or 1 (-)) of each value
        Vector3 invDir = 1.0f / ray.dir;
        int sign[3];
        sign[0] = invDir.x < 0;
        sign[1] = invDir.y < 0;
        sign[2] = invDir.z < 0;

        // Uses the equation:
        // t = (bound - origin) / direction
        // to solve for t (distance along the ray of possible intersection)

        // Solve solutions for x
        float t0min = (transformedBounds[sign[0]].x - ray.origin.x) * invDir.x;
        float t0max = (transformedBounds[1 - sign[0]].x - ray.origin.x) * invDir.x;

        // Solve solutions for y
        float t1min = (transformedBounds[sign[1]].y - ray.origin.y) * invDir.y;
        float t1max = (transformedBounds[1 - sign[1]].y - ray.origin.y) * invDir.y;

        if ((t0min > t1max) || (t1min > t0max)) // If min solution > max solution,
            return false; // there is no intersection

        if (t1min > t0min) // If a new min/max solution is found,
            t0min = t1min; // replace the old solution
        if (t1max < t0max)
            t0max = t1max;

        // Solve solutions for z
        t1min = (transformedBounds[sign[1]].z - ray.origin.z) * invDir.z;
        t1max = (transformedBounds[1 - sign[1]].z - ray.origin.z) * invDir.z;

        if ((t0min > t1max) || (t1min > t0max))
            return false;

        if (t1min > t0min)
            t0min = t1min;
        if (t1max < t0max)
            t0max = t1max;

        result->t = t0min; // Store the smallest solution

        if (result->t < 0.0f) // A negative solution is behind the ray origin
        {
            result->t = t0max;
            if (result->t < 0.0f)
                return false; // If both solutions are negative, there is no intersection
        }

        result->intersectionPoint = Point3(Vector3(ray.origin) + ray.dir * result->t); // Store point of intersection (travel t distance along the ray)
        // Calculate surface normal at point of intersection
        {
            result->intersectionNormal = Vector3(0.0f, 0.0f, 1.0f);
        }
        Normalise(result->intersectionNormal);

        return true;
    }
}