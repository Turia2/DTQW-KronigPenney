#pragma once
#include "Cayley.hpp"
#include <vector>
#include <utility>

class Lattice{

    public: 

        Lattice(const Cayley_Graph& g, int period, int width, double B, int offset = 0);
        int cells() const{
            return graph_.N/period_;
        }

        int period() const{
            return period_;
        }

        int width() const{
            return width_;
        }

        int offset() const{
            return offset_;
        }

        double B() const{
            return B_;
        }

        bool is_barrier(int x) const;
        std::vector<int> barrier_sites() const;
        std::vector<std::pair<int, double>> zeeman() const;

    private:
        
        const Cayley_Graph graph_;
        int period_;
        int width_;
        double B_;
        int offset_;
};