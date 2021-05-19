#include "Habitacion.h"
#include "assert.h"
#include <istream>
#include <sstream>
#include <string>

Habitacion string_to_hab(std::istream& is) {
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    return Habitacion(height, occupied);
}

Habitacion string_to_hab(string s) {
    std::istringstream is(s);
    int row = 0;
    int col = 0;
    int max_col = 0;
    int width;
    int height;

    set<Pos> occupied;

    char c;
    while (is.get(c)) {
        if (c == '#') {
            occupied.insert(Pos(col, row));
            col++;
        } else if (c == '\n') {
            row++;
            max_col = std::max(col, max_col);
            col = 0;
        } else {
            col++;
        }
    }
    width = max_col;
    height = row;

    assert(height == width);

    return Habitacion(height, occupied);
}

// Completar
Habitacion::Habitacion(unsigned int tam, set<Pos> ocupadas) : _tamanio(tam){
    vector<vector<bool > > casilleros(tam, vector<bool >(tam, true));
    _casilleros = casilleros;
    auto itOcupadas = ocupadas.begin();
    while(itOcupadas != ocupadas.end()) {
        int i = (*itOcupadas).first;
        int j = (*itOcupadas).second;
        _casilleros[i][j] = false;
        ++itOcupadas;
    }
}

unsigned int Habitacion::tam() const {
    return _tamanio;
}

bool Habitacion::ocupado(Pos pos) const {
    return !(_casilleros[pos.first][pos.second]);
}

bool Habitacion::operator==(const Habitacion &h) const {
    return _tamanio == h._tamanio and
            _casilleros == h._casilleros;
}

bool Habitacion::hayAdjArriba(const Pos &pos) const {
    int arriba = pos.second + 1;
    Pos casillaArriba = make_pair(pos.first, pos.second + 1);
    if(arriba >= _tamanio){
        return false;
    } else {
        return !ocupado(casillaArriba);
    }
}

bool Habitacion::hayAdjAbajo(const Pos &pos) const {
    int abajo = pos.second - 1;
    Pos casillaAbajo = make_pair(pos.first, pos.second - 1);
    if(abajo < 0){
        return false;
    } else {
        return !ocupado(casillaAbajo);
    }
}

bool Habitacion::hayAdjDerecha(const Pos &pos) const {
    int derecha = pos.first + 1;
    Pos casillaDerecha = make_pair(pos.first + 1, pos.second);
    if(derecha >= _tamanio){
        return false;
    } else {
        return !ocupado(casillaDerecha);
    }
}

bool Habitacion::hayAdjIzquierda(const Pos &pos) const {
    int izquierda = pos.first - 1;
    Pos casillaIzquierda = make_pair(pos.first - 1, pos.second);
    if(izquierda < 0){
        return false;
    } else {
        return !ocupado(casillaIzquierda);
    }
}

linear_set<Pos> Habitacion::alcanceDisparo(const Pos &pos,const Dir &dir) const {
    unsigned int i = pos.first;
    unsigned int j = pos.second;
    linear_set<Pos > res;
    if(dir == ARRIBA){
        while(hayAdjArriba(Pos(i, j))){
            res.fast_insert(Pos(i, j + 1));
            j++;
        }
    } else if(dir == ABAJO){
        while(hayAdjAbajo(Pos(i, j))){
            res.fast_insert(Pos(i, j - 1));
            j--;
        }
    } else if(dir == DERECHA){
        while(hayAdjDerecha(Pos(i, j))){
            res.fast_insert(Pos(i + 1, j));
            i++;
        }
    } else {
        while(hayAdjIzquierda(Pos(i, j))){
            res.fast_insert(Pos(i - 1, j));
            i--;
        }
    }
    return res;
}
