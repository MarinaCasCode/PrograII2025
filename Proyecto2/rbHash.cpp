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
        // Liberar todos los ParClaveValor creados
        liberarMemoria(arbol->raiz);
        // Liberar el árbol
        delete arbol;
    }

    // Operador de asignación
    RBHash& operator=(const RBHash& otro) {
        if (this != &otro) {
            // Liberar todos los ParClaveValor del árbol actual
            liberarMemoria(arbol->raiz);
            // Eliminar el árbol actual
            delete arbol;
            
            // Crear un nuevo árbol
            arbol = new ArbolRB("ParClaveValor");
            
            // Copiar todos los elementos del otro hash
            copiarElementos(otro.arbol->raiz);
        }
        return *this;
    }

    // Operador de comparación
    bool operator==(const RBHash& otro) const {
        // Comparar verificando que cada elemento del hash esté en el otro
        return verificarIgualdad(arbol->raiz, otro) && 
               verificarIgualdad(otro.arbol->raiz, *this);
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

private:
    // Función auxiliar para liberar memoria de todos los ParClaveValor
    void liberarMemoria(Nodo* nodo) {
        if (nodo == NULL) return;
        
        // Liberar recursivamente los subárboles primero
        liberarMemoria(nodo->izq);
        liberarMemoria(nodo->der);
        
        // Liberar el ParClaveValor del nodo actual
        ParClaveValor<T>* par = (ParClaveValor<T>*)nodo->dato;
        delete par;
    }
    
    // Función auxiliar para copiar elementos de un árbol a otro
    void copiarElementos(Nodo* nodo) {
        if (nodo == NULL) return;
        
        // Copiar el nodo actual
        ParClaveValor<T>* parOriginal = (ParClaveValor<T>*)nodo->dato;
        ParClaveValor<T>* parCopia = new ParClaveValor<T>(parOriginal->clave, parOriginal->valor);
        arbol->insert(parCopia);
        
        // Copiar recursivamente los subárboles
        copiarElementos(nodo->izq);
        copiarElementos(nodo->der);
    }
    
    // Función auxiliar para verificar que todos los elementos de un hash están en otro
    bool verificarIgualdad(Nodo* nodo, const RBHash& otro) const {
        if (nodo == NULL) return true;
        
        // Obtener el par clave-valor del nodo actual
        ParClaveValor<T>* par = (ParClaveValor<T>*)nodo->dato;
        
        // Buscar este par en el otro hash
        T valorDefault{};
        ParClaveValor<T> temp(par->clave, valorDefault);
        Nodo* nodoEncontrado = otro.arbol->search(&temp);
        
        // Si no se encuentra o el valor es diferente, no son iguales
        if (nodoEncontrado == NULL) return false;
        
        ParClaveValor<T>* parEncontrado = (ParClaveValor<T>*)nodoEncontrado->dato;
        if (parEncontrado->valor != par->valor) return false;
        
        // Verificar recursivamente los subárboles
        return verificarIgualdad(nodo->izq, otro) && 
               verificarIgualdad(nodo->der, otro);
    }
};

#endif