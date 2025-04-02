cmake_minimum_required(VERSION 3.30)
project(2DInterpolation)

set(CMAKE_CXX_STANDARD 17)

#include(FetchContent)
#
#FetchContent_Declare(
#        opencv
#        GIT_REPOSITORY https://github.com/opencv/opencv.git
#        GIT_TAG master
#)
#
#FetchContent_MakeAvailable(opencv)

add_executable(2DInterpolation main.cpp
        Interpolator2D.cpp
        Interpolator2D.h
)
#target_link_libraries(2DInterpolation PRIVATE opencv_core imgproc imgcodecs)

#add_library(2DInterpolation_static STATIC  Interpolator2D.cpp)
#add_library(2DInterpolation_shared SHARED  Interpolator2D.cpp)
#
#target_include_directories(2DInterpolation_static PUBLIC ${CMAKE_SOURCE_DIR})
#target_include_directories(2DInterpolation_shared PUBLIC ${CMAKE_SOURCE_DIR})
#
#install(TARGETS 2DInterpolation_static 2DInterpolation_shared
#        ARCHIVE DESTINATION lib
#        LIBRARY DESTINATION lib
#        RUNTIME DESTINATION bin
#)
#install(FILES Interpolator2D.h DESTINATION include)
