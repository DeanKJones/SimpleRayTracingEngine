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

Hittable *randomScene() {

    int n = 500;
    Hittable **list = new Hittable*[n + 1];
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(Vec3(0.5, 0.5, 0.5)));
    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = randomDouble();
            Vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
                if ((center-Vec3(4, 0.2, 0)).length() > 0.9) {
                    if (chooseMat < 0.8) { // diffuse
                        list[i++] = new Sphere(center, 0.2,
                            new Lambertian(Vec3(randomDouble()*randomDouble(),
                                                randomDouble()*randomDouble(),
                                                randomDouble()*randomDouble())
                            )
                        );
                    }

                    else if (chooseMat < 0.95) { // metal
                        list[i++] = new Sphere(center, 0.2,
                                new Metal(Vec3(0.5 * (1 + randomDouble()),
                                                0.5 * (1 + randomDouble()),
                                                0.5 * (1 + randomDouble())),
                                                0.5 * randomDouble()));

                    }
                    else { // glass
                        list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                    }
                }
            }
        }
        list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
        list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(Vec3(0.4, 0.2, 0.1)));
        list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
        return new HittableList(list,i);
}

int main()
{
    std::ofstream output;
    output.open("Renders//output_30.ppm");

    int nx = 600;    
    int ny = 300;
    int ns = 100;

    output << "P3\n" << nx << " " << ny << "\n255\n";

/*
    Hittable *list[5];
    list[0] = new Sphere(Vec3(0, 0, -1), 0.5, new Lambertian(Vec3(0.6, 0.6, 0.6)));
    list[1] = new Sphere(Vec3(0, -100.5, -1), 100, new Lambertian(Vec3(0.4, 0.4, 0.4)));
    list[2] = new Sphere(Vec3(1, 0, -1), 0.5, new Metal(Vec3(0.8, 0.6, 0.2), 0.0));
    list[3] = new Sphere(Vec3(-1, 0, -1), 0.5, new Dielectric(1.5));
    list[4] = new Sphere(Vec3(-1, 0, -1), -0.45, new Dielectric(1.5));
*/

    Hittable *world = randomScene();

    float R = cos(M_PI/4);
    Vec3 lookFrom(-3, 3, 2);
    Vec3 lookAt(0, 0, -1);
    float distToFocus = (lookFrom - lookAt).length();
    float aperture = 1.0;

    Camera Cam(lookFrom, lookAt, Vec3(0, 1, 0), 60, float(nx)/float(ny), aperture, distToFocus);

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