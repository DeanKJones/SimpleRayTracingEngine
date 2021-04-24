#include "Sphere.h"

bool Sphere::hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const 
{
    Vec3 oc = r.origin() - center;

    float a = dot(r.direction(), r.direction());
    float b = dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;
    float discriminant = b*b - a*c;

    if (discriminant > 0) {
        float temp = (-b - sqrt(discriminant)) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
        temp = (-b + sqrt(discriminant)) / a;
        if (temp < tMax && temp > tMin) {
            rec.t = temp;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.matPtr = matPtr;
            return true;
        }
    }
    return false;
}

bool movingSphere::hit(const Ray& r, float tMin, float tMax, hitRecord& rec) const 
{
    Vec3 oc = r.origin() - center(r.time());

    auto a = r.direction().squaredLength();
    auto half_b = dot(oc, r.direction());
    auto c = oc.squaredLength() - radius * radius;

    float discriminant = half_b * half_b - a*c;

    if (discriminant < 0) 
        return false;

    auto sqrt_d = sqrt(discriminant);
    
    // Find the nearest root that lies in acceptable range
    auto root = (-half_b - sqrt_d) / a;
    if (root < tMin || tMax < root)
    {
        root = (-half_b + sqrt_d) / a;
        if (root < tMin || tMax < root)
        {
            return false;
        }
    }

    rec.t = root;
    rec.p = r.pointAtParameter(rec.t);

    auto outwardNormal = (rec.p - center(r.time())) / radius;
    rec.setFaceNormal(r, outwardNormal);
    rec.matPtr = matPtr;

    return true;
}

Vec3 movingSphere::center(float time) const{
    return center0 + ((time - time0) / (time1 - time0)) * (center1 - center0);
}
