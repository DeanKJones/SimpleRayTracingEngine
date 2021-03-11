#ifndef CAMERAH
#define CAMERAH
#define _USE_MATH_DEFINES

#include "Ray.h"
#include <math.h>

class Camera {
public:
    Camera(Vec3 lookFrom, Vec3 lookAt, Vec3 vUp, float vfov, float aspect) {

        Vec3 u, v, w;

        float theta = vfov * M_PI / 180;
        float half_height = tan(theta / 2);
        float half_width = aspect * half_height;

        origin = lookFrom;
        w = unitVector(lookFrom - lookAt);
        u = unitVector(cross(vUp, w));
        v = cross(w, u);

        lowerLeftCorner = origin - half_width * u - half_height * v - w;
        horizontal = 2 * half_width * u;
        vertical = 2 * half_height * v;
    }
    Ray getRay(float s, float t) {
        return Ray(origin, 
                    lowerLeftCorner + s * horizontal + t * vertical - origin);
    }
    
    Vec3 origin;
    Vec3 lowerLeftCorner;
    Vec3 horizontal;
    Vec3 vertical;
};

#endif