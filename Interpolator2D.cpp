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

static int clampIndex(int i, int N) noexcept
{
    // реплицируем крайние узлы
    return (i < 0) ? 0 : (i >= N ? N-1 : i);
}

static double cubic(double p0, double p1, double p2, double p3, double t) {
    return p1 + 0.5 * t * (p2 - p0 + t * (2.0 * p0 - 5.0 * p1 + 4.0 * p2 - p3 + t * (3.0 * (p1 - p2) + p3 - p0)));
}

double Interpolator2D::biqubic(const std::vector<std::vector<double>> &input, double x, double y) {
    int N = static_cast<int>(input.size());
    //if (N < 4) throw std::runtime_error("Требуется минимум 4×4 узла");

    const int ix = static_cast<int>(std::floor(x));
    const int iy = static_cast<int>(std::floor(y));
    const double tx = x - ix;
    const double ty = y - iy;

    double col[4];

    for (int dx = -1; dx <= 2; ++dx)
    {
        double p[4];
        for (int dy = -1; dy <= 2; ++dy)
        {
            int sx = clampIndex(ix + dx, N);
            int sy = clampIndex(iy + dy, N);
            p[dy + 1] = input[sx][sy];
        }
        col[dx + 1] = cubic(p[0], p[1], p[2], p[3], ty);
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


void Interpolator2D::Interpolate(size_t n, InterpolationType type) {
    switch (type) {
        case InterpolationType::BILINEAR:
            InternalInterpolate(n, Interpolator2D::bilinear);
        break;
        case InterpolationType::BICUBIC:
            InternalInterpolate(n, Interpolator2D::biqubic);
        break;
    }
}

void Interpolator2D::InternalInterpolate(size_t n, double (*interpolate)(const vector<vector<double>>& input, double x, double y)) {
    if (input.empty()) throw std::runtime_error("ERROR Interpolator2D::Interpolate: input is empty");
    if (input.size() > n) throw std::runtime_error("ERROR Interpolator2D::Interpolate: Ndst must be > Nrsc");

    size_t N = input.size();
    output.resize(n);

    if (n == N) {
        output = input;
        return;
    }

    for (auto &v: output) {
        v.resize(n);
    }

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

