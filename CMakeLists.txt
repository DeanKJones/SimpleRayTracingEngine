cmake_minimum_required(VERSION 3.15)
project(SimpleRayTraceEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleRayTraceEngine src/main.cpp 
                                    src/Common/Vec3.h 
                                    src/Ray.h 
                                    src/Hittable.h 
                                    src/Sphere.h 
                                    src/Sphere.cpp 
                                    src/HittableList.h 
                                    src/HittableList.cpp 
                                    src/Random.h 
                                    src/Common/Camera.h 
                                    src/Material.h 
                                    src/Common/aabb.h
                                    src/Common/External/toojpeg.h
                                    src/Common/External/toojpeg.cpp
                                    src/bvhNode.h
                                    src/Common/Color.h
                                    src/Common/Texture.h)