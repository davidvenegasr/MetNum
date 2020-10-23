#pragma once

#include "types.h"


class KNNClassifier {
public:
    KNNClassifier(unsigned int n_neighbors);
    void fit(Matrix X, Matrix y);
    Vector predict(Matrix X);
private:
	unsigned int k;
	Matrix X;
	Vector y;
	double predictAux(const Vector& vec);
};
