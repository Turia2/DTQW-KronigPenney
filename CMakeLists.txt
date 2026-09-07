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

# Crear el ejecutable final a partir del codigo que escribiremos
add_executable(dtqw_sim
    src/main.cpp
    src/DTQWSolver.cpp
)

target_include_directories(dtqw_sim PRIVATE include)
target_link_libraries(dtqw_sim PRIVATE
    Eigen3::Eigen
    ${HDF5_LIBRARIES}
    HighFive
)
target_compile_options(dtqw_sim PRIVATE -O3 -march=native)