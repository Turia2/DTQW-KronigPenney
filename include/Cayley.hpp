#pragma once
#include <Eigen/Sparse>
#include <complex>
#include <cstdint>

using Scalar = std::complex<double>;
using State = Eigen::VectorX<Scalar>;
using SpMat = Eigen::SparseMatrix<Scalar>;

class Cayley{
    public:

        const int N; 
        explicit Cayley(int N):N(N){}

        int indx(int x, int s) const{ //convertir una posición x y un espín s en un índice único dentro del estado
            return 2*((x % N + N) + s); //garantiza que el índice sea siempre positivo (condiciones de contorno periódicas)
                //el espacio de Hilbert tiene dimensión 2N: N posiciones posibles y 2 espines (up y down)
        }
        
        State ini_state(int x0, int spin) const {
            State psi = State::Zero(2*N);
            psi(indx(x0, spin)) = Scalar(1.0, 0.0);
            return psi;
        }

        State ini_superp(int x0) const{
            static const Scalar inv2 = 1.0/std::sqrt(2.0);
            State psi = State::Zero(2*N);
            psi(indx(x0,0)) = inv2;
            psi(indx(x0, 1)) = Scalar(0.0, 1.0)*inv2;
            return psi;
        }
};

/*indx es una función de traducción de coordenadas que nos traduce las coordendas. 
Como convertir una dirección (calle, número) a un único número de apartamento.
Nos dice dónde vive el par (x,s) dentro del vector de memoria. Nada más, no te compliques.

Para relacionar a los putos nodos usamos la matriz SpMat, que básicamente son las flechas
del grafo que unen a los nodos. La matriz nos dirá que fulanito de tal (x,s) está conectado
con fulanito de cual (x+1,s), por ejemplo. 

Por tanto, entendemos el grafo de Cayley como un grafo que nos sopla cómo es la estructura
del grupo en base al generador, que es un elemento a partir del cual combinándolo podemos
obtener todos los demás elementos. Luego, en nuestro código, el nodo no es más que el elemento
del grupo, mientras que el espín no es más que un grado de libertad interno de cada nodo para
modelar la física cuántica. Es decir, que la función indx simplemente nos clasifica los estados
posibles del sistema. Si el grafo de Cayley es el esqueleto, entonces el espín es la carne extra
que le ponemos encima. 

Conclusión: cada posición del vector State corresponde a un nodo del grafo más su espín, los 
cuales hemos empaquetado usando indx. 

*/