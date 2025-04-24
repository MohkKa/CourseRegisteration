cmake_minimum_required(VERSION 3.30)
project(Project)

set(CMAKE_CXX_STANDARD 20)

add_executable(Project main.cpp Admin.cpp Course.cpp Student.cpp System_Manager.cpp)
