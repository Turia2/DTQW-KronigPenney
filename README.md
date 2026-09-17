# DTQW-KronigPenney

## Abstract
This project aims to simulate a discrete time quantum walk (DTQW) in a Kronig-Penney model subjected to magnetic fields using unidimensional cyclical networks (Cayley graphs) and the Zeeman effect. 

This project is designed using C++ for the numerical core and it is planned to use python for the analysis and visualization.

## Status
This repository is currently in development with only one module, which is the Cayley graph and has already been tested. 


## Requirements
- Eigen3 (>=3.4)
- CMake (>=3.15)
- HDF5
- C++17. 

## Cloning
The library vendor/HighFive is submoduled. Please clone it using the command 
```bash
git clone --recurse-submodules https://github.com/Turia2/DTQW-KronigPenney
```

## Build
DTQW-KronigPenney must be compiled using the command 
```bash
cmake -B build && cmake --build build
```

## License
License: TBD
