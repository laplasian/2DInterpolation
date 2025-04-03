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

    std::vector<std::vector<double>> output = {};
    size_t padding = 0;
    double pad_fill = 0;

    void Bilinear(size_t Ndst);
    void Bicubic();

    void save_result(const std::string& filename);

    std::vector<std::vector<double>> input= {};
    size_t Nsrc = 0;

};

#endif //INTERPOLATOR2D_H
