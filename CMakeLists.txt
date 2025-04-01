cmake_minimum_required(VERSION 3.30)
project(2DInterpolation)

set(CMAKE_CXX_STANDARD 17)

add_executable(2DInterpolation main.cpp
        Interpolator2D.cpp
        Interpolator2D.h
)
