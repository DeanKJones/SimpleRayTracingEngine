#include "Common\common.h"
#include "Common\External\toojpeg.h"

#include "Common\camera.h"
#include "Common\color.h"
#include "hittable_list.h"
#include "material.h"
#include "sphere.h"
#include "box.h"
#include "moving_sphere.h"
#include "Common\texture.h"
#include "aarect.h"
#include "constant_medium.h"

#include <iostream>
#include <fstream>


color ray_color(const ray& r, const hittable& world, int depth, bool is_sky_active) {
    hit_record rec;

    if (depth <= 0)
        return color(0,0,0);

    if(!is_sky_active) 
    {
        color background = color(0, 0, 0);

        if (!world.hit(r, 0.001, infinity, rec)) {
            return background;
        }
        ray scattered;
        color attenuation;
        color emitted = rec.mat_ptr->emitted(rec.u, rec.v, rec.p);
        if (!rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return emitted;
        return emitted + attenuation * ray_color(scattered, world, depth - 1, is_sky_active);

    }
    else 
    {

        if (world.hit(r, 0.001, infinity, rec)) {
            ray scattered;
            color attenuation;
        if (rec.mat_ptr->scatter(r, rec, attenuation, scattered))
            return attenuation * ray_color(scattered, world, depth - 1, is_sky_active);
        return color(0,0,0);
        }

        vec3 unit_direction = unit_vector(r.direction());
        auto t = 0.5*(unit_direction.y() + 1.0);
        return (1.0-t)*color(1.0, 1.0, 1.0) + t*color(0.5, 0.7, 1.0);
    }
}


hittable_list random_scene() {
    hittable_list world;

    // Lambert Colors
    auto red = make_shared<lambertian>(color(0.65, 0.05, 0.05));
    auto white = make_shared<lambertian>(color(0.73, 0.73, 0.73));
    auto green = make_shared<lambertian>(color(0.12, 0.45, 0.15));
    // Metal Material
    auto metalic_mat = make_shared<metal>(color(0.7, 0.6, 0.5), 0.0);
    //Glass Material
    auto glass_mat = make_shared<dielectric>(1.5);
    // Checker Texture
    auto checker = make_shared<checker_texture>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    // Perlin Texture
    auto perlin_text = make_shared<noise_texture>();
    // Image Texture 
    auto image_text = make_shared<image_texture>("src\\Common\\External\\Textures\\roommates.jpg");
    auto image_surface = make_shared<lambertian>(image_text);
    // Emitter
    auto emitter = make_shared<diffuse_light>(color(10, 10, 10));
    // Volume
    //auto volumetric = make_shared<constant_medium>(/*geo*/, 0.01, color(1, 1, 1));

    // OBJECTS

    // Empty Cornell Box
    world.add(make_shared<yz_rect>(0, 555, 0, 555, 555, green));        // Left Wall
    world.add(make_shared<yz_rect>(0, 555, 0, 555, 0, red));            // Right Wall
    world.add(make_shared<xz_rect>(163, 393, 177, 382, 554, emitter));  // Light
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 0, white));          // Floor
    world.add(make_shared<xz_rect>(0, 555, 0, 555, 555, white));        // Roof
    world.add(make_shared<xy_rect>(0, 555, 0, 555, 555, white));        // Back Wall

    // Geo 
    // Box Center (227.5, 0.0, 227.5)
    shared_ptr<hittable> sphere_01 = make_shared<sphere>(point3(150, 300, 300), 75, glass_mat);
    shared_ptr<hittable> sphere_02 = make_shared<sphere>(point3(400, 75, 150), 75, white);

    shared_ptr<hittable> box_01 = make_shared<box>(point3(0, 0, 0), point3(165, 330, 165), metalic_mat);
    box_01 = make_shared<rotate_y>(box_01, 15);
    box_01 = make_shared<translate>(box_01, vec3(265, 0, 295));

    shared_ptr<hittable> box_02 = make_shared<box>(point3(0, 0, 0), point3(165, 165, 165), white);
    box_02 = make_shared<rotate_y>(box_02, -18);
    box_02 = make_shared<translate>(box_02, vec3(130, 0, 65));

    // Add to world
    world.add(sphere_01);
    world.add(make_shared<constant_medium>(sphere_02, 0.01, color(1, 1, 1)));
    world.add(box_01);
    world.add(box_02);

    // Scattered Spheres
    /*
    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b + 0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<material> sphere_material;

                if (choose_mat < 0.8) {
                    // diffuse
                    auto albedo = color::random() * color::random();
                    sphere_material = make_shared<lambertian>(albedo);
                    auto center2 = center + vec3(0, random_double(0,.5), 0);
                    world.add(make_shared<moving_sphere>(
                        center, center2, 0.0, 1.0, 0.2, sphere_material));
                } else if (choose_mat < 0.95) {
                    // metal
                    auto albedo = color::random(0.5, 1);
                    auto fuzz = random_double(0, 0.5);
                    sphere_material = make_shared<metal>(albedo, fuzz);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                } else {
                    // glass
                    sphere_material = make_shared<dielectric>(1.5);
                    world.add(make_shared<sphere>(center, 0.2, sphere_material));
                }
            }
        }
    }
    */
    
    return world;
}

