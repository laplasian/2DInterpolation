#include "Interpolator2D.h"
#include <iostream>
#include <fstream>

int main() {
    Interpolator2D interpolator2D(R"(C:\Users\user\CLionProjects\2DInterpolation\map)");

    interpolator2D.Bilinear(5);

    std::fstream file("output.txt");
    for (auto &v : interpolator2D.output) {
        for (auto &w : v) {
            file << w << " ";
        }
        file << std::endl;
    }
    file.close();

    return 0;
}