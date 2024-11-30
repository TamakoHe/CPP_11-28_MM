cmake_minimum_required(VERSION 3.29)
project(JSON)

set(CMAKE_CXX_STANDARD 20)

add_executable(JSON main.cpp kd_tree.h)
add_executable(Test test.cpp kd_tree.h)
add_subdirectory(json_lib)
target_link_libraries(JSON nlohmann_json)
