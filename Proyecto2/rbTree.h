#ifndef RBTREE_H
#define RBTREE_H

#include "bstTree.h"

using namespace std;

enum Color { ROJO, NEGRO };

// ==================== NODO ROJO-NEGRO ====================
template<typename T>
class NodoRB : public NodoSVG<T> {
public:
    Color color;
    
    NodoRB(T* dato = nullptr, bool owns = true) : NodoSVG<T>(dato, owns) {
        postConstructor();
    }
    
    void postConstructor() override {
        this->color = ROJO;
        this->x = 0;
        this->y = 0;
    }
    
    string toString() override {
        stringstream ss;
        string colorStr = (color == ROJO) ? "R" : "N";
        ss << Nodo<T>::toString() << "(" << colorStr << ")";
        return ss.str();
    }
};

// ==================== ARBOL ROJO-NEGRO ====================
template<typename T>
class ArbolRB : public ArbolSVG<T> {
private:
    NodoRB<T>* getPadre(NodoRB<T>* n) {
        return static_cast<NodoRB<T>*>(n->padre);
    }
    
    NodoRB<T>* getAbuelo(NodoRB<T>* n) {
        NodoRB<T>* p = getPadre(n);
        if (!p) return nullptr;
        return static_cast<NodoRB<T>*>(p->padre);
    }
    
    NodoRB<T>* getTio(NodoRB<T>* n) {
        NodoRB<T>* abuelo = getAbuelo(n);
        if (!abuelo) return nullptr;
        
        NodoRB<T>* padre = getPadre(n);
        if (padre == abuelo->izq) {
            return static_cast<NodoRB<T>*>(abuelo->der);
        } else {
            return static_cast<NodoRB<T>*>(abuelo->izq);
        }
    }
    
    void rotarIzquierda(NodoRB<T>* x) {
        NodoRB<T>* y = static_cast<NodoRB<T>*>(x->der);
        x->der = y->izq;
        
        if (y->izq) {
            y->izq->padre = x;
        }
        
        y->padre = x->padre;
        
        if (!x->padre) {
            this->raiz = y;
        } else if (x == x->padre->izq) {
            x->padre->izq = y;
        } else {
            x->padre->der = y;
        }
        
        y->izq = x;
        x->padre = y;
    }
    
    void rotarDerecha(NodoRB<T>* y) {
        NodoRB<T>* x = static_cast<NodoRB<T>*>(y->izq);
        y->izq = x->der;
        
        if (x->der) {
            x->der->padre = y;
        }
        
        x->padre = y->padre;
        
        if (!y->padre) {
            this->raiz = x;
        } else if (y == y->padre->izq) {
            y->padre->izq = x;
        } else {
            y->padre->der = x;
        }
        
        x->der = y;
        y->padre = x;
    }
    
    void fixInsert(NodoRB<T>* n) {
        while (n != this->raiz && getPadre(n)->color == ROJO) {
            NodoRB<T>* padre = getPadre(n);
            NodoRB<T>* abuelo = getAbuelo(n);
            
            if (padre == abuelo->izq) {
                NodoRB<T>* tio = getTio(n);
                
                // Caso 1: tio rojo
                if (tio && tio->color == ROJO) {
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // Caso 2: n es hijo derecho
                    if (n == padre->der) {
                        n = padre;
                        rotarIzquierda(n);
                        padre = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // Caso 3: n es hijo izquierdo
                    padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotarDerecha(abuelo);
                }
            } else {
                NodoRB<T>* tio = getTio(n);
                
                // Caso 1: tio rojo
                if (tio && tio->color == ROJO) {
                    padre->color = NEGRO;
                    tio->color = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // Caso 2: n es hijo izquierdo
                    if (n == padre->izq) {
                        n = padre;
                        rotarDerecha(n);
                        padre = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // Caso 3: n es hijo derecho
                    padre->color = NEGRO;
                    abuelo->color = ROJO;
                    rotarIzquierda(abuelo);
                }
            }
        }
        
        static_cast<NodoRB<T>*>(this->raiz)->color = NEGRO;
    }

public:
    ArbolRB() : ArbolSVG<T>() {}
    
    void insert(T* dato, bool owns = true) override {
        NodoRB<T>* nuevo = new NodoRB<T>(dato, owns);
        
        if (!this->raiz) {
            this->raiz = nuevo;
            nuevo->color = NEGRO;
            return;
        }
        
        Nodo<T>* it = this->raiz;
        Nodo<T>* padre = nullptr;
        
        while (it) {
            padre = it;
            if (*dato < *(it->dato)) {
                it = it->izq;
            } else {
                it = it->der;
            }
        }
        
        if (*dato < *(padre->dato)) {
            padre->izq = nuevo;
        } else {
            padre->der = nuevo;
        }
        nuevo->padre = padre;
        
        fixInsert(nuevo);
    }
    
    NodoRB<T>* searchNode(const T& v) {
        return static_cast<NodoRB<T>*>(this->search(v));
    }
    
    string getSVGContent() override {
        NodoSVG<T>* root = static_cast<NodoSVG<T>*>(this->raiz);
        if (!root) return "";
        
        int espacioHorizontal = 50;
        int espacioVertical = 80;
        int contadorX = 1;
        
        this->asignarCoordenadas(root, 0, contadorX, espacioHorizontal, espacioVertical);
        
        stringstream svgContent;
        svgContent << "<svg viewBox=\"0 0 2000 2000\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";
        
        function<void(NodoRB<T>*)> dibujar = [&](NodoRB<T>* node) {
            if (!node) return;
            
            if (node->izq) {
                NodoRB<T>* izq = static_cast<NodoRB<T>*>(node->izq);
                svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y 
                          << "\" x2=\"" << izq->x << "\" y2=\"" << izq->y << "\" stroke=\"black\"/>";
            }
            if (node->der) {
                NodoRB<T>* der = static_cast<NodoRB<T>*>(node->der);
                svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y 
                          << "\" x2=\"" << der->x << "\" y2=\"" << der->y << "\" stroke=\"black\"/>";
            }
            
            string colorFill = (node->color == ROJO) ? "red" : "black";
            string textColor = "white";
            
            svgContent << "<circle cx=\"" << node->x << "\" cy=\"" << node->y 
                      << "\" r=\"20\" fill=\"" << colorFill << "\" stroke=\"black\"/>";
            svgContent << "<text x=\"" << node->x << "\" y=\"" << (node->y + 5) 
                      << "\" text-anchor=\"middle\" font-size=\"12\" fill=\"" << textColor << "\">" 
                      << node->toString() << "</text>";
            
            dibujar(static_cast<NodoRB<T>*>(node->izq));
            dibujar(static_cast<NodoRB<T>*>(node->der));
        };
        
        dibujar(static_cast<NodoRB<T>*>(root));
        svgContent << "</svg>";
        return svgContent.str();
    }
};

#endif