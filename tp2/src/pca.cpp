#include <iostream>
#include "pca.h"
#include "eigen.h"

using namespace std;


PCA::PCA(unsigned int n_components)
{
	alpha = n_components;
}

Vector PCA::mean_vector(const Matrix& A) {
	unsigned int n = A.rows();
	Vector v(n);
	v.fill((double) 1 / (double) n);

	return A.transpose() * v;
}

Matrix PCA::covariance(Matrix A){
	int n = A.rows();
	Vector means = mean_vector(A);
	Vector aux;

	for (int i = 0; i < n; i++){
		aux = A.row(i);
		A.row(i) = (aux-means) / sqrt(n-1);
	}

	return A.transpose() * A;
}

void PCA::fit(Matrix X)
{
	Matrix C = covariance(X);

	std::pair<Vector, Matrix> eigens = get_first_eigenvalues(C, alpha);	
	Vector eigenValues = std::get<0>(eigens);
	Vector eigenVectors = std::get<1>(eigens);

	this->T = eigenVectors;
}


MatrixXd PCA::transform(Matrix X)
{
  return X*T;
}
