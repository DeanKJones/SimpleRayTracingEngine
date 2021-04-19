#ifndef MATERIALH
#define MATERIALH

#include "Ray.h"
#include "Vec3.h"
#include "Hittable.h"
#include "Random.h"

class Material {
public:
    virtual bool scatter(
        const Ray& r_in, const hitRecord& rec, Vec3& attenuation,
    Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const Vec3& a) : albedo(a) {}
    virtual bool scatter(const Ray& r_in, const hitRecord& rec,
                        Vec3& attenuation, Ray& scattered) const override {
        Vec3 target = rec.p + rec.normal + randomInUnitSphere();

        scattered = Ray(rec.p, target, r_in.time());
        attenuation = albedo;
        return true;
    }
    Vec3 albedo;
};

// v3 reflect 

Vec3 reflect(const Vec3& v, const Vec3& n) {
    return v - 2 * dot(v, n) * n;
}

class Metal : public Material {
public:
    Metal(const Vec3& a, float f) : albedo(a) {
        if (f < 1)
            fuzz = f;
        else
            fuzz = 1;
    }
    virtual bool scatter(const Ray& r_in, const hitRecord& rec,
                        Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unitVector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected + fuzz * randomInUnitSphere(), r_in.time());
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    Vec3 albedo;
    float fuzz; //Roughness
};

// Refract
bool refract(const Vec3& v, const Vec3& n, float ni_over_nt, Vec3& refracted) {
    Vec3 uv = unitVector(v);
    float dt = dot(uv, n);
    float discriminant = 1.0 - ni_over_nt * ni_over_nt * (1 - dt * dt);
    if (discriminant > 0) {
        refracted = ni_over_nt * (uv - n * dt) - n * sqrt(discriminant);
        return true;
    }
    else 
        return false;
}

// Glass Reflectivity 
float schlick(float cosine, float ref_idx) {
    float r0 = (1 - ref_idx) / (1 + ref_idx);
    r0 = r0 * r0;
    return r0 + (1 - r0) * pow((1 - cosine), 5);
}

class Dielectric : public Material 
{
public:
    Dielectric(float ri) : ref_idx(ri) {}
    virtual bool scatter(const Ray& r_in, const hitRecord& rec, 
                        Vec3& attenuation, Ray& scattered
    ) const {
        Vec3 outwardNormal;
        Vec3 reflected = reflect(r_in.direction(), rec.normal);
        float ni_over_nt;
        attenuation = Vec3(1.0, 1.0, 1.0);
        Vec3 refracted;

        float reflectProb;
        float cosine;

        if (dot(r_in.direction(), rec.normal) > 0) {
            outwardNormal = -rec.normal;
            ni_over_nt = ref_idx;
            cosine = ref_idx * dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }
        else {
            outwardNormal = rec.normal;
            ni_over_nt = 1.0 / ref_idx;
            cosine = -dot(r_in.direction(), rec.normal) / r_in.direction().length();
        }

        if (refract(r_in.direction(), outwardNormal, ni_over_nt, refracted)) {
            reflectProb = schlick(cosine, ref_idx);
        }
        else {
            reflectProb = 1.0;
        }

        if (randomDouble() < reflectProb) {
            scattered = Ray(rec.p, reflected, r_in.time());
        }
        else {
            scattered = Ray(rec.p, refracted, r_in.time());
        }

        return true;
    }
    float ref_idx;
};

#endif