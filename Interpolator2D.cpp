#include "Interpolator2D.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>


using namespace std;

double Interpolator2D::bilinear(const std::vector<std::vector<double>> &input, double x, double y)
{
    int N = static_cast<int>(input.size());

    // Индексы узлов
    int x0 = floor(x);
    int y0 = floor(y);
    // Обеспечим, чтобы не выйти за границы массива
    int x1 = min(x0 + 1, N - 1);
    int y1 = min(y0 + 1, N - 1);

    // Дробные части
    double t = x - x0;
    double u = y - y0;

    // Получаем значения в узловых точках
    double f00 = input[x0][y0];
    double f10 = input[x1][y0];
    double f01 = input[x0][y1];
    double f11 = input[x1][y1];

    // Применяем формулу билинейной интерполяции
    double result = f00 * (1 - t) * (1 - u) +
                    f10 * t * (1 - u) +
                    f01 * (1 - t) * u +
                    f11 * t * u;
    return result;
}

static double cubic(double p0, double p1, double p2, double p3, double t) {
    return p1 + 0.5 * t * (p2 - p0 + t * (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 + t * (3.0 * (p1 - p2) + p3 - p0)));
}

double Interpolator2D::biqubic(const std::vector<std::vector<double>> &input, double x, double y) {
    int N = static_cast<int>(input.size());

    // Ограничиваем x и y, чтобы не выйти за допустимые границы
    x = std::max(1.0, std::min(x, static_cast<double>(N)));
    y = std::max(1.0, std::min(y, static_cast<double>(N)));

    int x1 = static_cast<int>(x);
    int y1 = static_cast<int>(y);
    double tx = x - x1;
    double ty = y - y1;

    double col[4];

    for (int i = -1; i <= 2; ++i) {
        double p0 = 0;
        double p1 = 0;
        double p2 = 0;
        double p3 = 0;
        if (x1 + i < N) {
            if (y1 - 1 < N) {
                p0 = input[x1 + i][y1 - 1];
            } else p0 = 0;
            if (y1 < N) {
                p1 = input[x1 + i][y1];
            } else p1 = 0;
            if (y1 + 1 < N) {
                p2 = input[x1 + i][y1 + 1];
            } else p2 = 0;
            if (y1 + 2 < N) {
                p3 = input[x1 + i][y1 + 2];
            } else p3 = 0;
            col[i + 1] = cubic(p0, p1, p2, p3, ty);
        }

    }

    return cubic(col[0], col[1], col[2], col[3], tx);
}

std::vector<std::vector<double>> Parser::get_data(std::istream &stream) {
    if (!stream.good()) {
        std::cerr << "Parser got wrong stream" << std::endl;
    }
    std::string line;
    std::vector<std::vector<double>> input;

    int Nsrc = 0;

    while (std::getline(stream, line)) {
        input.emplace_back(get_next(line));
        if (Nsrc != 0 && input.back().size() != Nsrc) throw std::runtime_error("ERROR! wrong grid dimension at line: " + std::to_string(input.size()));
        Nsrc = static_cast<int>(input.back().size());
    }

    if (Nsrc == 0 || input.back().size() != Nsrc) {
        throw std::runtime_error("ERROR! wrong grid dimension");
    }

    return input;
}

std::vector<double> Parser::get_next(const std::string& line) {
    std::istringstream iss(line);
    double num;
    std::vector<double> buff {};
    while (iss >> num) {
        buff.emplace_back(num);
    }
    return buff;
}

Interpolator2D::~Interpolator2D() = default;

void Interpolator2D::Interpolate(size_t n, size_t i_type) {
    InternalInterpolate(n, InterpolationFunction[i_type]);
}

void Interpolator2D::InternalInterpolate(size_t n, double (*interpolate)(const vector<vector<double>>& input, double x, double y)) {
    size_t N = input.size();

    if (n == 0 || n < N) {
        return;
    }

    output.resize(n);

    for (auto &v: output) {
        v.resize(n);
    }

    if (n > N) {
        // Вычисляем масштаб (с учетом индексации от 0)
        double scale = static_cast<double>(N - 1) / (static_cast<int>(n) - 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Сопоставляем координаты в выходном массиве с координатами в исходном
                double x = i * scale;
                double y = j * scale;

                // Выполняем интерполяцию для получения значения
                output[i][j] = interpolate(input, x, y);
            }
        }
    } else if (n == N) {
        output = input;
    }
}

const std::vector<std::vector<double>> & Interpolator2D::get_result() const {
    return output;
}

void save_result(std::ostream &stream, const std::vector<std::vector<double>> & data) {
    if (!stream.good()) {
        throw std::runtime_error("ERROR! can`t save file:");
    }
    for (auto &v : data) {
        for (auto &w : v) {
            stream << w << " ";
        }
        stream << std::endl;
    }
}

