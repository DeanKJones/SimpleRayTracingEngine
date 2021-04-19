#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES

#include "Ray.h"
#include "Random.h"
#include <math.h>

Vec3 randomInUnitDisk() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(randomDouble(), randomDouble(), 0) - Vec3(1, 1, 0);
    } while (dot(p, p) >= 1.0);
    return p;
}
class Camera {
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov, float aspect,
            float aperture, float focusDist,
            double t0, double t1) {

        time0 = t0;
        time1 = t1;
        lensRadius = aperture / 2;

        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        origin = lookFrom;
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vUp, w));
        v = cross(w, u);

        lowerLeftCorner = origin - half_width * focusDist * u - half_height * focusDist * v - focusDist * w;
        horizontal = 2 * half_width * focusDist * u;
        vertical = 2 * half_height * focusDist * v;
    }
    Ray getRay(float s, float t) {
        Vec3 rd = lensRadius * randomInUnitDisk();
        Vec3 offset = u * rd.x() + v * rd.y();
        return Ray(origin + offset, 
                    lowerLeftCorner + s * horizontal + t * vertical - origin - offset,
                    randomDouble(time0, time1)
                    );
    }
    
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
    Vec3 u, v, w;
    double time0, time1;
    float lensRadius;
};

#endif