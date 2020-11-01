//
// Created by pachi on 5/6/19.
//

#include <iostream>
#include <fstream>
#include "pca.h"
#include "eigen.h"
#include "knn.h"

using namespace std;

// Convierte los datos del csv "input" a una matriz.
Matrix create_matrix(const std::string& input) {
    Matrix X;
    std::ifstream fileInput;
    fileInput.open(input);
    std::string row;
    int row_n = -1;
    int column_n;
    int max_column_n = 0;

    while(std::getline(fileInput, row)) {
        std::stringstream lineStream(row);
        std::string cell;
        if (row_n >= 0) {
            X.conservativeResize(row_n + 1, max_column_n);

            column_n = 0;

            while(std::getline(lineStream,cell,',')) {
                if (column_n >= 0) {
                    X(row_n, column_n) = std::stoi(cell);
                }
                column_n++;
            }
        } else {
            while(std::getline(lineStream,cell,',')) {
                max_column_n++;
            }
        }
        row_n++;
    }

    return X;
}

// Guarda una matriz en un archivo csv.
void save_vector(const Vector &A, const string &output) {
    ofstream Output;
    Output.open(output);
    for (int i = 0; i < A.rows(); i++) {
        Output << A(i) << "\n";
    }
    Output.close();
}

int run(const string& train_set_file, const string& test_set_file, const string& output, const unsigned int& k, const unsigned int& alpha, const unsigned int& method) {
    Matrix train_set_matrix = create_matrix(train_set_file);
    Matrix y_train = train_set_matrix.col(0);
    Matrix X_train = train_set_matrix.block(0, 1, train_set_matrix.rows(), train_set_matrix.cols() - 1);
    Matrix X_predict = create_matrix(test_set_file);

    KNNClassifier knn = KNNClassifier(k);

    if (method == 0) { // kNN
        knn.fit(X_train, y_train);
        Vector y_predict = knn.predict(X_predict);
        save_vector(y_predict, output);

        return 0;
    } else if (method == 1) { // PCA + kNN
        PCA pca = PCA(alpha);
        pca.fit(X_train);
        Matrix X_train_trans = pca.transform(X_train);
        Matrix X_predict_trans = pca.transform(X_predict);

        knn.fit(X_train_trans, y_train);
        Vector y_predict = knn.predict(X_predict_trans);
        save_vector(y_predict, output);

        return 0;
    } else {
        printf("Metodo Invalido\n");
        return 1;
    }
}

int main(int argc, char** argv){

    if (!(argc == 2 || argc == 9 || argc == 11 || argc == 13)) {
        printf("Parametros Invalidos\n");
        return 1;
    } else if (argc == 2) {
        string help = argv[1];
        if (help == "--help") {
            printf("Uso: tp2 -m <metodo> --k <k> --alpha <alpha> -i <entrenamiento> -q <test> -o <output>\n");
            printf("\n");
            printf("<metodo>: 0 para kNN solo y 1 para PCA+kNN.\n");
            printf("<entrenamiento>: Direccion para el archivo CSV de entrenamiento.\n");
            printf("<test>: Direccion para el archivo CSV de test.\n");
            printf("<output>: Direccion para el archivo CSV donde guardar el resultado.\n");
            printf("\n");
            printf("Opcionales:\n");
            printf("\n");
            printf("--k <k>: Establece el valor de k para kNN como <k>. El valor predeterminado es 10.\n");
            printf("--alpha <alpha>: Establece el valor de alpha para PCA como <alpha>. El valor predeterminado es 30.\n");

            return 0;
        } else {
            printf("Parametros Invalidos\n");
            return 1;
        }
    } else if (argc == 9) {
        unsigned int method = atoi(argv[2]);
        string train_set_file = argv[4];
        string test_set_file = argv[6];
        string output = argv[8];

        unsigned int k = 10;
        unsigned int alpha = 30;

        return run(train_set_file, test_set_file, output, k, alpha, method);
    } else if (argc == 11) {
        unsigned int method = atoi(argv[2]);
        string train_set_file = argv[6];
        string test_set_file = argv[8];
        string output = argv[10];

        string optional = argv[3];
        if (optional == "--k") {
            unsigned int k = atoi(argv[4]);
            unsigned int alpha = 30;

            return run(train_set_file, test_set_file, output, k, alpha, method);
        } else if (optional == "--alpha") {
            unsigned int k = 10;
            unsigned int alpha = atoi(argv[4]);

            return run(train_set_file, test_set_file, output, k, alpha, method);
        } else {
            printf("Parametros Invalidos\n");
            return 1;
        }
    } else if (argc == 13) {
        unsigned int method = atoi(argv[2]);
        string train_set_file = argv[8];
        string test_set_file = argv[10];
        string output = argv[12];

        string optional1 = argv[3];
        string optional2 = argv[5];
        if (optional1 == "--k" && optional2 == "--alpha") {
            unsigned int k = atoi(argv[4]);
            unsigned int alpha = atoi(argv[6]);

            return run(train_set_file, test_set_file, output, k, alpha, method);
        } else if (optional1 == "--alpha" && optional2 == "--k") {
            unsigned int k = atoi(argv[6]);
            unsigned int alpha = atoi(argv[4]);

            return run(train_set_file, test_set_file, output, k, alpha, method);
        } else {
            printf("Parametros Invalidos\n");
            return 1;
        }
    }
}
