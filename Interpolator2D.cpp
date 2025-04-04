#include "Interpolator2D.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>

Interpolator2D::Interpolator2D() = default;

Interpolator2D::Interpolator2D(const std::string& filename) {
    std::ifstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ERROR! can`t open file: " + filename);
    }

    std::string line;

    while (std::getline(file, line)) {
        std::istringstream iss(line);
        double num;
        std::vector<double> buff {};
        while (iss >> num) {
            buff.emplace_back(num);
        }
        input.emplace_back(buff);
    }
    // Проверка размеров сетки
    Nsrc = input.size();

    for (auto &v : input) {
        if (v.size() != Nsrc) {
            throw std::runtime_error("ERROR! wrong grid dimension: " + std::to_string(Nsrc) + " != " + std::to_string(v.size()));
        }
    }

}

Interpolator2D::~Interpolator2D() = default;

void Interpolator2D::Bilinear(size_t Ndst) {
    if (Ndst == 0) {
        return;
    }
    output.resize(Ndst);
    for (auto &v: output) {
        v.resize(Ndst);
    }

    for (int y = 0; y < Ndst; ++y) {
        for (int x = 0; x < Ndst; ++x) {
            double srcX = (static_cast<double>(x) / static_cast<double>(Ndst)) * static_cast<double>(Nsrc);
            double srcY = (static_cast<double>(y) / static_cast<double>(Ndst)) * static_cast<double>(Nsrc);

            int x1 = static_cast<int>(std::floor(srcX));
            int y1 = static_cast<int>(std::floor(srcY));
            int x2 = std::min(x1 + 1, static_cast<int>(Nsrc) - 1);
            int y2 = std::min(y1 + 1, static_cast<int>(Nsrc) - 1);

            double dx = srcX - static_cast<double>(x1);
            double dy = srcY - static_cast<double>(y1);

            double Q11 = input[y1][x1];
            double Q12 = input[y2][x1];
            double Q21 = input[y1][x2];
            double Q22 = input[y2][x2];

            double interpolatedValue = (1 - dx) * (1 - dy) * Q11 +
                                      (1 - dx) * dy * Q12 +
                                      dx * (1 - dy) * Q21 +
                                      dx * dy * Q22;

            output[y][x] = interpolatedValue;
        }
    }


}

void Interpolator2D::Bicubic() {
}

void Interpolator2D::save_result(const std::string &filename) {
    std::fstream file(filename);
    if (!file.is_open()) {
        throw std::runtime_error("ERROR! can`t open/create file: " + filename);
    }
    for (auto &v : output) {
        for (auto &w : v) {
            file << w << " ";
        }
        file << std::endl;
    }
    file.close();
}

