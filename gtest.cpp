#include <gtest/gtest.h>
#include "Interpolator2D.h"

int main(int argc, char **argv) {
    testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}

TEST(Parser, TestParser) {
    std::stringstream s_input;
    s_input << "1 1 2 2 3 3" << std::endl <<
               "1 1 2 2 3 3" << std::endl <<
               "2 2 4 4 6 6" << std::endl <<
               "2 2 4 4 6 6" << std::endl <<
               "1 1 2 2 0 3" << std::endl <<
               "1 1 2 2 4 4" << std::endl;

    static std::vector<std::vector<double>> data = Parser::get_data(s_input);


    double expected_data[6][6] = {{1, 1, 2, 2, 3, 3},
                                    {1, 1, 2, 2, 3, 3},
                                    {2, 2, 4, 4, 6, 6},
                                    {2, 2, 4, 4, 6, 6},
                                    {1, 1, 2, 2, 0, 3},
                                    {1, 1, 2, 2, 4, 4}};

    for (int i = 0; i < 6; i++) {
        for (int j = 0; j < 6; j++) {
            EXPECT_EQ(expected_data[i][j], data[i][j]);
        }
    }
    s_input.clear();
}

TEST(Interpolator2D, DownScale) {
    std::stringstream s_input;
    s_input << "1 1 2 2 3 3" << std::endl <<
               "1 1 2 2 3 3" << std::endl <<
               "2 2 4 4 6 6" << std::endl <<
               "2 2 4 4 6 6" << std::endl <<
               "1 1 2 2 3 3" << std::endl <<
               "1 1 2 2 3 3" << std::endl;

    Interpolator2D ip {Parser::get_data(s_input)};
    ip.Bilinear(3);

    std::stringstream result;
    ip.save_result(result);

    std::string line;
    getline(result, line);
    EXPECT_EQ(line, "1 2 3 ");
    getline(result, line);
    EXPECT_EQ(line, "2 4 6 ");
    getline(result, line);
    EXPECT_EQ(line, "1 2 3 ");
}

TEST(Interpolator2D, UpScale) {
    std::stringstream s_input;
    s_input << "2 4" << std::endl <<
               "6 8" << std::endl;

    Interpolator2D ip {Parser::get_data(s_input)};
    ip.Bilinear(3);

    std::stringstream result;
    ip.save_result(result);

    std::string line;
    getline(result, line);
    EXPECT_EQ(line, "2 3 4 ");
    getline(result, line);
    EXPECT_EQ(line, "4 5 6 ");
    getline(result, line);
    EXPECT_EQ(line, "6 7 8 ");
    getline(result, line);
}

TEST(Interpolator2D, SameScale) {
    std::stringstream s_input;
    s_input << "1 2 3" << std::endl <<
               "1 1 1" << std::endl <<
               "3 2 1" << std::endl;

    Interpolator2D ip {Parser::get_data(s_input)};
    ip.Bilinear(3);

    std::stringstream result;
    ip.save_result(result);

    std::string line;
    getline(result, line);
    EXPECT_EQ(line, "1 2 3 ");
    getline(result, line);
    EXPECT_EQ(line, "1 1 1 ");
    getline(result, line);
    EXPECT_EQ(line, "3 2 1 ");
}