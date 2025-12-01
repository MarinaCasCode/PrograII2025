#ifndef RBHASH_H
#define RBHASH_H

#include "rbTree.h"
#include <sstream>

using namespace std;

template <typename V>
class RBHash {
private:
    ArbolRB<ParClaveValor<V>> arbol;

    NodoRB<ParClaveValor<V>>* buscarNodo(const string& clave) {
        ParClaveValor<V> temp(clave, V());
        return arbol.searchNode(temp);
    }

public:
    RBHash() : arbol() {}

    V& operator[](const string& clave) {
        NodoRB<ParClaveValor<V>>* nodo = buscarNodo(clave);
        
        if (!nodo) {
            ParClaveValor<V>* nuevoPar = new ParClaveValor<V>(clave, V());
            arbol.insert(nuevoPar, true);
            nodo = buscarNodo(clave);
        }
        
        return nodo->dato->valor;
    }

    int toSVG(const string& outfilename) { 
        return arbol.toSVG(outfilename); 
    }

    string toString() {
        return arbol.toString();
    }

    void imprimir() {
        cout << toString() << endl;
    }
};

#endif