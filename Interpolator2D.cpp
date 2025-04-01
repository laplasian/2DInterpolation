#include "Interpolator2D.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>

Interpolator2D::Interpolator2D() = default;

Interpolator2D::Interpolator2D(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("Ошибка: не удалось открыть файл: " + filename);
    }

    std::string line;
    while (std::getline(file, line)) {
        input.emplace_back(line.begin(), line.end());
    }
    Nsrc = input.size();
}

Interpolator2D::~Interpolator2D() {
}

std::vector<double> Interpolator2D::Bilinear() {
}

std::vector<double> Interpolator2D::Bicubic() {
}

