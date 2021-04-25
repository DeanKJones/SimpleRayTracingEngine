#include <iostream>
#include <fstream>
#include "Common\External\toojpeg.h"

#include "Sphere.h"
#include "HittableList.h"
#include "Common\Camera.h"
#include "Common\Color.h"

float MAX_FLOAT = std::numeric_limits<float>::max();

Vec3 RayColor(const Ray& r, Hittable *world, int depth) 
{
    hitRecord rec;

    if (depth <= 0){
        return color(0, 0, 0);
    }

    if (world->hit(r, 0.001, MAX_FLOAT, rec)) {
        Ray scattered;
        color attenuation;
        if (depth < 50 && rec.matPtr->scatter(r, rec, attenuation, scattered)){
            return attenuation * RayColor(scattered, world, depth + 1);
        }
        else {
            return color(0, 0, 0);
        }
    }
    else{
        Vec3 unitDirection = unitVector(r.direction());
        float t = 0.5 * (unitDirection.y() + 1.0);
        return (1.0 - t) * color(1.0, 1.0, 1.0) + t * color(0.5, 0.7, 1.0);
    }
}

Hittable *randomScene() {

    int n = 500;
    Hittable **list = new Hittable*[n + 1];

    Texture *checker = new checkerTexture(new constantTexture(Vec3(0.2, 0.3, 0.1)),
                                          new constantTexture(Vec3(0.9, 0.9, 0.9))   
    );
    list[0] = new Sphere(Vec3(0, -1000, 0), 1000, new Lambertian(checker));

    int i = 1;
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            float chooseMat = randomDouble();
            Vec3 center(a + 0.9 * randomDouble(), 0.2, b + 0.9 * randomDouble());
                if ((center-Vec3(4, 0.2, 0)).length() > 0.9) {
                    if (chooseMat < 0.8) 
                    { // diffuse
                        list[i++] = new movingSphere(
                                        center,
                                        center + Vec3(0, 0.5 * randomDouble(), 0),
                                        0.0, 1.0, 0.2, 
                                        new Lambertian(new constantTexture(
                                            Vec3(randomDouble()*randomDouble(),
                                            randomDouble()*randomDouble(),
                                            randomDouble()*randomDouble())))
                        );

                        list[i++] = new Sphere(center, 0.2,
                            new Lambertian(new constantTexture(
                                Vec3(randomDouble()*randomDouble(),
                                     randomDouble()*randomDouble(),
                                     randomDouble()*randomDouble())))
                        );
                    }

                    else //(chooseMat < 0.95) 
                    { // metal
                        list[i++] = new Sphere(center, 0.2,
                                new Metal(Vec3(0.5 * (1 + randomDouble()),
                                                0.5 * (1 + randomDouble()),
                                                0.5 * (1 + randomDouble())),
                                                0.5 * randomDouble()));

                    }
                    /*
                    else 
                    { // glass
                        list[i++] = new Sphere(center, 0.2, new Dielectric(1.5));
                    }
                    */
                }
            }
        }
        list[i++] = new Sphere(Vec3(0, 1, 0), 1.0, new Dielectric(1.5));
        list[i++] = new Sphere(Vec3(-4, 1, 0), 1.0, new Lambertian(new constantTexture(Vec3(0.4, 0.2, 0.1))));
        list[i++] = new Sphere(Vec3(4, 1, 0), 1.0, new Metal(Vec3(0.7, 0.6, 0.5), 0.0));
        return new HittableList(list,i);
}

/// JPEG OUTPUT ///
std::ofstream myFile("..\\Renders_TheNextWeek\\output_01.jpg", std::ios_base::out | std::ios_base::binary); 

void myOutput(unsigned char byte){
    myFile << byte;
}

///

int main()
{

/*
    std::ofstream output;
    output.open("..\\Renders_TheNextWeek\\output_01.ppm"); // "Renders_TheNextWeek//output_01.ppm"

    output << "P3\n" << nx << " " << ny << "\n255\n";
*/

    const int width = 600;    
    const int height = 300;
    const int samplesPerPixel = 100;
    const int maxDepth = 50;

    auto image = new unsigned char[width * height * samplesPerPixel];

    auto world = randomScene();

    float R = cos(M_PI/4);

    auto aspectRatio = 16.0 / 9.0;
    point3 lookFrom(13, 2, 3);
    point3 lookAt(0, 0, 0);
    Vec3 vup(0, 1, 0);

    float distToFocus = 10.0;
    float aperture = 0.0;

    Camera Cam(lookFrom, lookAt, vup, 20, aspectRatio, aperture, distToFocus, 0.0, 1.0);

    for (int j = height - 1; j >= 0; --j) {
        for (int i = 0; i < width; ++i) 
        {
            color pixelColor(0, 0, 0);
            for (int s = 0; s < samplesPerPixel; ++s) {
                auto u = (i + randomDouble()) / (width - 1);
                auto v = (j + randomDouble()) / (height - 1);

                Ray r = Cam.getRay(u, v);
                pixelColor += RayColor(r, world, maxDepth);
            }

            writeColor(std::cout, pixelColor, samplesPerPixel);

            //output << ir << " " << ig << " " << ib << "\n";
        }
    }
    //output.close();

    const bool isRGB = true;
    const auto quality = 90;
    const bool downsample = false;
    const char* comment = "Render complete";

    auto render = TooJpeg::writeJpeg(myOutput, world, width, height, isRGB, quality, downsample, comment);

    return 0;
}