#include "metodos.h"
    
vector<double>& WP(map<int,Equipo> Equipos, vector<double> &res){
    for(int i = 0; i < Equipos.size(); i++){
        double wp = (double) Equipos[i].p_ganados / (double)Equipos[i].p_totales();
        res.push_back(wp);
    }
    return res;
}

vector<double>& CMM(map<int,Equipo> Equipos, vector<double> &res){
    return res;
}