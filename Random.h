#ifndef RANDOMH
#define RANDOMH

#include <functional>
#include <random>
#include "Vec3.h"

inline double randomDouble() {
    static std::uniform_real_distribution<double> distribution(0.0, 1.0);
    static std::mt19937 generator;
    static std::function<double()> randGenerator = std::bind(distribution, generator);
    return randGenerator();
}

#endif