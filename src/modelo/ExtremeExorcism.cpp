#include "ExtremeExorcism.h"

// Completar
ExtremeExorcism::ExtremeExorcism(Habitacion h, set<Jugador> jugadores, PosYDir f_init, list<Accion> acciones_fantasma,
                                 Contexto *ctx) : _habitacion(h) {
    _conjuntoJugadores = jugadores;
    auto itJugs = jugadores.begin();
    Fantasma fanIni = crearFantasmaInicial(f_init, acciones_fantasma);
    list<Fantasma > listaFanInicial;
    listaFanInicial.push_back(fanIni);
    map<Jugador, PosYDir > diccJugadores = ctx->localizar_jugadores(jugadores, listaFanInicial,h);
    unsigned int id = jugadores.size();
    while(itJugs != jugadores.end()){
        Jugador jugadorActual = *itJugs;
        PosYDir posYDirIni = diccJugadores.at(jugadorActual);
        Evento evenIni(posYDirIni.pos, posYDirIni.dir, false);
        list<Evento > listaEvenIni;
        listaEvenIni.push_back(evenIni);
        tuple<list<Evento >, bool, unsigned int > datosJugTrie = make_tuple(listaEvenIni, true, id);
        tuple<list<Evento >, unsigned int > datosListaJugVivos = make_tuple(listaEvenIni, id);
        auto itTrie = _jugadores.definir(jugadorActual, datosJugTrie);
        auto itJugVivo = _jugadoresVivos.fast_insert(make_pair(jugadorActual ,datosListaJugVivos));
        auto itesJugVivos = make_pair(jugadorActual,make_tuple(itJugVivo, itTrie));
        _datosJugadoresVivos.fast_insert(itesJugVivos);
        id--;
        ++itJugs;
    }
    _fantasmaEspecial = fanIni;
    _fantasmas.push_back(fanIni);
    auto itFan = _fantasmasVivos.insert(_fantasmasVivos.end(), fanIni);
    auto itEveFan = (*itFan).begin();
    _datosFantasmasVivos.push_back(make_pair(itFan, itEveFan));
    vector<vector<bool > > casilleros(h.tam(), vector<bool >(h.tam(), false));
    _casillerosDisparadosPorFantasmas = casilleros;
    _cantTurnosRondaActual = 0;
    _ctx = ctx;
}

Fantasma ExtremeExorcism::crearFantasmaInicial(PosYDir &pxd, list<Accion> &accionesFan) {
    Evento evenIniF(pxd.pos, pxd.dir, false);
    Fantasma fan;
    fan.push_back(evenIniF);
    crearEventos(fan, accionesFan);
    Fantasma reverso = invertirAcciones(fan);
    extenderAccionesFantasma(fan);
    concatenar(fan, reverso);
    extenderAccionesFantasma(fan);
    return fan;
}

Fantasma ExtremeExorcism::crearFantasma(list<Evento> eventosJugador){
    Fantasma reverso = invertirAcciones(eventosJugador);
    extenderAccionesFantasma(eventosJugador);
    concatenar(eventosJugador, reverso);
    extenderAccionesFantasma(eventosJugador);
    return eventosJugador;
}

