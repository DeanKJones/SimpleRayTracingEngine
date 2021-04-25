#include "Vec3.h"

class Texture {
public:
    virtual Vec3 value(float u, float v, const Vec3& p) const = 0;
};

class constantTexture : public Texture {
public:
    constantTexture() {}
    constantTexture(Vec3 c) : color(c) {}

    virtual Vec3 value(float u, float v, const Vec3& p) const {
        return color;
    }

    Vec3 color;
};

class checkerTexture : public Texture {
public:
    checkerTexture() {}
    checkerTexture(Texture *t0, Texture *t1) : even(t0), odd(t1) {}
    virtual Vec3 value(float u, float v, const Vec3& p) const {
        float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());
        if (sines < 0)
            return odd->value(u, v, p);
        else 
            return even->value(u, v, p);
    }

    Texture *odd;
    Texture *even;
};