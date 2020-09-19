#include <iostream>
#include <iomanip>
#include <cmath>
#include "metodos.h"

void Elim_Gaussiana_Sin_Permutaciones(vector<vector<double>> &augmentedMatrix, vector<double> &res);
void Elim_Gaussiana_Con_Permutaciones(vector<vector<double>> &augmentedMatrix, vector<double> &res);

void print_matrix(vector<vector<double>> &matrix, int width) {
    for (uint32_t i = 0; i < matrix.size(); ++i) {
        for (uint32_t j = 0; j < matrix[0].size(); ++j) {
            std::cout <<std::setw(width) <<matrix[i][j] <<" ";
        }
        std::cout <<endl;
    }
    std::cout <<endl;
}

bool test_matrix(vector<vector<double>> &M, vector<double> &X, double epsilon) {
    bool r_bool = true;
    vector<double> res(M.size(),0);
    for (uint32_t i = 0; i < M.size(); ++i) {
        for (uint32_t j = 0; j < M[0].size(); ++j) {
            res[i] += M[i][j]*X[j];
        }
    }
    for (uint32_t i = 0; i < M.size(); ++i) {
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
    for(uint32_t i = 1; i <= Equipos.size(); i++){
        double wp = (double) Equipos[i].p_ganados / (double) Equipos[i].partidos_totales();
        //std::cout << "Valor de wp: " <<  wp<<std::endl;
        res.push_back(wp);
    }
    return res;
}

//TODO: Chequear si los ++i no increemntan las variables antes de evaluar los ciclos por primera vez.
//Lo dejo escrito para que lo charlemos.

vector<double>& ScoreRatio(map<int,Equipo> &Equipos, vector<double> &res){
	for(uint32_t i = 1; i <= Equipos.size(); i++){
		double score = (double) Equipos[i].diferencia_de_puntos / (double) Equipos[i].puntos_totales;
        res.push_back(score);
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
                C[i][j] = 2 + equipo_i.partidos_totales();
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
    Elim_Gaussiana_Sin_Permutaciones(C,res);
    return res;
}

vector<double>& Massey(const map<int,Equipo> &Equipos, vector<double> &res) {
    vector<vector<double>> M(Equipos.size(), vector<double>(Equipos.size() + 1, 0));
    // Llenamos la matriz M (aumentada) con sus datos correspondientes
    int i = 0;
    for (const auto &ei : Equipos) {
        int j = 0;
        auto &equipo_i = ei.second;
        for (const auto &ej : Equipos) {
            auto &equipo_j = ej.second;
            if (i == j) {
                M[i][j] = equipo_i.partidos_totales();
            } else {
                if (equipo_i.cant_matches_con.count(equipo_j.id)) {
                    M[i][j] = 0 - equipo_i.cant_matches_con.at(equipo_j.id);
                } else {
                    M[i][j] = 0;
                }
            }
            j++;
        }
        i++;
    }

    i = 0;
    for (const auto &ei : Equipos) {
        auto &equipo_i = ei.second;
        M[i][Equipos.size()] = equipo_i.diferencia_de_puntos;
        i++;
    }

    //Ajusto los valores de la matriz de Massey
    int N = M.size();
    for (int k = 0; k < N + 1; ++k) {
        if (k < N) {
            M[N - 1][k] = 1;
        } else {
            M[N - 1][k] = 0;
        }
    }

    // Resolvemos el sistema
    //print_matrix(M,4);
    Elim_Gaussiana_Con_Permutaciones(M,res);
    return res;
}

/*Funcion que swapea dos filas de una matriz*/
void swap_row(vector<vector<double>> &augmentedMatrix, int N, int i, int j){

	for(int k = 0; k < N + 1; k++){
		double temp = augmentedMatrix[i][k];
		augmentedMatrix[i][k] = augmentedMatrix[j][k];
		augmentedMatrix[j][k] = temp;
	}
}

/*Funcion que triangula superiormente una matriz aumentada sin realizar permutaciones. */
/* Como precondición Aii != 0 para todo 0 <=i < N en cada paso de la eliminación*/
void triangularSupSinPermutaciones(vector<vector<double>> &augmentedMatrix, int N){

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

/*Funcion que triangula superiormente una matriz aumentada realizando permutaciones. */
/* Como precondición la matriz aumentada ingresada como parametro debe ser no singular. */ 
void triangularSupConPermutaciones(vector<vector<double>> &augmentedMatrix, int N){

	for (int k = 0; k < N; k++){

		int index_max = k;
		double value_max = fabs(augmentedMatrix[index_max][k]);

		for (int i = k+1; i < N; i++){
			if (fabs(augmentedMatrix[i][k]) > fabs(value_max)){
				value_max = augmentedMatrix[i][k];
				index_max = i;
			}
		}

		if (index_max != k){
			swap_row(augmentedMatrix, N, k, index_max);
		}

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
void Elim_Gaussiana_Sin_Permutaciones(vector<vector<double>> &augmentedMatrix, vector<double> &res){

	int N = augmentedMatrix.size();
    vector<vector<double>> copy_m (augmentedMatrix);

	triangularSupSinPermutaciones(augmentedMatrix, N);
    //print_matrix(augmentedMatrix,15);
	backwardSubstitution(augmentedMatrix, N, res);
    //print_matrix(augmentedMatrix,15);
    test_matrix(copy_m,res,0.0001); //TODO: Comentar cuando ande todov

}

void Elim_Gaussiana_Con_Permutaciones(vector<vector<double>> &augmentedMatrix, vector<double> &res){

	int N = augmentedMatrix.size();
    vector<vector<double>> copy_m (augmentedMatrix);

	triangularSupConPermutaciones(augmentedMatrix, N);
    //print_matrix(augmentedMatrix,15);
	backwardSubstitution(augmentedMatrix, N, res);
    //print_matrix(augmentedMatrix,15);
    test_matrix(copy_m,res,0.0001); //TODO: Comentar cuando ande todov

}


