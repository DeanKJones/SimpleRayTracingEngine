#ifndef HITTABLELISTH
#define HITTABLELISTH

#include "Hittable.h"

class HittableList : public Hittable {
public:
    HittableList() {}

    HittableList(Hittable **l, int n) {
        list = l; 
        listSize = n;
        }

    virtual bool hit(
        const Ray& r, 
        float tmin, 
        float tmax,
        hitRecord& rec
    ) const;

    Hittable **list;
    int listSize;
};

#endif