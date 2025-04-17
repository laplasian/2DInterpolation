#include <gtest/gtest.h>
#include "Interpolator2D.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
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
    interpolator.Interpolate(3, BILINEAR);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, UpscaleWithBiqubic) {
    std::istringstream input("1 1 1 1\n2 2 2 2\n3 3 3 3\n4 4 4 4\n");
    std::vector<std::vector<double>> expected = {
        { 2, 2, 2, 2, 2.096, 2 },
        { 2, 2, 2, 2, 2.096, 2 },
        { 2.2, 2.2, 2.2, 2.2, 2.3056, 2.2 },
        { 2.8, 2.8, 2.8, 2.8, 2.9344, 2.8 },
        { 3.448, 3.448, 3.448, 3.448, 3.62272, 3.448 },
        { 4, 4, 4, 4, 4.192, 4 } };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(6, BICUBIC);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, Downscale) {
    std::istringstream input("1 1 1 1 1\n2 2 2 2 2\n3 3 3 3 3\n4 4 4 4 4\n5 5 5 5 5\n");

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(4, BICUBIC);
    EXPECT_THROW(Parser::get_data(input), std::runtime_error);
}

TEST(Interpolator2D, SameScale) {
    std::istringstream input("1 2\n3 4\n");
    std::vector<std::vector<double>> expected = {
        {1, 2},
        {3, 4}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(2, BILINEAR);
    EXPECT_EQ(interpolator.get_result(), expected);
}


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


TEST(Interpolator2D, InterpolateZeroSize) {
    std::istringstream input("1 2\n3 4\n");
    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(0, BILINEAR);
    EXPECT_TRUE(interpolator.get_result().empty());
}


