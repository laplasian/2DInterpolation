#include "Interpolator2D.h"

int main() {
    Interpolator2D interpolator2D(R"(C:\Users\user\CLionProjects\2DInterpolation\map)");

    interpolator2D.Bilinear(5);
    return 0;
}