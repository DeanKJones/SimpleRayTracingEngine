#ifndef HITTABLEH
#define HITTABLEH

#include "Ray.h"

class Material;
struct hitRecord{
    float t;
    Vec3 p;
    Vec3 normal;
    Material *matPtr;
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
