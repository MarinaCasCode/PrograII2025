#ifndef RBHASH_CPP
#define RBHASH_CPP

#include <iostream>
#include <string>
#include "rbTree.cpp"

using namespace std;

template<typename T>
class RBHash {
private:
    ArbolRB* arbol;

public:
    RBHash() {
        arbol = new ArbolRB("ParClaveValor");
    }

    ~RBHash() {
        delete arbol;
    }

    ArbolRB& getArbol() {
        return *arbol;
    }

    T& operator[](const string& clave) {
        // crear objeto temporal para busqueda
        T valorDefault{};
        ParClaveValor<T> temp(clave, valorDefault);
        
        // buscar en el arbol
        Nodo* nodo = arbol->search(&temp);
        
        if (nodo == NULL) {
            // si no existe, crear nuevo par e insertarlo
            T valorInicial{};
            ParClaveValor<T>* nuevoPar = new ParClaveValor<T>(clave, valorInicial);
            arbol->insert(nuevoPar);
            nodo = arbol->search(&temp);
        }

        // retornar referencia al valor
        ParClaveValor<T>* par = (ParClaveValor<T>*)nodo->dato;
        return par->valor;
    }

    void generarSVG(string filename) {
        arbol->toSVG(filename);
    }
};

#endif