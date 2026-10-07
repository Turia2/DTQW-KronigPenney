cmake_minimum_required (VERSION 3.15)
project(DTQW_KronigPenney LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release) 
endif()

find_package(Eigen3 3.4 REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

add_subdirectory(vendor/HighFive)

enable_testing()

add_library(dtqw src/Coin.cpp src/Shift.cpp)
target_include_directories(dtqw PUBLIC include)
target_link_libraries(dtqw PUBLIC Eigen3::Eigen)

foreach(test_target IN ITEMS test_cayley test_coin test_shift)
    add_executable(${test_target} tests/${test_target}.cpp)
    target_link_libraries(${test_target} PRIVATE dtqw)
    target_compile_options(${test_target} PRIVATE -UNDEBUG)
    add_test(NAME ${test_target} COMMAND ${test_target})
endforeach()