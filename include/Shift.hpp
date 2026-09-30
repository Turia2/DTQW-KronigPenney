#pragma once
#include "Cayley.hpp"

class Shift{
    public:

        explicit Shift(const Cayley_Graph g, double peierls_phi = 0.0);
        SpMat build() const;
    
    private:
        
        const Cayley_Graph graph_;
        double phi_;

};