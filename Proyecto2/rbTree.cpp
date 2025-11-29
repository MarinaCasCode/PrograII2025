#ifndef RBTREE_CPP
#define RBTREE_CPP

#include <iostream>
#include "bstTree.cpp"

using namespace std;

enum Color { ROJO, NEGRO };

class NodoRB : public NodoSVG {
public:
    Color color;

    NodoRB(void* dato) : NodoSVG(dato) {
        postConstructor();
    }

    void postConstructor() {
        this->color = ROJO;
        this->x = 0;
        this->y = 0;
    }
};

class ArbolRB : public ArbolSVG {
public:
    ArbolRB(string T) : ArbolSVG(T) { }

    void insert(void* dato) {
        NodoRB* nuevoNodo = new NodoRB(dato);
        nuevoNodo->T = T;

        if(raiz == NULL) {
            raiz = nuevoNodo;
            ((NodoRB*)raiz)->color = NEGRO;
            return;
        }

        Nodo* it = raiz;
        Nodo* p = NULL;
        char donde = 'D';

        while(it != NULL) {
            p = it;
            if(T == "string") {
                string* d1 = (string*)dato;
                string* d2 = (string*)it->dato;
                if(d1->compare(*d2) < 0) {
                    it = it->izq;
                    donde = 'I';
                } else {
                    it = it->der;
                    donde = 'D';
                }
            } else if(T == "int" || T == "float" || T == "double") {
                int* d1 = (int*)dato;
                int* d2 = (int*)it->dato;
                if(*d1 < *d2) {
                    it = it->izq;
                    donde = 'I';
                } else {
                    it = it->der;
                    donde = 'D';
                }
            } else {
                if(dato < it->dato) {
                    it = it->izq;
                    donde = 'I';
                } else {
                    it = it->der;
                    donde = 'D';
                }
            }
        }

        if(donde == 'I') {
            p->izq = nuevoNodo;
        } else {
            p->der = nuevoNodo;
        }
        nuevoNodo->padre = p;

        fixInsert((NodoRB*)nuevoNodo);
    }

private:
    void rotarIzquierda(NodoRB* x) {
        // implementacion basica de rotacion izquierda
    }

    void rotarDerecha(NodoRB* y) {
        // implementacin de rotacion derecha  
    }

    void fixInsert(NodoRB* n) {
        // implementacion del fixInsert
    }
};

#endif