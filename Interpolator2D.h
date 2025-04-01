#ifndef INTERPOLATOR2D_H
#define INTERPOLATOR2D_H
#include <string>
#include <vector>

#define max_line_size 256

class Interpolator2D {
public:
    Interpolator2D();
    Interpolator2D(const std::string& filename);
    ~Interpolator2D();

    std::vector<double> output = {};
    size_t padding = 0;
    double pad_fill = 0;
    size_t Ndst = 0;

    std::vector<double> Bilinear();
    std::vector<double> Bicubic();

private:
    std::vector<std::vector<double>> input = {};
    size_t Nsrc = 0;
};




#endif //INTERPOLATOR2D_H
