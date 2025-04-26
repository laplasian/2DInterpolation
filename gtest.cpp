#include <cmath>
#include <gtest/gtest.h>

#include "expects.h"
#include "Interpolator2D.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

#define ESP 10e-5

static bool operator==(const std::vector<std::vector<double>> &a, const std::vector<std::vector<double>> &b) {
    for (int i = 0; i < a.size(); i++) {
        for (int j = 0; j < b.size(); j++) {
            if (std::abs(a[i][j] - b[i][j]) > ESP) {
                return false;
            }
        }
    }
    return true;
}

// TEST Parser

TEST(Parser, ValidGrid) {
    std::istringstream input("1 2 3\n4 5 6\n7 8 9\n");
    std::vector<std::vector<double>> expected = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };
    EXPECT_EQ(Parser::get_data(input), expected);
}

TEST(Parser, EmptyInput) {
    std::istringstream input("");
    EXPECT_THROW(Parser::get_data(input), std::runtime_error);
}

TEST(Parser, MismatchedRowSizesThrows) {
    std::istringstream input("1 2\n3 4 5\n");
    EXPECT_THROW(Parser::get_data(input), std::runtime_error);
}

TEST(Parser, InvalidGrid) {
    std::istringstream input("1 a 3\n4 5 6\n7 b 9\n");
    EXPECT_THROW(Parser::get_data(input), std::runtime_error);
}

// TEST Interpolator

TEST(Interpolator2D, UpscaleWithBilinear) {
    std::istringstream input("0 10\n20 30\n");
    std::vector<std::vector<double>> expected = {
        {0, 5, 10},
        {10, 15, 20},
        {20, 25, 30}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(3, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expected);
}

TEST(Interpolator2D, UpscaleWithBiqubic) {
    std::istringstream input("1 1 1 1\n2 2 2 2\n3 3 3 3\n4 4 4 4\n");
    std::vector<std::vector<double>> expected = {
        { 1, 1, 1, 1, 1, 1 },
        { 1.552, 1.552, 1.552, 1.552, 1.552, 1.552 },
        { 2.2, 2.2, 2.2, 2.2, 2.2, 2.2 },
        { 2.8, 2.8, 2.8, 2.8, 2.8, 2.8 },
        { 3.448, 3.448, 3.448, 3.448, 3.448, 3.448 },
        { 4, 4, 4, 4, 4, 4 }
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(6, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expected);
}

TEST(Interpolator2D, UpscaleVerticalWithBilinear) {
    std::istringstream input(
    "1 0 0 0 \n"
        "1 0 0 0\n"
        "1 0 0 0\n"
        "1 0 0 0\n"
        );

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(9, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_bilinear::expected_vertical);
}

TEST(Interpolator2D, UpscaleVerticalWithBicubic) {
    std::istringstream input(
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        "0 0 0 0 0 1 0 0 0 0 0\n"
        );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(13, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_bicubic::expected_vertical);
}

TEST(Interpolator2D, UpscaleHorizontalWithBilinear) {
    std::istringstream input(
    "0 0 0 0 \n"
        "0 0 0 0\n"
        "1 1 2 2\n"
        "0 0 0 0\n"
        );

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(7, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_bilinear::expected_horizontal);
}

TEST(Interpolator2D, UpscaleHorizontalWithBicubic) {
    std::istringstream input(
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "1 1 2 2 2 2 2 2 2 1 1\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(13, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_bicubic::expected_horizontal);
}

TEST(Interpolator2D, UpscaleReactangleWithBilinear) {
    std::istringstream input(
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 1 1 1 1 1 1 0 0\n"
    "0 0 0 1 0 0 0 0 1 0 0\n"
    "0 0 0 1 0 0 0 0 1 0 0\n"
    "0 0 0 1 0 0 0 0 1 0 0\n"
    "0 0 0 1 1 1 1 1 1 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(17, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_bilinear::expected_rect);
    // for (auto &v: interpolator.get_result()) {
    //     std::cout << "{ ";
    //     for (auto &w: v) {
    //         std::cout << std::round(w*1000000)/1000000 << ", ";
    //     }
    //     std::cout << " }," << std::endl;
    // }
}

TEST(Interpolator2D, UpscaleReactangleWithBicubic) {
    std::istringstream input(
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 1 1 1 1 1 0 0 0\n"
    "0 0 0 1 0 0 0 1 0 0 0\n"
    "0 0 0 1 0 0 0 1 0 0 0\n"
    "0 0 0 1 0 0 0 1 0 0 0\n"
    "0 0 0 1 1 1 1 1 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    "0 0 0 0 0 0 0 0 0 0 0\n"
    );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(22, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_bicubic::expected_rect);
}

TEST(Interpolator2D, UpscalePointWithBilinear) {
    std::istringstream input(
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 10 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
    );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(17, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_bilinear::expected_point);
}

TEST(Interpolator2D, UpscalePointWithBicubic) {
    std::istringstream input(
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 10 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
        "0 0 0 0 0 0 0 0 0 0 0\n"
    );
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(13, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_bicubic::expected_point);
}

TEST(Interpolator2D, Upscale_2x_3x_small_data) {
    std::istringstream input("0 10\n20 30\n");

    Interpolator2D interpolator(Parser::get_data(input));

    interpolator.Interpolate(4, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected2x_bilinear);
    interpolator.Interpolate(6, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected3x_bilinear);

    interpolator.Interpolate(4, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected2x_biqubic);
    interpolator.Interpolate(6, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result()== expects_small_data::expected3x_biqubic);
}

TEST(Interpolator2D, Downscale) {
    std::istringstream input("1 1 1 1 1\n2 2 2 2 2\n3 3 3 3 3\n4 4 4 4 4\n5 5 5 5 5\n");

    Interpolator2D interpolator(Parser::get_data(input));
    EXPECT_THROW(interpolator.Interpolate(4, InterpolationType::BICUBIC);, std::runtime_error);
}

TEST(Interpolator2D, SameScale) {
    std::istringstream input("1 2\n3 4\n");
    std::vector<std::vector<double>> expected = {
        {1, 2},
        {3, 4}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(2, InterpolationType::BILINEAR);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, InterpolateZeroSize) {
    std::istringstream input("1 2\n3 4\n");
    Interpolator2D interpolator(Parser::get_data(input));
    EXPECT_THROW(interpolator.Interpolate(0, InterpolationType::BILINEAR);, std::runtime_error);
}

TEST(Interpolator2D, WrongType) {
    std::istringstream input("1 2\n3 4\n");
    Interpolator2D interpolator(Parser::get_data(input));
    EXPECT_THROW(interpolator.Interpolate(0, static_cast<InterpolationType>(5));, std::runtime_error);
}

// TEST SaveResult

TEST(SaveResult, OutputMatchesExpected) {
    std::vector<std::vector<double>> data = {{1.1, 2.2}, {3.3, 4.4}};
    std::ostringstream out;
    save_result(out, data);

    std::string expected = "1.1 2.2 \n3.3 4.4 \n";
    EXPECT_EQ(out.str(), expected);
}

TEST(SaveResult, BadStreamThrows) {
    std::vector<std::vector<double>> data = {{1}};
    std::ostringstream out;
    out.setstate(std::ios::badbit);
    EXPECT_THROW(save_result(out, data), std::runtime_error);
}




