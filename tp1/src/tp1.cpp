#include <iostream>
#include <fstream>
#include <map>
#include <vector>
#include "metodos.h"

using namespace std;

//--------------------------------------------MAIN-------------------------------------------------------
int main(int argc, char** argv){
    //std::ifstream in("in.txt");
    //std::cin.rdbuf(in.rdbuf());
    // Leemos el parametro que indica el algoritmo a ejecutar
    map<string, string> metodos_implementados = {
            {"0", "CMM"}, {"1", "WP"}, {"2", "Alternativo"}
    };

    // Verificar que el metodo pedido exista
    if (argc < 2 || metodos_implementados.find(argv[3]) == metodos_implementados.end()){
        cerr << "Metodo no encontrado" << endl;
        cerr << "Los algoritmos existentes son: " << endl;
        for (auto& alg_desc: metodos_implementados) cerr << "\t- " << alg_desc.first << ": " << alg_desc.second << endl;
        return 0;
    }

    // Nos quedamos con los parametros de entrada
    string metodo = argv[3];
    string input_file;
    string output_file;
    if (argc > 3) {
        input_file = argv[1];
        output_file = argv[2];
    }else {
        cerr<<"Ingreso incorrecto de parametros" <<endl;
        return 0;
    }

    // Leemos el input
    fstream fin (input_file);
    int cant_equipos = 0;
    int cant_partidos = 0;
    fin >> cant_equipos >> cant_partidos;

    vector<Partido> partidos;
    for (int i = 0; i < cant_partidos; ++i) {
        Partido partido;
        fin >> partido.fecha;
        fin >> partido.equipo_1 >> partido.puntaje_1;
        fin >> partido.equipo_2 >> partido.puntaje_2;
        partidos.push_back(partido);
    }
    fin.close();

    // Creamos los equipos
    map<int,Equipo> Equipos;
    for (const auto &partido : partidos) {
        if (Equipos.count(partido.equipo_1)){
            //El equipo 1 ya existe, debo actualizar la info
            Equipo &equipo_actual = Equipos.find(partido.equipo_1)->second;
            equipo_actual.p_ganados += partido.ganador()==1 ? 1 : 0;
            equipo_actual.p_perdidos += partido.ganador()==2 ? 1 : 0;
            equipo_actual.cant_matches_con[partido.equipo_2]++;
            equipo_actual.diferencia_de_puntos += partido.puntaje_1 - partido.puntaje_2;
            equipo_actual.puntos_totales += partido.puntaje_1;
            equipo_actual.puntos_totales += partido.puntaje_2;
        }else{
            //El equipo 1 no existe todavia, debo agregarlo
            Equipo equipo_actual_1;
            equipo_actual_1.id = partido.equipo_1;
            equipo_actual_1.p_ganados += partido.ganador()==1 ? 1 : 0;
            equipo_actual_1.p_perdidos += partido.ganador()==2 ? 1 : 0;
            equipo_actual_1.cant_matches_con.insert({partido.equipo_2, 1});
            equipo_actual_1.diferencia_de_puntos = partido.puntaje_1 - partido.puntaje_2;
            equipo_actual_1.puntos_totales = partido.puntaje_1;
            equipo_actual_1.puntos_totales = partido.puntaje_2;
            Equipos.insert({equipo_actual_1.id, equipo_actual_1});
        }

        if (Equipos.count(partido.equipo_2)){
            //El equipo 2 ya existe, debo actualizar la info
            Equipo &equipo_actual = Equipos.find(partido.equipo_2)->second;
            equipo_actual.p_ganados += partido.ganador()==2 ? 1 : 0;
            equipo_actual.p_perdidos += partido.ganador()==1 ? 1 : 0;
            equipo_actual.cant_matches_con[partido.equipo_1]++;
            equipo_actual.diferencia_de_puntos += partido.puntaje_2 - partido.puntaje_1;
            equipo_actual.puntos_totales += partido.puntaje_1;
            equipo_actual.puntos_totales += partido.puntaje_2;
        }else{
            //El equipo 2 no existe todavia, debo agregarlo
            Equipo equipo_actual_2;
            equipo_actual_2.id = partido.equipo_2;
            equipo_actual_2.p_ganados += partido.ganador()==2 ? 1 : 0;
            equipo_actual_2.p_perdidos += partido.ganador()==1 ? 1 : 0;
            equipo_actual_2.cant_matches_con.insert({partido.equipo_1, 1});
            equipo_actual_2.diferencia_de_puntos = partido.puntaje_2 - partido.puntaje_1;
            equipo_actual_2.puntos_totales = partido.puntaje_1;
            equipo_actual_2.puntos_totales = partido.puntaje_2;
            Equipos.insert({equipo_actual_2.id, equipo_actual_2});
        }
    }

    // Ejecutamos el algoritmo
    vector<double> results (cant_equipos,0);
    if(metodo == "0"){
        CMM(Equipos, results);
    }else if (metodo == "1"){
        WP(Equipos, results);
    }else if (metodo == "2"){
        ScoreRatio(Equipos, results);
    }else{
        Massey(Equipos, results);
    }

    ofstream fout (output_file);
    for (double rating: results){
        fout << rating << "\n";
        //cout << rating << "\n";
    }
    fout.close();

    return 0;
}