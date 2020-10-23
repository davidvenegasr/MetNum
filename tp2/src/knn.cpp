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
	Matrix sub(X.rows(), X.cols());
    for (int i = 0; i < sub.rows(); i++) {
        sub.row(i) = vec;
    }

	Matrix aux(X.rows(), X.cols());
    aux = X - sub;

    // Termino de hacer distancia euclideana.
    Vector dis(X.rows());
    for (int i = 0; i < X.rows(); i++) {
        dis(i) = aux.row(i).squaredNorm();
    }

    // Ordeno digitos por distancia.
    vector<int> ind(dis.size());
   	iota(ind.begin(), ind.end(), 0);
	sort(ind.begin(), ind.end(), [&dis](size_t i1, size_t i2) {return dis(i1) < dis(i2);});
    ind.resize(k);  //me quedo solo con los mas cercanos

    // Lleno el vector res con los tags pertinentes a los k cercanos.
    Vector res(k);
	for (unsigned int i = 0; i < k; i++) {
        res(i) = y(ind[i]);
    }

    // Contamos la cantidad de apariciones de cada tag entre los k cercanos.
    Vector coun(res.size());
    for (int i = 0; i < coun.size(); i++) {
    	coun(i) = 0;
    	for (int j = 0; j < res.size(); j++) {
    	    if (res(i) == res(j)) {
    	        coun(i)++;
    	    }
    	}
 	}

    // Hallamos el maximo de coun.
    double max = -1;
 	int index = -1;
    for (int i = 0; i < coun.count(); i++) {
    	if (coun(i) > max) {
       		max = coun(i);
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
