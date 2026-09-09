#include <iostream>
#include <cassert>
#include <cmath>
#include "../include/Cayley.hpp"

using namespace std; 

int main() {
    const int N = 10;
    Cayley_Graph model(N);

    int id0 = model.indx(0, 0);
    int id1 = model.indx(0, 1);
    int id_negativo = model.indx(-1, 1);
    assert(id0 >= 0 && id0 < 2 * N);
    assert(id1 >= 0 && id1 < 2 * N);
    assert(id_negativo >= 0 && id_negativo < 2 * N);


    State psi_base = model.initial_state(0, 1);
    assert(psi_base.size() == 2 * N);
    cout << "base state norm: " << psi_base.norm() << "\n";
    assert(abs(psi_base.norm() - 1.0) < 1e-12);


    State psi_superposition = model.initial_superposition(0);
    assert(psi_superposition.size() == 2 * N);
    cout << "superposicion norm: " << psi_superposition.norm() << "\n";
    assert(abs(psi_superposition.norm() - 1.0) < 1e-12);

    cout << "\n All good.\n";
    return 0;
}