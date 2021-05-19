#ifndef STRING_MAP_H_
#define STRING_MAP_H_

#include <string>
#include <vector>
#include "modulos_basicos/linear_map.h"

using namespace std;

template<typename T>
class string_map {
public:
    using iterator = typename linear_map<const string, T* >::iterator;

    /**
    CONSTRUCTOR
    * Construye un diccionario vacio.
    **/
    string_map();

    /**
    CONSTRUCTOR POR COPIA
    * Construye un diccionario por copia.
    **/
    string_map(const string_map<T>& aCopiar);

    /**
    OPERADOR ASIGNACION
     */
    string_map& operator=(const string_map& d);

    /**
    DESTRUCTOR
    **/
    ~string_map();

    /**
    operator[]
    * Acceso o definición de pares clave/valor
    **/

    //pair<iterator, T &> operator[](const string &key);

    T& operator[](const string &key);

    iterator definir(const string &key, const T& significado);

    /**
    COUNT
    * Devuelve la cantidad de apariciones de la clave (0 o 1).
    * Sirve para identificar si una clave está definida o no.
    **/

    int count(const string &key) const;

    /**
    AT
    * Dada una clave, devuelve su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    -- Versión modificable y no modificable
    **/
    const T& at(const string& key) const;
    T& at(const string& key);

    /**
    ERASE
    * Dada una clave, la borra del diccionario junto a su significado.
    * PRE: La clave está definida.
    --PRODUCE ALIASING--
    **/
    void erase(const string& key);

    /**
     SIZE
     * Devuelve cantidad de claves definidas */
    int size() const;

    /**
     EMPTY
     * devuelve true si no hay ningún elemento en el diccionario */
    bool empty() const;

    iterator begin();

    iterator end();

private:

    struct Nodo {
        vector<Nodo*> siguientes;
        T* definicion;
        Nodo() : siguientes(256, NULL), definicion(NULL) {
        }

        Nodo(T* def) : siguientes(256, NULL), definicion(def) {
        }
    };

    Nodo* raiz;
    int _size;
    linear_map<const string, T* > _listaIteradores;

    //Funciones auxiliares.
    int sizeRecursivo(Nodo* n) const;
    int cantidadDeNulls(vector<Nodo* > v);
    void borrarRecursivo(Nodo* n);
    void copiarRecursivo(Nodo* n, const Nodo *d);
};

#include "string_map.hpp"

#endif // STRING_MAP_H_
