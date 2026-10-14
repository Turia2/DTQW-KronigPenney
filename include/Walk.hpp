#pragma once
#include "Cayley.hpp"
#include <string>

struct WalkParams{

    double theta = 0.0;
    double phi = 0.0;
    double B = 0.0;
    int x0 = 0;

};


class Walk{

    public:

        Walk(Cayley_Graph graph, SpMat U_operator, State psi0, WalkParams params = {});
        void step();
        void run(int timesteps);
        void run(int timesteps, int save_every, const std::string& hdf5_path);
        const State& state() const{
            return psi_;
        }
        Eigen::VectorXd probabilities() const;
        double norm() const;

    private:

        Cayley_Graph graph_;
        SpMat U_;
        State psi_;
        WalkParams params_;

};

double mean_position(const Eigen::VectorXd& P, int x0);
double variance(const Eigen::VectorXd& P, int x0);
bool wrapped(const Eigen::VectorXd& P, int x0, double eps = 1e-8);

