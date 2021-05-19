#ifndef EXTREME_EXORCISM_H
#define EXTREME_EXORCISM_H

#include "Contexto.h"
#include "Habitacion.h"
#include "TiposJuego.h"
#include "string_map.h"
#include "modulos_basicos/linear_map.h"
#include "modulos_basicos/linear_set.h"
#include <list>
#include <map>
#include <set>
#include <string>
#include <utility>
#include <array>
#include <vector>

using namespace std;

class ExtremeExorcism {
public:
  ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init,
                  list<Accion> acciones_fantasma, Contexto *ctx);

  void pasar();

  void ejecutarAccion(Jugador j, Accion a);

  list<pair<Jugador, PosYDir>> posicionJugadores() const;

  list<PosYDir> posicionFantasmas() const;

  PosYDir posicionEspecial() const;

  list<PosYDir> disparosFantasmas() const;

  set<Pos> posicionesDisparadas() const;

  bool jugadorVivo(Jugador j) const;

  const Habitacion &habitacion() const;

  PosYDir posicionJugador(Jugador j) const;

  const set<Jugador> &jugadores() const;

  const list<Fantasma> &fantasmas() const;

private:
    set<Jugador > _conjuntoJugadores;
    string_map<tuple<list<Evento>, bool, unsigned int > > _jugadores;
    list<Fantasma > _fantasmas;
    Habitacion _habitacion;
    Fantasma _fantasmaEspecial;
    linear_map<Jugador, tuple<list<Evento >, unsigned int > > _jugadoresVivos;
    list<Fantasma > _fantasmasVivos;
    linear_map<Jugador, tuple<linear_map<Jugador, tuple<list<Evento >,unsigned int > >::iterator,
            string_map<tuple<list<Evento>, bool, unsigned int > >::iterator > > _datosJugadoresVivos;
    list<pair<list<Fantasma>::iterator,
            list<Evento>::iterator > > _datosFantasmasVivos;
    vector<vector<bool> > _casillerosDisparadosPorFantasmas;
    unsigned int _cantTurnosRondaActual;
    Contexto* _ctx;

    //Funciones no exportadas
    Fantasma crearFantasmaInicial(PosYDir &pxd, list<Accion  > &accionesFan);
    Fantasma crearFantasma(list<Evento > eventosJugador);
    void crearEventos(list<Evento > &evenIni, list<Accion > &acciones);
    void extenderAccionesFantasma(Fantasma &fan);
    Fantasma invertirAcciones(const Fantasma &fan);
    Evento invertirEvento(const Evento &evento);
    Dir invertir(const Dir &dir);
    void concatenar(Fantasma &fan, const Fantasma &reverso);
    linear_set<PosYDir> obtenerPosYDirFantasmasDisparando() const;
    void actualizarCasillasDisparadasPorFantasmas();
    void reiniciarCasillasDisparadasPorFantasmas();
    void reiniciarDatosDeFantasmasVivos();
    void reiniciarJugadores();
    void actualizarFantasmasVivos(const Jugador &j);
    void aplicarAccionAlJugador(const Accion &a,const Jugador &j);
    void actualizarDatosJugadoresVivosYJugadoresVivos();
    void actualizarDatosDeFantasmasVivos();
    void aplicarPasarAJugadores(const Jugador &j);
};

#endif
