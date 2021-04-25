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

        inline bool aabb::hit(const Ray& r, double t_min, double t_max) const {
            for (int a = 0; a < 3; a++) {
            auto invD = 1.0f / r.direction()[a];
            auto t0 = (min()[a] - r.origin()[a]) * invD;
            auto t1 = (max()[a] - r.origin()[a]) * invD;
            if (invD < 0.0f)
                std::swap(t0, t1);
            t_min = t0 > t_min ? t0 : t_min;
            t_max = t1 < t_max ? t1 : t_max;
            if (t_max <= t_min)
                return false;
            }
            return true;
        }

        Vec3 _min;
        Vec3 _max;
};

inline aabb surroundingBox(aabb box0, aabb box1) {
    Vec3 small( ffmin(box0.min().x(), box1.min().x()),
                ffmin(box0.min().y(), box1.min().y()),
                ffmin(box0.min().z(), box1.min().z()));
    Vec3 big  ( ffmax(box0.max().x(), box1.max().x()),
                ffmax(box0.max().y(), box1.max().y()),
                ffmax(box0.max().z(), box1.max().z()));
    return aabb(small, big);
}


#endif