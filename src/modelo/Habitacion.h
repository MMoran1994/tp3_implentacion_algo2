#ifndef INC_2019_C1_TP3_HABITACION_H
#define INC_2019_C1_TP3_HABITACION_H

#include <iostream>
#include <set>
#include <vector>
#include <modulos_basicos/linear_set.h>
#include "TiposJuego.h"

using namespace std;


class Habitacion {
public:
    Habitacion(unsigned int tam, set<Pos> ocupadas);

    unsigned int tam() const;

    bool ocupado(Pos pos) const;

    bool operator==(const Habitacion& h) const;

    bool hayAdjArriba(const Pos& pos) const;

    bool hayAdjAbajo(const Pos& pos) const;

    bool hayAdjDerecha(const Pos& pos) const;

    bool hayAdjIzquierda(const Pos& pos) const;

    linear_set<Pos > alcanceDisparo(const Pos &pos,const Dir &dir) const;

private:
  vector<vector<bool > > _casilleros;
  unsigned int _tamanio;
};


Habitacion string_to_hab(std::istream& is);
Habitacion string_to_hab(string s);

#endif
