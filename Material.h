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
                        Vec3& attenuation, Ray& scattered) const {
        Vec3 target = rec.p + rec.normal + randomInUnitSphere();
        scattered = Ray(rec.p, target - rec.p);
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
    Metal(const Vec3& a) : albedo(a) {}
    virtual bool scatter(const Ray& r_in, const hitRecord& rec,
                        Vec3& attenuation, Ray& scattered) const {
        Vec3 reflected = reflect(unitVector(r_in.direction()), rec.normal);
        scattered = Ray(rec.p, reflected);
        attenuation = albedo;
        return (dot(scattered.direction(), rec.normal) > 0);
    }
    Vec3 albedo;
};