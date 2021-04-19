#ifndef RANDOMH
#define RANDOMH

#include <functional>
#include <random>
#include "Vec3.h"
//class Vec3;

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> randGenerator = std::bind(distribution, generator);
    return randGenerator();
}

inline double randomDouble(double min, double max) {
    // Returns a random real in [min,max).
    return min + (max-min)*randomDouble();
}

inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

Vec3 randomInUnitSphere() {
    Vec3 p;
    do {
        p = 2.0 * Vec3(randomDouble(), randomDouble(), randomDouble()) - Vec3(1, 1, 1);
    } while (p.squaredLength() >= 1.0);
    return p;
}

Vec3 random_unit_vector() {
    return unitVector(randomInUnitSphere());
}

#endif