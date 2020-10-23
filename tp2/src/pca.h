#pragma once
#include "types.h"

class PCA {
public:
    PCA(unsigned int n_components);
    void fit(Matrix X);
    Eigen::MatrixXd transform(Matrix X);
    Matrix covariance(Matrix A);
private:
	unsigned int alpha;
	Matrix T;
	Vector mean_vector(const Matrix& A);
};
