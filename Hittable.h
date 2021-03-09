#ifndef HITTABLEH
#define HITTABLEH

#include "Ray.h"

struct hitRecord{
    float t;
    Vec3 p;
    Vec3 normal;
};

class Hittable {
public:
    virtual bool hit(
        const Ray& r, 
        float tMin, 
        float tMax, 
        hitRecord& rec
    ) const = 0;
};

#endif
