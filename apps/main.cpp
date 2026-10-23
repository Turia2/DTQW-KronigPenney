#include "Cayley.hpp"
#include "Coin.hpp"
#include "Shift.hpp"
#include "Walk.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>

int main(int argc, char* argv[]){

    int N = 200;
    int T = 5000; 
    double theta = M_PI/4.0;
    double B = M_PI/2.0;
    double phi = 0.0;
    int x0 = N/2;
    int save_every = 10; 
    std::string hdf5 = "data/simulation.h5";

    for(int i = 1; i < argc-1; i++){
        std::string a = argv[i];
        if(a == "--N"){
            N = std::stoi(argv[i]);

        } else if(a == "--T") {
            T = std::stoi(argv[i]);

        } else if(a == "--theta"){
            theta = std::stod(argv[i]);

        } else if(a == "--phi"){
            phi = std::stod(argv[i]);

        } else if(a == "--out"){
            hdf5 = argv[i];

        }
    }

    std::filesystem::path out(hdf5);
    if(out.has_parent_path()){
        std::filesystem::create_directories(out.parent_path());
    }

    Cayley_Graph graph(N);
    std::vector<std::pair<int, double>> zeeman = {{N/2, B}};
    Coin coin(graph, theta, zeeman);
    Shift shift(graph, phi);

    SpMat U = shift.build()*coin.build();
    
    State psi0 = graph.initial_superposition(x0);
    WalkParams params{theta, phi, B, x0};
    Walk walk(graph, U, psi0);
    walk.run(T, save_every, hdf5);
    std::cout<<"All good. Data in " << hdf5 << "\n";

    return 0;


}