#ifndef SPHEREH
#define SPHEREH

#include "Hittable.h"

class Sphere: public Hittable {
public:
    Sphere() {}
    Sphere (Vec3 cen, float r, Material *m) : center(cen), radius(r), matPtr(m) {};
    virtual bool hit(const Ray& r, float tmin, float tmax, hitRecord& rec) const;
    Vec3 center;
    float radius;
    Material *matPtr;
};

#endif