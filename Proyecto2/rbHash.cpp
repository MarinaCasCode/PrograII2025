#ifndef RBHASH_CPP
#define RBHASH_CPP

#include <iostream>
#include <string>
#include "rbTree.cpp"

using namespace std;

template<typename T>
class ParClaveValor : public ParClaveValorBase {
public:
    T valor;

    ParClaveValor() : ParClaveValorBase(""), valor() { }

    ParClaveValor(const string& k, const T& v)
        : ParClaveValorBase(k), valor(v) { }
};

template<typename T>
class RBHash {
private:
    ArbolRB arbol;

    NodoRB* buscarNodo(const string& clave) {
        T valorPorDefecto = 0;
        ParClaveValor<T> temp(clave, valorPorDefecto);

        Nodo* nodo = arbol.search((void*)&temp);
        return (NodoRB*)nodo;
    }

public:
    ArbolRB& getArbol() {
        return arbol;
    }

    RBHash() : arbol("pair") { }

    T& operator[](const string& clave) {
        NodoRB* nodo = buscarNodo(clave);

        if (nodo == NULL) {
            T valorPorDefecto = 0;
            ParClaveValor<T>* nuevoPar = new ParClaveValor<T>(clave, valorPorDefecto);
            arbol.insert((void*)nuevoPar);
            nodo = buscarNodo(clave);
        }

        ParClaveValor<T>* par = (ParClaveValor<T>*)nodo->dato;
        return par->valor;
    }
};

#endif
