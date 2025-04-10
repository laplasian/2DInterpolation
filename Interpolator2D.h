#ifndef INTERPOLATOR2D_H
#define INTERPOLATOR2D_H
#include <string>
#include <vector>
#include <iostream>

#define max_line_size 256

class Parser {
public:
    Parser() = default;
    ~Parser() = default;


    static std::vector<std::vector<double>> get_data(std::ifstream& stream);
    static std::vector<std::vector<double>> get_data(std::stringstream& stream);
    static std::vector<double> get_next(const std::string& line);

};

class Interpolator2D {
public:
    explicit Interpolator2D(std::vector<std::vector<double>>&& in) : input(std::move(in)) {} // move semantic
    ~Interpolator2D();

    void Bilinear(size_t Ndst);
    void Bicubic(size_t Ndst);

    void save_result(std::ofstream& stream);
    void save_result(std::stringstream& stream);
private:
    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> output = {};
};


#endif //INTERPOLATOR2D_H
