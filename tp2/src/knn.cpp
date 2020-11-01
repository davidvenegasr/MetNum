#include <algorithm>
//#include <chrono>
#include <iostream>
#include "knn.h"
#include <numeric>

using namespace std;


KNNClassifier::KNNClassifier(unsigned int n_neighbors)
{
	this->k = n_neighbors;
}

void KNNClassifier::fit(Matrix X, Matrix y)
{
	this->X = X;
	this->y = y;
}

double KNNClassifier::predictAux(const Vector& vec) {

	Matrix mat_a_restar(X.rows(), X.cols());

    // Realizo el calculo de la distancia euclideana.
    for (int i = 0; i < mat_a_restar.rows(); i++) {
        mat_a_restar.row(i) = vec;
    }

	Matrix mat_restada(X.rows(), X.cols());
    mat_restada = X - mat_a_restar;

    Vector distancia(X.rows());
    for (int i = 0; i < X.rows(); i++) {
        distancia(i) = mat_restada.row(i).squaredNorm();
    }

    // Selecciono los k digitos cuya distancia está mas cerca
    vector<int> indices(distancia.size());
   	iota(indices.begin(), indices.end(), 0);
	sort(indices.begin(), indices.end(), [&distancia](size_t i1, size_t i2) {return distancia(i1) < distancia(i2);});
    indices.resize(k);  

    // Asigno los valores correspondientes a cada indice
    Vector res(k);
	for (unsigned int i = 0; i < k; i++) {
        res(i) = y(indices[i]);
    }

    // Cuento la cantidad de apariciones de cada tag entre los k mas cercanos
    Vector cantidades(res.size());
    for (int i = 0; i < cantidades.size(); i++) {
    	cantidades(i) = 0;
    	for (int j = 0; j < res.size(); j++) {
    	    if (res(i) == res(j)) {
    	        cantidades(i)++;
    	    }
    	}
 	}

    // Hallamos el elemento mayor cantidad de apariciones
    double max = -1;
 	int index = -1;
    for (int i = 0; i < cantidades.count(); i++) {
    	if (cantidades(i) > max) {
       		max = cantidades(i);
       		index = i;
     	}
   }

   return res(index);
}


Vector KNNClassifier::predict(Matrix X)
{
    // Creamos vector columna a devolver
    auto ret = Vector(X.rows());

    for (unsigned k = 0; k < X.rows(); ++k)
    {
        ret(k) = predictAux(X.row(k));
    }

    return ret;
}
