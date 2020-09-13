#include <vector>
#include <map>

using namespace std;

struct Equipo{
    int id;
    int p_ganados;
    int p_perdidos;
    map<int,int> cant_matches_con; //map(id de otro equipo, cantidad de partidos con ese equipo)

    int p_totales() const {return p_ganados+p_perdidos;};
    Equipo(){id=0; p_perdidos=0; p_ganados=0;};
    bool operator<(const Equipo &e) const {
        return (id < e.id);
    }
};

struct Partido{
    string fecha;
    int equipo_1;
    int puntaje_1;
    int equipo_2;
    int puntaje_2;
    int ganador () const {
        return (puntaje_2>puntaje_1) + 1;
    };
    Partido(){equipo_1=0;equipo_2=0;puntaje_1=0;puntaje_2=0;};
};

vector<double>& CMM(const map<int,Equipo> &Equipos, vector<double> &res);
    
vector<double>& WP(map<int,Equipo> Equipos, vector<double> &res);
