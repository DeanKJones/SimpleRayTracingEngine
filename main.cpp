#include <iostream>
#include <fstream>

#include "Sphere.h"
#include "HittableList.h"
#include "Camera.h"
#include "Random.h"
#include "Material.h"

float MAX_FLOAT = std::numeric_limits<float>::max();

Vec3 color(const Ray& r, Hittable *world, int depth) 
{
    hitRecord rec;
    if (world->hit(r, 0.001, MAX_FLOAT, rec)) {
        Ray scattered;
        Vec3 attenuation;
        if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)){
            return attenuation * color(scattered, world, depth + 1);
        }
        else {
            return Vec3(0, 0, 0);
        }
    }
    else{
        Vec3 unitDirection = unitVector(r.direction());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * Vec3(1.0, 1.0, 1.0) + t * Vec3(0.5, 0.7, 1.0);
    }
}

int main() 
{
    std::ofstream output;
    output.open("Renders//output_27.ppm");

    int nx = 600;    
    int ny = 300;
    int ns = 100;

    output << "P3\n" << nx << " " << ny << "\n255\n";

    Hittable *list[5];
    list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.6, 0.6, 0.6)));
    list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.4, 0.4, 0.4)));
    list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
    Hittable *world = new HittableList(list, 5);

    float R = cos(M_PI/4);
    Camera Cam(Vec3(-2, 2, 1), Vec3(0, 0, -1), Vec3(0, 1, 0), 45, float(nx)/float(ny));

    for (int j = ny - 1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) 
        {
            Vec3 col(0, 0, 0);
            for (int s = 0; s < ns; s++) {
                float u = float(i + randomDouble()) / float(nx);
                float v = float(j + randomDouble()) / float(ny);

                Ray r = Cam.getRay(u, v);
                col += color(r, world, 0);
            }

            col /= float(ns);
            col = Vec3(sqrt(col[0]), sqrt(col[1]), sqrt(col[2]));

            int ir = int(255.99*col[0]);
            int ig = int(255.99*col[1]);
            int ib = int(255.99*col[2]);

            output << ir << " " << ig << " " << ib << "\n";
        }
    }
    output.close();
    return 0;
}