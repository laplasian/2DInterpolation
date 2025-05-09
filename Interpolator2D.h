#ifndef INTERPOLATOR2D_H
#define INTERPOLATOR2D_H
#include <string>
#include <vector>
#include <iostream>

#define max_line_size 256

class Parser {
public:
    Parser() = delete;
    static std::vector<std::vector<double>> get_data(std::istream& stream);
private:
    static std::vector<double> get_next(const std::string& line);
};

enum class InterpolationType {
    BILINEAR,
    BICUBIC
};

class Interpolator2D {
public:
    explicit Interpolator2D(std::vector<std::vector<double>>&& in) : input(std::move(in)) {} // move semantic
    ~Interpolator2D();

    void Interpolate(size_t n, InterpolationType type);
    const std::vector<std::vector<double>>& get_result() const;

private:
    void InternalInterpolate(size_t n, double (*interpolate)(const std::vector<std::vector<double>>& input, double x, double y));

    static double bilinear(const std::vector<std::vector<double>>& input, double x, double y);
    static double biqubic(const std::vector<std::vector<double>>& input, double x, double y);

    std::vector<std::vector<double>> input;
    std::vector<std::vector<double>> output = {};
};

void save_result(std::ostream& stream, const std::vector<std::vector<double>> & data);

#endif //INTERPOLATOR2D_H
