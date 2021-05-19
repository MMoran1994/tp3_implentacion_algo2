#include "string_map.h"

template <typename T>
string_map<T>::string_map() : raiz(new Nodo(NULL)), _size(0){
}

template <typename T>
string_map<T>::string_map(const string_map<T>& aCopiar) : string_map() { *this = aCopiar; } // Provisto por la catedra: utiliza el operador asignacion para realizar la copia.

template <typename T>
string_map<T>& string_map<T>::operator=(const string_map<T>& d) {
    borrarRecursivo(raiz);
    raiz = NULL;
    _size = d._size;
    if(d.raiz != NULL) {
        raiz = new Nodo();
        copiarRecursivo(raiz, d.raiz);
    }
    return *this;
}

template<typename T>
void string_map<T>::copiarRecursivo(string_map::Nodo *n, const Nodo *d) {
    if (d->definicion != NULL) {
        n->definicion = new T();
        *(n->definicion) = *(d->definicion);
    }
    for (int i = 0; i < 256; i++) {
        if(d->siguientes[i] != NULL) {
            n->siguientes[i] = new Nodo();
            copiarRecursivo(n->siguientes[i], d->siguientes[i]);
        }
    }
}

template<typename T>
void string_map<T>::borrarRecursivo(Nodo *n) {
    if(n != NULL) {
        for (int i = 0; i < 256; i++) {
            borrarRecursivo(n->siguientes[i]);
        }
        delete n->definicion;
        delete n;
    }
}

template <typename T>
string_map<T>::~string_map(){
    borrarRecursivo(raiz);
}

template <typename T>
T& string_map<T>::operator[](const string& clave){
    Nodo* actual = raiz;
    unsigned int i = 0;
    while(i < clave.size()){
        if(actual->siguientes[int(clave[i])] == NULL){                  //si no esta la letra.
            actual->siguientes[int(clave[i])] = new Nodo();
        }
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    bool definido = true;
    if(actual->definicion == NULL){
        actual->definicion = new T();
        definido = false;
    }
    if(!definido){
        _size++;
    }
    return *actual->definicion;
}

template <typename T>
typename string_map<T>::iterator string_map<T>::definir(const string& clave, const T& significado){
    Nodo* actual = raiz;
    unsigned int i = 0;
    while(i < clave.size()){
        if(actual->siguientes[int(clave[i])] == NULL){                  //si no esta la letra.
            actual->siguientes[int(clave[i])] = new Nodo();
        }
        actual = actual->siguientes[int(clave[i])];
        i++;
    }
    bool definido = true;
    if(actual->definicion == NULL){
        actual->definicion = new T();
        definido = false;
        *actual->definicion = significado;
    }
    if(!definido){
        _size++;
    }
    pair<const string&, T* > jugadorxClave = make_pair(clave, actual->definicion);
    auto it = _listaIteradores.fast_insert(jugadorxClave);
    return it;
}

template <typename T>
int string_map<T>::count(const string& clave) const{
    Nodo* actual = raiz;
    if(raiz == NULL){
        return 0;
    }
    for(unsigned int i = 0; i < clave.size(); i++){
        if(actual->siguientes[int(clave[i])] != NULL) {
            actual = actual->siguientes[int(clave[i])];
        } else {
            return 0;
        }
    }
    return actual->definicion != NULL;
}

template <typename T>
const T& string_map<T>::at(const string& clave) const {
    Nodo* actual = raiz;
    int i = 0;
    for(int i = 0; i < clave.size(); i++){
        actual = actual->siguientes[int(clave[i])];
    }
    return *actual->definicion;
}

template <typename T>
T& string_map<T>::at(const string& clave) {
    Nodo* actual = raiz;
    int i = 0;
    for(i = 0; i < clave.size(); i++){
        actual = actual->siguientes[int(clave[i])];
    }
    return *actual->definicion;
}

template <typename T>
void string_map<T>::erase(const string& clave) {
    if(clave.size() == 0){
        delete raiz->definicion;
        raiz->definicion = NULL;
        return;
    }
    vector<Nodo*> aBorrar = {};
    unsigned int i = 0;
    Nodo* actual = raiz;
    Nodo *ultimoSinBorrar = actual;
    bool definicionConRamas = false;
    while(i < clave.size()){
        actual = actual->siguientes[int(clave[i])];
        if(i == clave.size() - 1){
            if(cantidadDeNulls(actual->siguientes) != 256) {
                definicionConRamas = true;
            }
        } else if(cantidadDeNulls(actual->siguientes) < 255 or actual->definicion != NULL){  //si hay otras palabras o esta definido
            aBorrar = {};
            ultimoSinBorrar = actual;
        } else {
            aBorrar.push_back(actual);
        }
        i++;
    }
    if(i != 0) {
        i--;
    }
    delete actual->definicion;
    if(definicionConRamas){
        actual->definicion = NULL;
    } else {
        delete actual;
        while (aBorrar.size() != 0) {
            delete aBorrar[aBorrar.size() - 1];
            aBorrar.pop_back();
            i--;
        }
        ultimoSinBorrar->siguientes[int(clave[i])] = NULL;
    }
    _size--;
}

template<typename T>
int string_map<T>::cantidadDeNulls(vector<Nodo* > v) {
    int suma = 0;
    for(int i = 0; i < 256; i++){
        if(v[i] == NULL){
            suma++;
        }
    }
    return suma;
}

template <typename T>
int string_map<T>::size() const{
    return sizeRecursivo(raiz);
}

template<typename T>
int string_map<T>::sizeRecursivo(Nodo *n) const {
    Nodo* actual = n;
    int suma = 0;
    if(n == NULL){
        return 0;
    }
    if(actual->definicion != NULL){
        suma++;
    }
    int i = 0;
    while(i < 256){
        suma += sizeRecursivo(actual->siguientes[i]);
        i++;
    }
    return suma;
}

template <typename T>
bool string_map<T>::empty() const{
    return size() == 0;
}

template<typename T>
typename string_map<T>::iterator string_map<T>::begin() {
    return _listaIteradores.begin();
}

template<typename T>
typename string_map<T>::iterator string_map<T>::end() {
    return _listaIteradores.end();
}




