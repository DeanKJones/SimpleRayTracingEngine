#ifndef SPHEREH
#define SPHEREH

#include "Hittable.h"

class Sphere: public Hittable {
public:
    Sphere() {}
    Sphere (Vec3 cen, float r) : center(cen), radius(r) {};
    virtual bool hit(const Ray& r, float tmin, float tmax, hitRecord& rec) const;
    Vec3 center;
    float radius;
};

#endif