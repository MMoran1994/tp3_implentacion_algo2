#ifndef TIPOS_JUEGO_H
#define TIPOS_JUEGO_H

#include <string>
#include <list>
#include <utility>
#include <iostream>

using namespace std;

using Jugador = string;
using IDFantasma = unsigned int;
using Pos = pair<unsigned int, unsigned int>;
using Ronda = unsigned int;

struct Evento;

using Fantasma = list<Evento >;

enum Dir {
    ARRIBA = 0, ABAJO = 1, IZQUIERDA = 2, DERECHA = 3
};

enum Accion {
    MARRIBA = 0, MABAJO = 1, MIZQUIERDA = 2, MDERECHA = 3, DISPARAR = 4, ESPERAR = 5
};

struct PosYDir {
    Pos pos;
    Dir dir;

    PosYDir(Pos, Dir);

    bool operator==(const PosYDir& o) const;
};

struct Evento {
    Pos pos;
    Dir dir;
    bool dispara;

    Evento(Pos, Dir, bool);
    Evento pasar() const;
    PosYDir pos_y_dir() const;

    bool operator==(const Evento&) const;
};

ostream& operator<<(ostream&, const Evento&);
ostream& operator<<(ostream&, const PosYDir&);

#endif
