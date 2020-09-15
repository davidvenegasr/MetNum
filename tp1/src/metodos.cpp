#include <iostream>
#include <iomanip>
#include <cmath>
#include "metodos.h"

void Elim_Gaussiana(vector<vector<double>> &augmentedMatrix, vector<double> &res);

void print_matrix(vector<vector<double>> &matrix, int width) {
    for (int i = 0; i < matrix.size(); ++i) {
        for (int j = 0; j < matrix[0].size(); ++j) {
            std::cout <<std::setw(width) <<matrix[i][j] <<" ";
        }
        std::cout <<endl;
    }
    std::cout <<endl;
}

bool test_matrix(vector<vector<double>> &M, vector<double> &X, double epsilon) {
    bool r_bool = true;
    vector<double> res(M.size(),0);
    for (int i = 0; i < M.size(); ++i) {
        for (int j = 0; j < M[0].size(); ++j) {
            res[i] += M[i][j]*X[j];
        }
    }
    for (int i = 0; i < M.size(); ++i) {
        std::cout <<std::setw(12) <<M[i][M.size()] <<" == " <<std::setw(12) <<res[i] <<" ?";
        if (fabs(res[i] - M[i][M.size()]) >= epsilon) {
            r_bool = false;
            std::cout <<"\t<-NO" << endl;
        } else {
            std::cout <<"\t<-SI" << endl;
        }
    }
    return r_bool;
}

vector<double>& WP(map<int,Equipo> Equipos, vector<double> &res){
    for(int i = 0; i < Equipos.size(); i++){
        double wp = (double) Equipos[i].p_ganados / (double)Equipos[i].p_totales();
        res.push_back(wp);
    }
    return res;
}

vector<double>& CMM(const map<int,Equipo> &Equipos, vector<double> &res){
    vector<vector<double>> C(Equipos.size(),vector<double>(Equipos.size()+1,0));
    // Llenamos la matriz C (aumentada) con sus datos correspondientes
    int i = 0;
    for (const auto &ei : Equipos) {
        int j = 0;
        auto &equipo_i = ei.second;
        for (const auto &ej : Equipos) {
            auto &equipo_j = ej.second;
            if (i == j) {
                C[i][j] = 2 + equipo_i.p_totales();
            } else {
                if (equipo_i.cant_matches_con.count(equipo_j.id)) {
                    C[i][j] = 0 - equipo_i.cant_matches_con.at(equipo_j.id);
                } else {
                    C[i][j] = 0;
                }
            }
            j++;
        }
        i++;
    }

    i = 0;
    for (const auto &ei : Equipos) {
        auto &equipo_i = ei.second;
        C[i][Equipos.size()] = 1 + (equipo_i.p_ganados - equipo_i.p_perdidos) / (double) 2;
        i++;
    }
    // Resolvemos el sistema
    Elim_Gaussiana(C,res);
    return res;
}


/*Funcion que triangula superiormente una matriz aumentada sin realizar permutaciones. */
void triangularSuperiormente(vector<vector<double>> &augmentedMatrix, int N){

	for (int k = 0; k < N; k++){

		for (int i = k+1; i < N; i++){
            if (augmentedMatrix[k][k] == 0){
                std::cout <<"ERROR"<<endl;
            }
			double factor = augmentedMatrix[i][k]/augmentedMatrix[k][k];
			
			for (int j = k; j < N+1 ; j++){
				augmentedMatrix[i][j] -= augmentedMatrix[k][j]*factor;
			}
		}	
	}
}

/*Funcion que dada una matriz aumentada triangular superior, resuelve el sistema asociado. */
vector<double>& backwardSubstitution(vector<vector<double>> &augmentedMatrix, int N, vector<double> &res){

	for (int i = N-1; i >= 0; i--){

		res[i] = augmentedMatrix[i][N];

		for (int j = i+1; j < N; j++){
			res[i] -= augmentedMatrix[i][j] * res[j];
		}

		res[i] = res[i]/augmentedMatrix[i][i];
	}

	return res;
}

/* Metodo de eliminación gaussiana para resolver el sistema A*X = B utilizando una matriz aumentada .*/
/* Como precondición Aii != 0 para todo 0 <=i < N en cada paso de la eliminación*/
/* Matriz de N filas y N+1 columnas(1 columna de más ya que la matriz fue aumentada)*/
void Elim_Gaussiana(vector<vector<double>> &augmentedMatrix, vector<double> &res){

	int N = augmentedMatrix.size();
    vector<vector<double>> copy_m (augmentedMatrix);

	triangularSuperiormente(augmentedMatrix, N);
    //print_matrix(augmentedMatrix,15);
	backwardSubstitution(augmentedMatrix, N, res);
    //print_matrix(augmentedMatrix,15);
    test_matrix(copy_m,res,0.0001);

}

//TODO: Chequear si hace falta devolver el vector por referencia si igualmente lo modificamos por referencia.