void ExtremeExorcism::crearEventos(list<Evento > &evenIni, list<Accion> &acciones) {
    auto itEven = evenIni.begin();
    Evento evenActual = *itEven;
    auto itAcciones = acciones.begin();
    while(itAcciones != acciones.end()){
        Pos posActual = evenActual.pos;
        Dir dirActual = evenActual.dir;
        Accion accionActual = *itAcciones;
        if(accionActual == MARRIBA){
            if(_habitacion.hayAdjArriba(posActual)) {
                Pos nuevaPos = make_pair(posActual.first, posActual.second + 1);
                Evento nuevoEvento(nuevaPos, ARRIBA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            } else {
                Evento nuevoEvento(posActual, ARRIBA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            }
        } else if(accionActual == MABAJO){
            if(_habitacion.hayAdjAbajo(posActual)){
                Pos nuevaPos = make_pair(posActual.first, posActual.second - 1);
                Evento nuevoEvento(nuevaPos, ABAJO, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            } else {
                Evento nuevoEvento(posActual, ABAJO, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            }
        } else if(accionActual == MDERECHA){
            if(_habitacion.hayAdjDerecha(posActual)){
                Pos nuevaPos = make_pair(posActual.first + 1, posActual.second);
                Evento nuevoEvento(nuevaPos, DERECHA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            } else {
                Evento nuevoEvento(posActual, DERECHA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            }
        } else if(accionActual == MIZQUIERDA){
            if(_habitacion.hayAdjIzquierda(posActual)){
                Pos nuevaPos = make_pair(posActual.first - 1, posActual.second);
                Evento nuevoEvento(nuevaPos, IZQUIERDA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            } else {
                Evento nuevoEvento(posActual, IZQUIERDA, false);
                evenIni.push_back(nuevoEvento);
                evenActual = nuevoEvento;
            }
        } else if(accionActual == DISPARAR){
            Evento nuevoEvento(posActual, dirActual, true);
            evenIni.push_back(nuevoEvento);
        } else {
            Evento nuevoEvento(posActual, dirActual, false);
            evenIni.push_back(nuevoEvento);
        }
        ++itAcciones;
    }
}

void ExtremeExorcism::extenderAccionesFantasma(Fantasma &fan) {
    Evento ultimoEvento = fan.back();
    Evento nuevoEvento(ultimoEvento.pos, ultimoEvento.dir, false);
    for(int i = 0; i < 5; i++){
        fan.push_back(nuevoEvento);
    }
}

Fantasma ExtremeExorcism::invertirAcciones(const Fantasma &fan) {
    list<Evento > res;
    /*if(fan.size() == 1) {
        Evento eventoInicial = fan.front();
        Evento eventoInverso(eventoInicial.pos, invertir(eventoInicial.dir), false);
        res.push_back(eventoInverso);
        return res;
    } else {
        Fantasma listaInversa = fan;
        listaInversa.pop_front();
        listaInversa.reverse();
        auto itEventos = listaInversa.begin();
        Evento ultimoEvento = *itEventos;
        while (itEventos != fan.end()) {
            Evento eventoActual = *itEventos;
            Pos ultimaPos = ultimoEvento.pos;
            Dir ultimaDir = ultimoEvento.dir;
            if (eventoActual.dispara) {
                Evento nuevoEvento(ultimaPos, ultimaDir, true);
                res.push_back(nuevoEvento);
            } else {
                Evento eventoInverso = invertirEvento(eventoActual);
                res.push_front(eventoInverso);
                ultimoEvento = eventoInverso;
            }
            ++itEventos;
        }
        return res;
    }*/
    Fantasma listaInversa = fan;
    //listaInversa.reverse();
    auto itEventos = listaInversa.begin();
    while (itEventos != listaInversa.end()) {
        Evento eventoInverso = invertirEvento(*itEventos);
        res.push_front(eventoInverso);
        ++itEventos;
    }
    return res;
}

Evento ExtremeExorcism::invertirEvento(const Evento &evento) {
    Dir dirEvento = evento.dir;
    Dir dirInversa;
    Pos posEvento = evento.pos;
    Pos posInversa = evento.pos;
    if(dirEvento == ARRIBA){
        dirInversa = ABAJO;
    } else if(dirEvento == ABAJO){
        dirInversa = ARRIBA;
    } else if(dirEvento == DERECHA){
        dirInversa = IZQUIERDA;
    } else {
        dirInversa = DERECHA;
    }
    Evento res(posInversa, dirInversa, evento.dispara);
    return res;
}

Dir ExtremeExorcism::invertir(const Dir &dir) {
    Dir res;
    if(dir == ARRIBA){
        res = ABAJO;
    } else if(dir == ABAJO){
        res = ARRIBA;
    } else if(dir == DERECHA){
        res = IZQUIERDA;
    } else {
        res = DERECHA;
    }
    return res;
}

void ExtremeExorcism::concatenar(Fantasma &fan, const Fantasma &reverso) {
    auto itReverso = reverso.begin();
    while(itReverso != reverso.end()){
        Evento eventoActual = *itReverso;
        fan.push_back(eventoActual);
        ++itReverso;
    }
}

bool ExtremeExorcism::jugadorVivo(Jugador j) const {
    bool res = get<1>(_jugadores.at(j));
    return res;
}

set<Pos> ExtremeExorcism::posicionesDisparadas() const {
    set<Pos > res;
    linear_set<PosYDir > PyDFD = obtenerPosYDirFantasmasDisparando();
    auto itPyDFD = PyDFD.begin();
    while (itPyDFD != PyDFD.end()) {
        PosYDir posYDirActual = *itPyDFD;
        linear_set<Pos > casDispFantasma =
                _habitacion.alcanceDisparo(posYDirActual.pos, posYDirActual.dir);
        auto itCasDispFantasma = casDispFantasma.begin();
        while(itCasDispFantasma != casDispFantasma.end()){
            res.insert(*itCasDispFantasma);
            ++itCasDispFantasma;
        }
        ++itPyDFD;
    }
    return res;
}

linear_set<PosYDir> ExtremeExorcism::obtenerPosYDirFantasmasDisparando() const {
    linear_set<PosYDir > res;
    auto itDatosFantasmasVivos = _datosFantasmasVivos.begin();
    while(itDatosFantasmasVivos != _datosFantasmasVivos.end()){
        Evento eventoActual = *((*itDatosFantasmasVivos).second);
        bool dispara = eventoActual.dispara;
        if(dispara){
            Pos posActualFan = eventoActual.pos;
            Dir dirActualFan = eventoActual.dir;
            res.fast_insert(PosYDir(posActualFan, dirActualFan));
        }
        ++itDatosFantasmasVivos;
    }
    return res;
}

void ExtremeExorcism::actualizarCasillasDisparadasPorFantasmas() {
    linear_set<PosYDir > PyDFD = obtenerPosYDirFantasmasDisparando();
    auto itPyDFD = PyDFD.begin();
    while(itPyDFD != PyDFD.end()){
        unsigned int i = (*itPyDFD).pos.first;
        unsigned int j = (*itPyDFD).pos.second;
        Dir dirActual = (*itPyDFD).dir;
        if(dirActual == ARRIBA){
            while(_habitacion.hayAdjArriba(Pos(i, j))){
                j++;
                _casillerosDisparadosPorFantasmas[i][j] = true;
            }
        } else if(dirActual == ABAJO){
            while(_habitacion.hayAdjAbajo(Pos(i, j))){
                j--;
                _casillerosDisparadosPorFantasmas[i][j] = true;
            }
        } else if(dirActual == DERECHA){
            while(_habitacion.hayAdjDerecha(Pos(i, j))){
                i++;
                _casillerosDisparadosPorFantasmas[i][j] = true;
            }
        } else {
            while(_habitacion.hayAdjIzquierda(Pos(i, j))){
                i--;
                _casillerosDisparadosPorFantasmas[i][j] = true;
            }
        }
        ++itPyDFD;
    }
}

void ExtremeExorcism::reiniciarCasillasDisparadasPorFantasmas() {
    linear_set<PosYDir> PyDFD = obtenerPosYDirFantasmasDisparando();
    auto itPyDFD = PyDFD.begin();
    while (itPyDFD != PyDFD.end()) {
        unsigned int i = (*itPyDFD).pos.first;
        unsigned int j = (*itPyDFD).pos.second;
        Dir dirActual = (*itPyDFD).dir;
        if (dirActual == ARRIBA) {
            while (_habitacion.hayAdjArriba(Pos(i, j))) {
                _casillerosDisparadosPorFantasmas[i][j] = false;
                j++;
            }
        } else if (dirActual == ABAJO) {
            while (_habitacion.hayAdjAbajo(Pos(i, j))) {
                _casillerosDisparadosPorFantasmas[i][j] = false;
                j--;
            }
        } else if (dirActual == DERECHA) {
            while (_habitacion.hayAdjDerecha(Pos(i, j))) {
                _casillerosDisparadosPorFantasmas[i][j] = false;
                i++;
            }
        } else {
            while (_habitacion.hayAdjIzquierda(Pos(i, j))) {
                _casillerosDisparadosPorFantasmas[i][j] = false;
                i--;
            }
        }
        ++itPyDFD;
    }
}

void ExtremeExorcism::ejecutarAccion(Jugador j, Accion a) {
    list<Evento>& listaDeEventosDelJugadorActual = get<0>(_jugadores.at(j));
    Pos posActualDelJugador = listaDeEventosDelJugadorActual.back().pos;
    Dir dirActualDelJugador = listaDeEventosDelJugadorActual.back().dir;
    Pos posicionDelFanEsp = _datosFantasmasVivos.back().second->pos;
    linear_set<Pos> posDisparadosPorJugador = _habitacion.alcanceDisparo(posActualDelJugador, dirActualDelJugador);
    if (a == DISPARAR and posDisparadosPorJugador.count(posicionDelFanEsp)) {
        Evento ultimoEventoDelJugador(posActualDelJugador, dirActualDelJugador, true);
        listaDeEventosDelJugadorActual.push_back(ultimoEventoDelJugador);
        Fantasma nuevoFantasma = crearFantasma(listaDeEventosDelJugadorActual);
        _fantasmaEspecial = nuevoFantasma;
        _fantasmas.push_back(nuevoFantasma);
        _fantasmasVivos = _fantasmas;
        reiniciarDatosDeFantasmasVivos();
        reiniciarJugadores();
        _cantTurnosRondaActual = 0;
    } else {
        if (a == DISPARAR) {
            actualizarFantasmasVivos(j);
        }
        aplicarAccionAlJugador(a, j);
        actualizarCasillasDisparadasPorFantasmas();
        actualizarDatosJugadoresVivosYJugadoresVivos();
        reiniciarCasillasDisparadasPorFantasmas();
        actualizarDatosDeFantasmasVivos();
        aplicarPasarAJugadores(j);
        _cantTurnosRondaActual++;
    }
}

void ExtremeExorcism::reiniciarDatosDeFantasmasVivos() {
    auto itFantasmas = _fantasmas.begin();
    list<pair<list<Fantasma>::iterator, list<Evento>::iterator > > listaVacia;
    _datosFantasmasVivos = listaVacia;
    while(itFantasmas != _fantasmas.end()){
        auto itAlFantasma = itFantasmas;
        auto itAlEventoDelFantasma = (*itFantasmas).begin();
        auto datos = make_pair(itAlFantasma, itAlEventoDelFantasma);
        _datosFantasmasVivos.push_back(datos);
        ++itFantasmas;
    }
}

void ExtremeExorcism::reiniciarJugadores() {
    map<Jugador, PosYDir > DiccJugadores = _ctx->localizar_jugadores(_conjuntoJugadores, _fantasmas, _habitacion);
    auto itAJugadores = _jugadores.begin();
    while(itAJugadores != _jugadores.end()){
        unsigned int id = get<2>(*(*itAJugadores).second);
        Jugador sigJugador = (*itAJugadores).first;
        list<Evento > listaDeEvento;
        PosYDir posYDirInicialDelJugador = DiccJugadores.at(sigJugador);
        Evento primerEventoDelJugador(posYDirInicialDelJugador.pos, posYDirInicialDelJugador.dir, false);
        listaDeEvento.push_back(primerEventoDelJugador);
        *(*itAJugadores).second = make_tuple(listaDeEvento, true, id);
        auto itJugadoresVivos = (_jugadoresVivos.insert(make_pair(sigJugador, make_tuple(get<0>(*((*itAJugadores).second)), id)))).first;
        _datosJugadoresVivos.insert(make_pair(sigJugador, make_tuple(itJugadoresVivos, itAJugadores)));
        ++itAJugadores;
    }
}

void ExtremeExorcism::actualizarFantasmasVivos(const Jugador &j) {
    list<Evento > listaDeEventosDelJugadorActual = get<0>(_jugadores.at(j));
    Pos posActualDelJugador = listaDeEventosDelJugadorActual.back().pos;
    Dir dirActualDelJugador = listaDeEventosDelJugadorActual.back().dir;
    linear_set<Pos > alcanceDelDisparoDelJugador = _habitacion.alcanceDisparo(posActualDelJugador, dirActualDelJugador);
    auto itDatosFantasmasVivos = _datosFantasmasVivos.begin();
    while(itDatosFantasmasVivos != _datosFantasmasVivos.end()){
        Pos posDelFantasma = (*((*itDatosFantasmasVivos).second)).pos;
        if(alcanceDelDisparoDelJugador.count(posDelFantasma)){
            _fantasmas.erase((*itDatosFantasmasVivos).first);
            itDatosFantasmasVivos = _datosFantasmasVivos.erase(itDatosFantasmasVivos);
        } else {
            ++itDatosFantasmasVivos;
        }
    }
}

void ExtremeExorcism::aplicarAccionAlJugador(const Accion &a, const Jugador &j) {
    tuple<list<Evento>, bool, int> datosJugador = _jugadores.at(j);
    list<Evento> listaDeEventosDeJugadores = get<0>(datosJugador);
    unsigned int idJugador = get<2>(datosJugador);
    auto itJugadoresVivos = _jugadoresVivos.begin();
    bool encontrado = false;
    while (!encontrado) {
        if (get<1>((*itJugadoresVivos).second) == idJugador) {
            encontrado = true;
        } else {
            ++itJugadoresVivos;
        }
    }
    list<Evento> listaDeEventosDeJugadoresVivos = get<0>((*itJugadoresVivos).second);
    Pos ultPosDelJugador = listaDeEventosDeJugadores.back().pos;
    Dir ultDirDelJugador = listaDeEventosDeJugadores.back().dir;
    if (a == DISPARAR) {
        Evento nuevoEvento(ultPosDelJugador, ultDirDelJugador, true);
        get<0>(_jugadores[j]).push_back(nuevoEvento);
        get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
    } else if (a == MARRIBA) {
        Pos nuevaPosicion = Pos(ultPosDelJugador.first, ultPosDelJugador.second + 1);
        if (_habitacion.hayAdjArriba(ultPosDelJugador)) {
            Evento nuevoEvento(nuevaPosicion, ARRIBA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        } else {
            Evento nuevoEvento(ultPosDelJugador, ARRIBA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        }
    } else if (a == MABAJO) {
        Pos nuevaPosicion = Pos(ultPosDelJugador.first, ultPosDelJugador.second - 1);
        if (_habitacion.hayAdjAbajo(ultPosDelJugador)) {
            Evento nuevoEvento(nuevaPosicion, ABAJO, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        } else {
            Evento nuevoEvento(ultPosDelJugador, ABAJO, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        }
    } else if (a == MDERECHA) {
        Pos nuevaPosicion = Pos(ultPosDelJugador.first + 1, ultPosDelJugador.second);
        if (_habitacion.hayAdjDerecha(ultPosDelJugador)) {
            Evento nuevoEvento(nuevaPosicion, DERECHA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        } else {
            Evento nuevoEvento(ultPosDelJugador, DERECHA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        }
    } else if (a == MIZQUIERDA) {
        Pos nuevaPosicion = Pos(ultPosDelJugador.first - 1, ultPosDelJugador.second);
        if (_habitacion.hayAdjIzquierda(ultPosDelJugador)) {
            Evento nuevoEvento(nuevaPosicion, IZQUIERDA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        } else {
            Evento nuevoEvento(ultPosDelJugador, IZQUIERDA, false);
            get<0>(_jugadores[j]).push_back(nuevoEvento);
            get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
        }
    } else {
        Evento nuevoEvento(ultPosDelJugador, ultDirDelJugador, false);
        get<0>(_jugadores[j]).push_back(nuevoEvento);
        get<0>((*itJugadoresVivos).second).push_back(nuevoEvento);
    }
}

void ExtremeExorcism::actualizarDatosJugadoresVivosYJugadoresVivos() {
    auto itDatosJugadoresVivos = _datosJugadoresVivos.begin();
    while(itDatosJugadoresVivos != _datosJugadoresVivos.end()){
        Pos posJugador = ((get<0>((*(get<0>((*itDatosJugadoresVivos).second))).second)).back()).pos;
        unsigned int i = posJugador.first;
        unsigned int j = posJugador.second;
        if(_casillerosDisparadosPorFantasmas[i][j]){
            _jugadoresVivos.erase(get<0>((*itDatosJugadoresVivos).second));
            get<1>(*((*(get<1>((*itDatosJugadoresVivos).second))).second)) = false;
            itDatosJugadoresVivos = _datosJugadoresVivos.erase(itDatosJugadoresVivos);
        } else {
            ++itDatosJugadoresVivos;
        }
    }
}

void ExtremeExorcism::actualizarDatosDeFantasmasVivos() {
    auto itDFV = _datosFantasmasVivos.begin();
    while(itDFV != _datosFantasmasVivos.end()){
        auto& itAEvento = (*itDFV).second;
        (*itDFV).second = ++((*itDFV).second);
        if(itAEvento == (*((*itDFV).first)).end()){
            Fantasma* fantasmaActual = &(*((*itDFV).first));
            (*itDFV).second = (*fantasmaActual).begin();
        }
        ++itDFV;
    }
}

void ExtremeExorcism::aplicarPasarAJugadores(const Jugador &j) {
    auto itDatosJugadoresVivos = _datosJugadoresVivos.begin();
    unsigned int idJugador = get<2>(_jugadores.at(j));
    while(itDatosJugadoresVivos != _datosJugadoresVivos.end()){
        if(get<1>((*(get<0>((*itDatosJugadoresVivos).second))).second) != idJugador){
            list<Evento >& listaEventosJugadoresVivos = get<0>((*(get<0>((*itDatosJugadoresVivos).second))).second);
            Pos ultimaPos = listaEventosJugadoresVivos.back().pos;
            Dir ultimaDir = listaEventosJugadoresVivos.back().dir;
            Evento ultEvento(ultimaPos, ultimaDir, false);
            list<Evento >& listaEventosJugadores = get<0>(*((*(get<1>((*itDatosJugadoresVivos).second))).second));
            listaEventosJugadoresVivos.push_back(ultEvento);
            listaEventosJugadores.push_back(ultEvento);
        }
        ++itDatosJugadoresVivos;
    }
}

void ExtremeExorcism::pasar() {
    actualizarDatosDeFantasmasVivos();
    actualizarCasillasDisparadasPorFantasmas();
    auto itDatosJugadoresVivos = _datosJugadoresVivos.begin();
    while(itDatosJugadoresVivos != _datosJugadoresVivos.end()) {                //aplica pasar a todos los jugadores
        list<Evento>* listaEventosJugadoresVivos = &get<0>((*(get<0>((*itDatosJugadoresVivos).second))).second);
        Pos ultimaPos = (*listaEventosJugadoresVivos).back().pos;
        Dir ultimaDir = (*listaEventosJugadoresVivos).back().dir;
        Evento ultEvento(ultimaPos, ultimaDir, false);
        list<Evento>* listaEventosJugadores = &get<0>(*((*(get<1>((*itDatosJugadoresVivos).second))).second));
        (*listaEventosJugadoresVivos).push_back(ultEvento);
        (*listaEventosJugadores).push_back(ultEvento);
        ++itDatosJugadoresVivos;
    }
    actualizarDatosJugadoresVivosYJugadoresVivos();
    reiniciarCasillasDisparadasPorFantasmas();
}

list<pair<Jugador, PosYDir>> ExtremeExorcism::posicionJugadores() const {
    list<pair<Jugador, PosYDir > > res;
    auto itJugadoresVivos = _jugadoresVivos.begin();
    while(itJugadoresVivos != _jugadoresVivos.end()){
        Jugador jugadorActual = (*itJugadoresVivos).first;
        Pos ultimaPos = (get<0>((*itJugadoresVivos).second).back()).pos;
        Dir ultimaDir = (get<0>((*itJugadoresVivos).second).back()).dir;
        PosYDir ultimaPosYDir(ultimaPos, ultimaDir);
        res.push_back(make_pair(jugadorActual, ultimaPosYDir));
        ++itJugadoresVivos;
    }
    return res;
}

list<PosYDir> ExtremeExorcism::posicionFantasmas() const {
    list<PosYDir > res;
    auto itDatosFantasmasVivos = _datosFantasmasVivos.begin();
    while(itDatosFantasmasVivos != _datosFantasmasVivos.end()){
        Pos ultimaPos = (*((*itDatosFantasmasVivos).second)).pos;
        Dir ultimaDir =(*((*itDatosFantasmasVivos).second)).dir;
        PosYDir ultimaPosYDir(ultimaPos, ultimaDir);
        res.push_back(ultimaPosYDir);
        ++itDatosFantasmasVivos;
    }
    return res;
}

PosYDir ExtremeExorcism::posicionEspecial() const {
    auto itDatosFantasmasVivos = _datosFantasmasVivos.end();
    --itDatosFantasmasVivos;
    Pos ultimaPos = (*((*itDatosFantasmasVivos).second)).pos;
    Dir ultimaDir =(*((*itDatosFantasmasVivos).second)).dir;
    PosYDir ultimaPosYDir(ultimaPos, ultimaDir);
    return ultimaPosYDir;
}

list<PosYDir> ExtremeExorcism::disparosFantasmas() const {
    list<PosYDir > res;
    linear_set<PosYDir > f = obtenerPosYDirFantasmasDisparando();
    auto itF = f.begin();
    while(itF != f.end()){
        res.push_back(*itF);
        ++itF;
    }
    return res;
}

const Habitacion &ExtremeExorcism::habitacion() const {
    return _habitacion;
}

PosYDir ExtremeExorcism::posicionJugador(Jugador j) const {
    tuple<list<Evento >, bool, unsigned int > datosJug = _jugadores.at(j);
    Pos ultimaPos = get<0>(datosJug).back().pos;
    Dir ultimaDir = get<0>(datosJug).back().dir;
    PosYDir ultimaPosYDir(ultimaPos, ultimaDir);
    return ultimaPosYDir;
}

const set<Jugador> &ExtremeExorcism::jugadores() const {
    return _conjuntoJugadores;
}

const list<Fantasma> &ExtremeExorcism::fantasmas() const {
    return _fantasmas;
}




