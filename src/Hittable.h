#ifndef HITTABLEH
#define HITTABLEH

#include "Ray.h"
#include "Common\aabb.h"

class Material;
struct hitRecord{
    float t;
    Vec3 p;
    Vec3 normal;
    Material *matPtr;
    bool frontFace;

    inline void setFaceNormal(const Ray& r, const Vec3& outward_normal) {
        frontFace = dot(r.direction(), outward_normal) < 0;
        normal = frontFace ? outward_normal :-outward_normal;
    }
};

class Hittable {
public:
    virtual bool hit(const Ray& r, 
                     float tMin, 
                     float tMax, 
                     hitRecord& rec) const = 0;

    virtual bool boundingBox(float t0,
                             float t1,
                             aabb& box) const = 0;
};

#endif