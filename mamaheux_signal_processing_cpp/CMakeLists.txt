cmake_minimum_required(VERSION 3.5)
project(mamaheux_signal_processing_cpp)

find_package(pybind11 REQUIRED)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-march=native -ffast-math")

pybind11_add_module(mamaheux_signal_processing_cpp src/main.cpp src/SignalStatistics.cpp src/Effects.cpp)

# EXAMPLE_VERSION_INFO is defined by setup.py and passed into the C++ code as a
# define (VERSION_INFO) here.
target_compile_definitions(mamaheux_signal_processing_cpp PRIVATE VERSION_INFO=${EXAMPLE_VERSION_INFO})
