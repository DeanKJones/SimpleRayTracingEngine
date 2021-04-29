cmake_minimum_required(VERSION 3.15)
project(SimpleRayTraceEngine)

set(CMAKE_CXX_STANDARD 14)

add_executable(SimpleRayTraceEngine src/main.cpp 
                                    src/Common/vec3.h 
                                    src/Common/ray.h 
                                    src/hittable.h
                                    src/hittable.cpp 
                                    src/sphere.h
                                    src/sphere.cpp
                                    src/box.h
                                    src/box.cpp  
                                    src/hittable_list.h
                                    src/hittable_list.cpp 
                                    src/Common/camera.h 
                                    src/material.h 
                                    src/Common/aabb.h
                                    src/Common/External/toojpeg.h
                                    src/Common/External/toojpeg.cpp
                                    src/bvh_node.h
                                    src/bvh_node.cpp
                                    src/Common/color.h
                                    src/Common/texture.h
                                    src/Common/External/stb_image_write.h
                                    src/Common/External/stb_image.h
                                    src/Common/common.h
                                    src/Common/perlin.h
                                    src/Common/rtw_stb_image.h
                                    src/moving_sphere.h
                                    src/moving_sphere.cpp
                                    src/aarect.h
                                    src/aarect.cpp)