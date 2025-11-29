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
        NodoRB* y = (NodoRB*)x->der;
        x->der = y->izq;

        if(y->izq != NULL) {
            y->izq->padre = x;
        }

        Nodo* padre_y = x->padre;
        y->padre = padre_y;

        if(padre_y == NULL) {
            raiz = y;
        } else if(x == padre_y->izq) {
            padre_y->izq = y;
        } else {
            padre_y->der = y;
        }

        y->izq = x;
        x->padre = y;
    }

    void rotarDerecha(NodoRB* y) {
        NodoRB* x = (NodoRB*)y->izq;
        y->izq = x->der;

        if(x->der != NULL) {
            x->der->padre = y;
        }

        Nodo* padre_x = y->padre;
        x->padre = padre_x;

        if(padre_x == NULL) {
            raiz = x;
        } else if(y == padre_x->izq) {
            padre_x->izq = x;
        } else {
            padre_x->der = x;
        }

        x->der = y;
        y->padre = x;
    }

    void fixInsert(NodoRB* n) {
        // implementacion del fixInsert
    }
};

#endif