cmake_minimum_required(VERSION 3.30)
project(2DInterpolation)
project(2DInterpolation_test)

set(CMAKE_CXX_STANDARD 17)

# Подключаем Google Test через FetchContent
include(FetchContent)
FetchContent_Declare(
        googletest
        GIT_REPOSITORY https://github.com/google/googletest.git
        GIT_TAG v1.14.0
)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(2DInterpolation main.cpp
        Interpolator2D.cpp
        Interpolator2D.h
)

add_executable(2DInterpolation_test
        Interpolator2D.cpp
        Interpolator2D.h
        gtest.cpp
)

target_link_libraries(2DInterpolation_test GTest::gtest_main)

# Автоматически добавляем тесты
include(GoogleTest)
gtest_discover_tests(2DInterpolation_test)