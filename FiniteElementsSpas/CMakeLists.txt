cmake_minimum_required(VERSION 3.0.0)
project(FiniteElementsSpas VERSION 1.0.0)

add_executable(FiniteElementsSpas src/config.h src/main.cpp)

target_include_directories(FiniteElementsSpas Private dependencies)
