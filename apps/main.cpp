#include "Cayley.hpp"
#include "Coin.hpp"
#include "Shift.hpp"
#include "Walk.hpp"
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>
#include <filesystem>
#include <cmath>
#include "Lattice.hpp"


int main(int argc, char* argv[]){

    int N = 200;
    int T = 5000; 
    double theta = M_PI/4.0;
    double B = M_PI/2.0;
    double phi = 0.0;
    int x0 = -1;
    int save_every = 10; 
    int L = 10;
    int w = 2;
    int offset = 0;
    std::string hdf5 = "data/simulation.h5";

    for(int i = 1; i < argc-1; i++){
        std::string a = argv[i];
        if(a == "--N"){
            N = std::stoi(argv[++i]);

        } else if(a == "--T") {
            T = std::stoi(argv[++i]);

        } else if(a == "--theta"){
            theta = std::stod(argv[++i]);

        } else if(a == "--phi"){
            phi = std::stod(argv[++i]);

        } else if(a == "--out"){
            hdf5 = argv[++i];

        } else if(a == "--B"){
            B = std::stod(argv[++i]);

        } else if(a == "--x0"){
            x0 = std::stoi(argv[++i]);
        
        } else if(a == "--save_every"){
            save_every = std::stoi(argv[++i]);

        } else if(a == "--L"){
            L = std::stoi(argv[++i]);

        } else if(a == "--w"){
            w = std::stoi(argv[++i]);

        } else if(a == "--offset"){
            offset = std::stoi(argv[++i]);

        } else {
            throw std::invalid_argument("Unknown argument: " + a);
        }
    }
    
    if(x0 < 0 ){
        x0 = N/2;
    }

    if(N <= 0){
        throw std::invalid_argument("N must be positive");
    }

    if(T <= 0){
        throw std::invalid_argument("T must be positive");
    }

    if(save_every <= 0){
        throw std::invalid_argument("save_every must be positive");
    }

    if(x0 >= N){
        throw std::invalid_argument("x0 must be in [0, N)");
    }

    std::filesystem::path out(hdf5);
    if(out.has_parent_path()){
        std::filesystem::create_directories(out.parent_path());
    }

    Cayley_Graph graph(N);
    Lattice lattice(graph, L, w, B, offset);
    Coin coin(graph, theta, lattice.zeeman());
    Shift shift(graph, phi);

    SpMat U = shift.build()*coin.build();
    
    State psi0 = graph.initial_superposition(x0);
    WalkParams params{theta, phi, B, x0, lattice.period(), lattice.width(), lattice.offset()};
    Walk walk(graph, U, psi0, params);
    walk.run(T, save_every, hdf5);
    std::cout<<"All good. Data in " << hdf5 << "\n";

    return 0;


}