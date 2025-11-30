#ifndef RBTREE_CPP
#define RBTREE_CPP

#include <iostream>
#include "bstTree.cpp"

using namespace std;

enum Color { ROJO, NEGRO };

class ParClaveValorBase {
public:
    string clave;

    ParClaveValorBase() : clave("") { }
    ParClaveValorBase(const string& k) : clave(k) { }
};

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

        if (raiz == NULL) {
            raiz = nuevoNodo;
            ((NodoRB*)raiz)->color = NEGRO;
            return;
        }

        Nodo* it = raiz;
        Nodo* p  = NULL;
        char donde = 'D';

        while (it != NULL) {
            p = it;
            int cmp = comparar(dato, it->dato);

            if (cmp < 0) {
                it = it->izq;
                donde = 'I';
            } else {
                it = it->der;
                donde = 'D';
            }
        }

        if (donde == 'I') {
            p->izq = nuevoNodo;
        } else {
            p->der = nuevoNodo;
        }

        nuevoNodo->padre = p;

        fixInsert(nuevoNodo);
    }

    // sobreescribir toSVG para mostrar colores
    string toSVG() {
        NodoSVG* root = (NodoSVG*)raiz;
        int svgWidth = 800;
        int svgHeight = 600;
        int horizontalSpacing = 50;
        int verticalSpacing = 80;

        int xRef = 1;
        asignarCoordenadas(root, 0, xRef, horizontalSpacing, verticalSpacing);

        stringstream svgContent;
        svgContent << "<svg viewBox=\"0 0 200 200\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

        auto dibujarNodo = [&](const auto &self, NodoSVG* node, stringstream& svgContent) -> void {
            if(node == NULL) return;
            
            NodoRB* nodeRB = (NodoRB*)node;

            if(node->izq != NULL){
                svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->izq)->x<<"\" y2=\""<<((NodoSVG*)node->izq)->y<<"\" stroke=\"black\"/>";
            }
            
            if(node->der != NULL){
                svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->der)->x<<"\" y2=\""<<((NodoSVG*)node->der)->y<<"\" stroke=\"black\"/>";
            }
            
            // usar color rojo o negro segun el nodo RB
            string colorFill = (nodeRB->color == ROJO) ? "red" : "black";
            string textColor = (nodeRB->color == ROJO) ? "white" : "white";
            
            svgContent << "<circle cx=\""<<node->x<<"\" cy=\""<<node->y<<"\" r=\"20\" fill=\""<<colorFill<<"\" stroke=\"black\"/>";
            svgContent << "<text x=\""<<node->x<<"\" y=\""<<(node->y + 5)<<"\" text-anchor=\"middle\" font-size=\"12\" fill=\""<<textColor<<"\">"<<node->toString()<<"</text>";
            
            self(self, (NodoSVG*)node->izq, svgContent);
            self(self, (NodoSVG*)node->der, svgContent);
        };

        dibujarNodo(dibujarNodo, root, svgContent);
        svgContent << "</svg>";

        return svgContent.str();
    }

    int toSVG(string outfilename) {
        string svg = toSVG();
        return ArbolSVG::toSVG(outfilename);
    }

    // búsqueda binaria usando comparar()
    Nodo* search(void* dato) {
        Nodo* it = raiz;

        while (it != NULL) {
            int cmp = comparar(dato, it->dato);

            if (cmp == 0) {
                return it;
            } else if (cmp < 0) {
                it = it->izq;
            } else {
                it = it->der;
            }
        }
        return NULL;
    }


private:
    int comparar(void* dato1, void* dato2) {
        if (T == T(string)) {
            string* d1 = (string*)dato1;
            string* d2 = (string*)dato2;
            return d1->compare(*d2);
        }
        else if (T == T(int) || T == T(float) || T == T(double)) {
            int* d1 = (int*)dato1;
            int* d2 = (int*)dato2;
            if (*d1 == *d2) return 0;
            else if (*d1 < *d2) return -1;
            else return 1;
        }
        else if (T == "pair") {
            ParClaveValorBase* p1 = (ParClaveValorBase*)dato1;
            ParClaveValorBase* p2 = (ParClaveValorBase*)dato2;
            return p1->clave.compare(p2->clave);
        }
        else {
            if (dato1 == dato2) return 0;
            else if (dato1 < dato2) return -1;
            else return 1;
        }
    }

    NodoRB* getPadre(NodoRB* n) {
        return (NodoRB*)n->padre;
    }

    NodoRB* getAbuelo(NodoRB* n) {
        NodoRB* p = getPadre(n);
        if (p == NULL) return NULL;
        return (NodoRB*)p->padre;
    }

    NodoRB* getTio(NodoRB* n) {
        NodoRB* abuelo = getAbuelo(n);
        if (abuelo == NULL) return NULL;

        NodoRB* padre = getPadre(n);
        if (padre == abuelo->izq) {
            return (NodoRB*)abuelo->der;
        } else {
            return (NodoRB*)abuelo->izq;
        }
    }

    void rotarIzquierda(NodoRB* x) {
        NodoRB* y = (NodoRB*)x->der;
        x->der = y->izq;

        if (y->izq != NULL) {
            y->izq->padre = x;
        }

        Nodo* padre_y = x->padre;
        y->padre = padre_y;

        if (padre_y == NULL) {
            raiz = y;
        } else if (x == padre_y->izq) {
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

        if (x->der != NULL) {
            x->der->padre = y;
        }

        Nodo* padre_x = y->padre;
        x->padre = padre_x;

        if (padre_x == NULL) {
            raiz = x;
        } else if (y == padre_x->izq) {
            padre_x->izq = x;
        } else {
            padre_x->der = x;
        }

        x->der = y;
        y->padre = x;
    }

    void fixInsert(NodoRB* n) {
        while (n != raiz && ((NodoRB*)n->padre)->color == ROJO) {
            NodoRB* padre  = getPadre(n);
            NodoRB* abuelo = getAbuelo(n);

            if (padre == abuelo->izq) {
                NodoRB* tio = getTio(n);

                // caso 1: tio rojo
                if (tio != NULL && tio->color == ROJO) {
                    padre->color  = NEGRO;
                    tio->color    = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // caso 2: n es hijo derecho
                    if (n == padre->der) {
                        n = padre;
                        rotarIzquierda(n);
                        padre  = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // caso 3: n es hijo izquierdo
                    padre->color  = NEGRO;
                    abuelo->color = ROJO;
                    rotarDerecha(abuelo);
                }
            } else {
                NodoRB* tio = getTio(n);

                if (tio != NULL && tio->color == ROJO) {
                    padre->color  = NEGRO;
                    tio->color    = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // caso 2: n es hijo izquierdo
                    if (n == padre->izq) {
                        n = padre;
                        rotarDerecha(n);
                        padre  = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // caso 3: n es hijo derecho
                    padre->color  = NEGRO;
                    abuelo->color = ROJO;
                    rotarIzquierda(abuelo);
                }
            }
        }

        ((NodoRB*)raiz)->color = NEGRO;
    }
};

#endif
