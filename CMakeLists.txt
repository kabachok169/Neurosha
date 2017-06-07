cmake_minimum_required(VERSION 3.7)
project(NeuralNetwork)

set(CMAKE_CXX_STANDARD 14)


set(BOOST_ROOT "/usr/local/include/boost")
find_package(Boost REQUIRED filesystem)
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SOURCE_FILES main.cpp
        neurosha/vector.hpp
        neurosha/head.h
        neurosha/exception.hpp
        neurosha/functions.hpp)

add_executable(NeuralNetwork ${SOURCE_FILES})
target_link_libraries(NeuralNetwork ${Boost_LIBRARIES})