#ifndef AABBH
#define AABBH
#include "..\Ray.h"
#include "..\Hittable.h"

inline float ffmin(float a, float b) { return a < b ? a : b; }
inline float ffmax(float a, float b) { return a > b ? a : b; }

class aabb {
    public:
        aabb() {}
        aabb(const Vec3& a, const Vec3& b) { _min = a; _max = b;}  

        Vec3 min() const {return _min; }
        Vec3 max() const {return _max; }

        bool hit(const Ray& r, float tmin, float tmax) const {
            for (int a = 0; a < 3; a++) {
                float t0 = ffmin((_min[a] - r.origin()[a]) / r.direction()[a],  
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                float t1 = ffmax((_min[a] - r.origin()[a]) / r.direction()[a],  
                                (_max[a] - r.origin()[a]) / r.direction()[a]);
                tmin = ffmax(t0, tmin);
                tmax = ffmin(t1, tmax);
                if (tmax <= tmin)
                    return false;
            }
            return true;
        }

        Vec3 _min;
        Vec3 _max;
};

inline aabb surrounding_box(aabb box0, aabb box1) {
    Vec3 small( ffmin(box0.min().x(), box1.min().x()),
                ffmin(box0.min().y(), box1.min().y()),
                ffmin(box0.min().z(), box1.min().z()));
    Vec3 big  ( ffmax(box0.max().x(), box1.max().x()),
                ffmax(box0.max().y(), box1.max().y()),
                ffmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}


#endif