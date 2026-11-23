cmake_minimum_required(VERSION 3.24)

project(GeneScope VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

option(GENESCOPE_BUILD_TESTS "Build GeneScope tests" ON)

find_package(Eigen3 3.3 REQUIRED NO_MODULE)

add_library(genescope_core
    src/data/dna_sequence.cpp
    src/data/splice_dataset.cpp
    src/features/one_hot_encoder.cpp
    src/features/kmer_extractor.cpp
    src/models/mlp.cpp
    src/models/training.cpp
    src/evaluation/metrics.cpp
)

target_include_directories(genescope_core PUBLIC include)
target_link_libraries(genescope_core PUBLIC Eigen3::Eigen)
target_compile_features(genescope_core PUBLIC cxx_std_20)

if(MSVC)
    target_compile_options(genescope_core PRIVATE /W4 /permissive-)
else()
    target_compile_options(genescope_core PRIVATE -Wall -Wextra -Wpedantic)
endif()

add_executable(genescope src/main.cpp)
target_link_libraries(genescope PRIVATE genescope_core)

if(GENESCOPE_BUILD_TESTS)
    include(CTest)
    add_executable(genescope_tests tests/test_dna_sequence.cpp)
    target_link_libraries(genescope_tests PRIVATE genescope_core)
    add_test(NAME genescope_tests COMMAND genescope_tests)
endif()
