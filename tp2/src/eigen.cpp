#include <algorithm>
#include <chrono>
#include <iostream>
#include "eigen.h"

using namespace std;


pair<double, Vector> power_iteration(const Matrix& X, unsigned num_iter, double eps)
{

    Vector eigenAnterior = Vector::Zero(X.cols());
    Vector eigenActual = Vector::Random(X.cols());

    double eigenvalue;

    // Realizo la cantidad de iteraciones ingresadas por parámetro, o hasta que converger al autovector.
    while (num_iter > 0 && (eigenActual-eigenAnterior).isZero(eps)){

        eigenAnterior = eigenActual;
        eigenActual = X * eigenActual;
        eigenActual = eigenActual / eigenActual.norm();

        num_iter--;
    }

    // Calculo el autovalor usando el autovector
    eigenvalue = eigenAnterior.transpose().dot(X * eigenAnterior) / eigenAnterior.norm();

    return make_pair(eigenvalue, eigenAnterior);
}

pair<Vector, Matrix> get_first_eigenvalues(const Matrix& X, unsigned num, unsigned num_iter, double epsilon)
{
    Matrix A(X);
    Vector eigenValues(num);
    Matrix eigenVectors(A.rows(), num);
    Vector eigenVector;

    // Quiero un valor mayor al de epsilon para entrar al ciclo
    double eigenValue = epsilon + 1;

    unsigned int i = 0;

    while(i < num && eigenValue > epsilon) {
        std::pair<double, Vector> power_result = power_iteration(A, num_iter, epsilon);
        eigenValue = std::get<0>(power_result);
        eigenVector = std::get<1>(power_result);

        eigenValues(i) = eigenValue;
        eigenVectors.col(i) = eigenVector;
        A = A - eigenValue * eigenVector * eigenVector.transpose();

        i++;
    } 

    return make_pair(eigenValues, eigenVectors);
}
