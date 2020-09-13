#include <vector>
#include <map>

using namespace std;

struct Equipo{
    unsigned int id;
    unsigned int p_ganados;
    unsigned int p_perdidos;
    map<int,int> cant_matches_con; //map(id de otro equipo, cantidad de partidos con ese equipo)

    unsigned int p_totales() const {return p_ganados+p_perdidos;};
    Equipo(){id=0; p_perdidos=0; p_ganados=0;};
    bool operator<(const Equipo &e) const {
        return (id < e.id);
    }
};

struct Partido{
    string fecha;
    unsigned int equipo_1;
    unsigned int puntaje_1;
    unsigned int equipo_2;
    unsigned int puntaje_2;
    int ganador () const {
        return (puntaje_2>puntaje_1) + 1;
    };
    Partido(){equipo_1=0;equipo_2=0;puntaje_1=0;puntaje_2=0;};
};

vector<double>& CMM(map<int,Equipo> Equipos, vector<double> &res);
    
vector<double>& WP(map<int,Equipo> Equipos, vector<double> &res);
