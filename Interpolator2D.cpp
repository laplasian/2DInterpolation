#include "Interpolator2D.h"

#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <ostream>
#include <sstream>


using namespace std;

// Функция для билинейной интерполяции одного значения
double bilinearInterpolate(const vector<vector<double>>& input,
                             double x, double y)
{
    int N = input.size();

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

vector<vector<double>> AreaAveraging(const vector<vector<double>>& input, vector<vector<double>>& output, int n) {
    int N = input.size();
    double ratio = static_cast<double>(N) / n; // отношение размеров

    for (int i = 0; i < n; ++i) {
        // Границы области по x во входном массиве для пикселя i выходного массива
        double x_start = i * ratio;
        double x_end = (i + 1) * ratio;

        for (int j = 0; j < n; ++j) {
            // Границы области по y во входном массиве
            double y_start = j * ratio;
            double y_end = (j + 1) * ratio;

            double sum = 0.0;
            double totalArea = ratio * ratio; // общая площадь области

            // Определяем диапазон входных индексов, затронутых этой областью
            int p0 = floor(x_start);
            int p1 = min(N - 1, static_cast<int>(ceil(x_end)));
            int q0 = floor(y_start);
            int q1 = min(N - 1, static_cast<int>(ceil(y_end)));

            // Проходим по всем входным пикселям, которые частично или полностью попадают в область
            for (int p = p0; p < p1; ++p) {
                // Вычисляем перекрытие по x
                double overlapX = max(0.0, min(x_end, static_cast<double>(p + 1)) - max(x_start, static_cast<double>(p)));
                for (int q = q0; q < q1; ++q) {
                    // Вычисляем перекрытие по y
                    double overlapY = max(0.0, min(y_end, static_cast<double>(q + 1)) - max(y_start, static_cast<double>(q)));
                    double weight = overlapX * overlapY;
                    sum += input[p][q] * weight;
                }
            }

            output[i][j] = sum / totalArea;
        }
    }

    return output;
}

std::vector<std::vector<double>> Parser::get_data(std::ifstream &stream) {
    if (!stream.is_open()) {
        std::cerr << "Parser got wrong stream" << std::endl;
    }
    std::string line;
    std::vector<std::vector<double>> input;

    while (std::getline(stream, line)) {
        std::istringstream iss(line);
        double num;
        std::vector<double> buff {};
        while (iss >> num) {
            buff.emplace_back(num);
        }
        input.emplace_back(buff);
    }
    // Проверка размеров сетки
    int Nsrc = input.size();

    for (auto &v : input) {
        if (v.size() != Nsrc) {
            throw std::runtime_error("ERROR! wrong grid dimension: " + std::to_string(Nsrc) + " != " + std::to_string(v.size()));
        }
    }

    return input;

}

Interpolator2D::~Interpolator2D() = default;

void Interpolator2D::Bilinear(size_t n) {
    if (n == 0) {
        return;
    }
    output.resize(n);
    for (auto &v: output) {
        v.resize(n);
    }

    double N = input.size();

    if (n > N) {
        // Вычисляем масштаб (с учетом индексации от 0)
        double scale = static_cast<double>(N - 1) / (n - 1);
        for (int i = 0; i < n; ++i) {
            for (int j = 0; j < n; ++j) {
                // Сопоставляем координаты в выходном массиве с координатами в исходном
                double x = i * scale;
                double y = j * scale;

                // Выполняем билинейную интерполяцию для получения значения
                output[i][j] = bilinearInterpolate(input, x, y);
            }
        }
    } else if (n < N) {
        AreaAveraging(input, output, n);
    } else if (n == N) {
        output = input;
    }
}

void Interpolator2D::Bicubic(size_t Ndst) {
}

void Interpolator2D::save_result(std::ofstream &stream) {
    if (!stream.is_open()) {
        throw std::runtime_error("ERROR! can`t save file:");
    }
    for (auto &v : output) {
        for (auto &w : v) {
            stream << w << " ";
        }
        stream << std::endl;
    }
    stream.close();
}

