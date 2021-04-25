#ifndef RAYH
#define RAYH
#include "Common\Vec3.h"

class Ray
{
public:
    Ray() {}

    Ray(const Vec3& origin, const Vec3& direction, double ti = 0.0) :
        A(origin),
        B(direction),
        __time(ti)
    {
    }

    Vec3 origin() const { return A; }

    Vec3 direction() const { return B; }

    double time() const { return __time; }
    
    Vec3 pointAtParameter(float t) const { return A + t * B; }

    Vec3 A;
    Vec3 B;
    double __time;
};

#endif