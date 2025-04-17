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
    std::vector<std::vector<double>> expected = {};
    EXPECT_EQ(Parser::get_data(input), expected);
}

TEST(Parser, MismatchedRowSizesThrows) {
    std::istringstream input("1 2\n3 4 5\n");
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
    interpolator.Interpolate(3, Interpolator2D::bilinear);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, UpscaleWithBiqubic) {
    std::istringstream input("1 1 1 1\n1 1 1 1\n1 1 1 1 \n1 1 1 1\n");
    std::vector<std::vector<double>> expected = {
        {1, 1, 1},
        {1, 1, 1},
        {1, 1, 1}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(3, Interpolator2D::biqubic);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, DownscaleWithAreaAverage) {
    std::istringstream input("1 2 3 4\n5 6 7 8\n9 10 11 12\n13 14 15 16\n");
    std::vector<std::vector<double>> expected = {
        {3.5, 5.5},
        {11.5, 13.5}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(2, Interpolator2D::bilinear);
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, SameScale) {
    std::istringstream input("1 2\n3 4\n");
    std::vector<std::vector<double>> expected = {
        {1, 2},
        {3, 4}
    };

    Interpolator2D interpolator(Parser::get_data(input));
    interpolator.Interpolate(2, Interpolator2D::bilinear);
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
    interpolator.Interpolate(0, Interpolator2D::bilinear);
    EXPECT_TRUE(interpolator.get_result().empty());
}