/// JPEG OUTPUT ///
/*
std::ofstream myFile("..\\Renders_TheNextWeek\\output_01.jpg", std::ios_base::out | std::ios_base::binary); 

void myOutput(unsigned char byte){
    myFile << byte;
}

///
*/

int main() {

    // Image

    auto aspect_ratio = 16.0 / 9.0;
    int image_width;
    int samples_per_pixel;
    const int max_depth = 50;

    // World

    hittable_list world;

    point3 lookfrom;
    point3 lookat;
    auto vfov = 40.0;
    auto aperture = 0.0;
    bool sky_is_active;

    switch (0) {
        case 1:
            world = random_scene();
            sky_is_active = true;
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aperture = 0.1;
            aspect_ratio = 16.0 / 9.0;
            image_width = 500;
            samples_per_pixel = 100;
            break;

        case 2:
            world = random_scene();
            sky_is_active = true;
            lookfrom = point3(13,2,3);
            lookat = point3(0,0,0);
            vfov = 20.0;
            aspect_ratio = 16.0 / 9.0;
            image_width = 500;
            samples_per_pixel = 100;
            break;

        case 3:
            world = random_scene();
            sky_is_active = true;
            lookfrom = point3(13, 2, 3);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aspect_ratio = 16.0 / 9.0;
            image_width = 500;
            samples_per_pixel = 100;
            break;

        case 4:
            world = random_scene();
            sky_is_active = true;
            lookfrom = point3(13, 2, 4);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aspect_ratio = 16.0 / 9.0;
            image_width = 350;
            samples_per_pixel = 50;
            break;

        case 5:
            world = random_scene();
            sky_is_active = false;
            lookfrom = point3(13, 2, 4);
            lookat = point3(0, 0, 0);
            vfov = 20.0;
            aspect_ratio = 16.0 / 9.0;
            image_width = 720;
            samples_per_pixel = 100;
            break;

        case 6:
            world = random_scene();
            sky_is_active = false;
            lookfrom = point3(26, 3, 6);
            lookat = point3(0, 2, 0);
            vfov = 20.0;
            aspect_ratio = 16.0 / 9.0;
            image_width = 512;
            samples_per_pixel = 100;
            break;

        case 7:
            world = random_scene();
            sky_is_active = false;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40;
            aspect_ratio = 1.0;
            image_width = 1024;
            samples_per_pixel = 400;
            break;

        case 8:
            world = random_scene();
            sky_is_active = false;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40;
            aspect_ratio = 1.0;
            image_width = 500;
            samples_per_pixel = 100;
            break;

        default:
        case 9:
            world = random_scene();
            sky_is_active = false;
            lookfrom = point3(278, 278, -800);
            lookat = point3(278, 278, 0);
            vfov = 40;
            aspect_ratio = 1.0;
            image_width = 1024;
            samples_per_pixel = 1500;
            break;
    }

    // Camera

    vec3 vup(0,1,0);
    auto dist_to_focus = 10.0;
    int image_height = static_cast<int>(image_width / aspect_ratio);

    camera cam(lookfrom, lookat, vup, vfov, aspect_ratio, aperture, dist_to_focus, 0.0, 1.0);

    // Render

    //std::ofstream output;
    //output.open("..\\Renders_TheNextWeek\\output_01.ppm"); 
    //output << "P3\n" << image_width << " " << image_height << "\n255\n";

    std::cout << "P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = image_height-1; j >= 0; --j) {
        std::cerr << "\rScanlines remaining: " << j << ' ' << std::flush;
        for (int i = 0; i < image_width; ++i) {
            color pixel_color(0,0,0);
            for (int s = 0; s < samples_per_pixel; ++s) {
                auto u = (i + random_double()) / (image_width-1);
                auto v = (j + random_double()) / (image_height-1);
                ray r = cam.get_ray(u, v);
                pixel_color += ray_color(r, world, max_depth, sky_is_active);
            }
            write_color(std::cout, pixel_color, samples_per_pixel);

            //output << ir << " " << ig << " " << ib << "\n";
        }
    }

    //output.close();
    std::cerr << "\nDone.\n";

    /*
    const bool isRGB = true;
    const auto quality = 90;
    const bool downsample = false;
    const char* comment = "Render complete";

    auto render = TooJpeg::writeJpeg(myOutput, world, image_width, image_height, isRGB, quality, downsample, comment);
    */

    return 0;
}
