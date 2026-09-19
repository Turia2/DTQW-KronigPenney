cmake_minimum_required (VERSION 3.15)
project(DTQW_KronigPenney LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Optimizacion para que la simulacion vaya rapido
if (NOT CMAKE_BUILD_TYPE) 
    set(CMAKE_BUILD_TYPE Release) 
endif()

# Buscar dependencias instaladas en tu sistema
find_package(Eigen3 3.4 REQUIRED)
find_package(HDF5 REQUIRED COMPONENTS C CXX)

# Añadir la libreria de datos HighFive
add_subdirectory(vendor/HighFive)


add_executable(test_coin tests/test_coin.cpp src/Coin.cpp)
target_include_directories(test_coin PRIVATE include)
target_link_libraries(test_coin PRIVATE Eigen3::Eigen)

add_executable(test_cayley tests/test_cayley.cpp)
target_include_directories(test_cayley PRIVATE include)
target_link_libraries(test_cayley PRIVATE Eigen3::Eigen)