project(biquad-optimized)
cmake_minimum_required(VERSION 3.1)


set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Wall")

add_executable(biquad-optimized biquadOptimized-paramcheck.cpp)
