cmake_minimum_required(VERSION 3.6)

project(NuakeUIDemo)
include_directories(./NuakeUI/)
include_directories(./NuakeUI/Dependencies/NuakeRenderer/)

add_executable(NuakeUIDemo src/main.cpp)
target_compile_features(NuakeUIDemo PRIVATE cxx_std_17)
target_link_libraries(NuakeUIDemo PUBLIC NuakeUI)

add_subdirectory(./src/)
add_subdirectory(./NuakeUI/)




