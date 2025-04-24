#include <cmath>
#include <gtest/gtest.h>

#include "expects.h"
#include "Interpolator2D.h"

int main(int argc, char **argv) {
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


static bool operator==(std::vector<std::vector<double>> a, std::vector<std::vector<double>> b) {
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
    EXPECT_EQ(interpolator.get_result(), expected);
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
    EXPECT_EQ(interpolator.get_result(), expected);
}

TEST(Interpolator2D, Upscale_2x_3x_4x_small_data) {
    std::istringstream input("0 10\n20 30\n");

    Interpolator2D interpolator(Parser::get_data(input));

    interpolator.Interpolate(4, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected2x_bilinear);
    interpolator.Interpolate(6, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected3x_bilinear);
    interpolator.Interpolate(8, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected4x_bilinear);

    interpolator.Interpolate(4, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected2x_biqubic);
    interpolator.Interpolate(6, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result()== expects_small_data::expected3x_biqubic);
    interpolator.Interpolate(8, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_small_data::expected4x_biqubic);
}

TEST(Interpolator2D, Upscale_2x_3x_large_data) {
    std::istringstream input(
        "50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50 50\n"
                "62 61 60 59 56 53 50 47 44 42 40 38 37 37 38 39 41 44 47 50\n"
                "73 73 71 67 62 57 51 45 40 34 30 27 26 26 27 30 34 39 44 50\n"
                "84 83 79 74 68 60 52 43 35 28 22 18 16 16 18 22 27 34 42 51\n"
                "92 90 86 80 72 63 52 42 32 23 16 11 8 8 10 15 22 31 41 51\n"
                "97 95 91 84 75 64 53 41 30 20 11 6 3 2 5 11 18 28 39 51\n"
                "99 98 93 86 76 65 53 41 29 18 10 3 0 0 3 9 17 27 39 51\n"
                "99 97 93 85 76 65 53 41 29 19 10 4 1 1 3 9 17 28 39 51\n"
                "95 94 89 83 74 64 53 41 31 21 13 7 4 4 7 12 20 29 40 51\n"
                "88 87 84 78 71 62 52 43 33 25 18 14 11 11 13 18 24 32 41 51\n"
                "79 78 76 71 66 59 52 44 37 31 26 22 20 20 21 25 30 36 43 51\n"
                "69 68 66 63 60 56 51 46 42 38 34 32 31 31 32 34 37 41 45 50\n"
                "57 56 56 55 53 52 50 48 47 45 44 43 43 42 43 44 45 46 48 50\n"
                "44 44 45 46 47 48 49 50 52 53 54 55 55 55 55 54 53 52 51 49\n"
                "32 33 34 37 40 44 48 53 57 61 64 66 67 67 66 64 61 57 53 49\n"
                "21 22 24 29 34 40 47 55 61 67 72 76 78 78 76 73 68 62 56 48\n"
                "12 13 16 22 29 38 47 56 65 73 80 84 87 87 85 81 74 66 57 48\n"
                "3 5 10 18 28 39 51 62 73 82 89 94 96 95 91 85 76 65 53 47\n"
                "4 2 4 12 22 33 45 56 67 76 83 88 90 89 85 78 69 58 46 46\n"
                "9 6 0 8 17 28 39 50 60 70 77 82 84 82 78 71 62 52 44 45\n"
                );

    Interpolator2D interpolator(Parser::get_data(input));

    interpolator.Interpolate(40, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_large_data::expected2x_bilinear);
    interpolator.Interpolate(60, InterpolationType::BILINEAR);
    EXPECT_TRUE(interpolator.get_result() == expects_large_data::expected3x_bilinear);

    interpolator.Interpolate(40, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result() == expects_large_data::expected2x_biqubic);
    interpolator.Interpolate(60, InterpolationType::BICUBIC);
    EXPECT_TRUE(interpolator.get_result()== expects_large_data::expected3x_biqubic);

    // for (auto &v: interpolator.get_result()) {
    //     std::cout << "{ ";
    //     for (auto &w: v) {
    //         std::cout << std::round(w*100)/100 << ", ";
    //     }
    //     std::cout << " }," << std::endl;
    // }
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




