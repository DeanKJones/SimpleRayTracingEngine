cmake_minimum_required(VERSION 3.15)
project(SimpleRayTraceEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleRayTraceEngine main.cpp Vec3.h Ray.h Hittable.h Sphere.h Sphere.cpp HittableList.h HittableList.cpp Random.h Camera.h Material.h)