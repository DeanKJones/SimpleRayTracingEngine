#ifndef AARECT_H
#define AARECT_H

#include "Common\common.h"
#include "hittable.h"

class xy_rect : public hittable {
public:
    xy_rect() {}

    xy_rect(double _x0, double _x1, double _y0, double _y1, double _k, shared_ptr<material> mat) :
        x0(_x0), x1(_x1), y0(_y0), y1(_y1), k(_k), mp(mat) {};

    virtual bool hit(const ray& r, double t_min, double t_max, hit_record& rec) const override;

    virtual bool bounding_box(double time0, double time1, aabb& output_box) const override;
    
public:
    shared_ptr<material> mp;
    double x0, x1, y0, y1, k;
};


#endif