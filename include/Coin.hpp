#pragma once
#include "Cayley.hpp"
#include <Eigen/Sparse>
#include <vector>
#include <utility>

class Coin{

    public:
        const Cayley_Graph& graph;
        Coin(const Cayley_Graph& g, double theta_uniform, std::vector<std::pair<int, double>> zeeman = {});
        SpMat build() const;

    private:
        double theta_; 
        std::vector<std::pair<int,double>> zeeman_;

};