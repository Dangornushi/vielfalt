cmake_minimum_required(VERSION 3.13)
project(vielfalt CXX)
add_executable(vielfalt
	main.cpp
	run.cpp
	vielfalt.hpp
)

target_compile_features(vielfalt PUBLIC cxx_std_20)

