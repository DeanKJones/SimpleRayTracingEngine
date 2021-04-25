#ifndef SPHEREH
#define SPHEREH

#include "Hittable.h"
#include "Material.h"
#include "Common\aabb.h"

class Sphere: public Hittable {
public:
    Sphere() {}
    Sphere (Vec3 cen, float r, Material *m) : center(cen), radius(r), matPtr(m) {};
    virtual bool hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const;
    bool boundingBox(float t0, float t1, aabb& box) const;
    Vec3 center;
    float radius;
    Material *matPtr;
};

class movingSphere : public Hittable {
public:
    movingSphere() {}
    movingSphere(Vec3 cen0, Vec3 cen1, double t0, double t1, double r, Material *m)
    : center0(cen0), center1(cen1), time0(t0), time1(t1), radius(r), matPtr(m) {};

    virtual bool hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const;
    virtual bool boundingBox(float t0, float t1, aabb& box) const;
   
    Vec3 center(float time) const;
    Vec3 center0, center1;
    double time0, time1;
    double radius;
    Material *matPtr;
};

#endif