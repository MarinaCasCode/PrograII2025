#include "rbTree.h"
#if 0

using namespace std;

enum Color { ROJO, NEGRO };

template<typename T>
class NodoRB : public NodoSVG<T> {
public:
    Color color;

    NodoRB(T* dato = nullptr, bool owns = true) : NodoSVG<T>(dato, owns) {
        postConstructor();
    }

    void postConstructor() {
        this->color = ROJO;
        this->x = 0;
        this->y = 0;
    }
};

template<typename T>
class ArbolRB : public ArbolSVG<T> {
public:
    ArbolRB(const string& tname = string()) : ArbolSVG<T>(tname) {}

    ~ArbolRB() {}

    // soporta insert por copia o puntero
    void insert(const T& v) {
        T* p = new T(v);
        insert(p, true);
    }

    void insert(T* p, bool owns = true) {
        NodoRB<T>* nuevo = new NodoRB<T>(p, owns);

        if (this->raiz == nullptr) {
            this->raiz = nuevo;
            ((NodoRB<T>*)this->raiz)->color = NEGRO;
            return;
        }

        NodoSVG<T>* it = this->raiz;
        NodoSVG<T>* padre = nullptr;

        while (it != nullptr) {
            padre = it;
            if ( *(nuevo->dato) < *(it->dato) ) it = it->izq;
            else it = it->der;
        }

        if ( *(nuevo->dato) < *(padre->dato) ) padre->izq = nuevo;
        else padre->der = nuevo;

        nuevo->padre = padre;

        fixInsert(nuevo);
    }

    // buscar nodo por valor
    NodoRB<T>* searchNode(const T& v) {
        NodoSVG<T>* it = this->raiz;
        while (it != nullptr) {
            if (*(it->dato) == v) return (NodoRB<T>*)it;
            if (v < *(it->dato)) it = it->izq;
            else it = it->der;
        }
        return nullptr;
    }

    // override para generar circulos coloreados
    string getSVGContent() override {
        NodoSVG<T>* root = this->raiz;
        int espacioHorizontal = 50;
        int espacioVertical = 80;

        int contadorX = 1;
        this->asignarCoordenadas(root, 0, contadorX, espacioHorizontal, espacioVertical);

        stringstream svgContent;
        svgContent << "<svg viewBox=\"0 0 2000 2000\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

        function<void(NodoSVG<T>*)> dibujar = [&](NodoSVG<T>* node) {
            if (!node) return;
            NodoRB<T>* nodeRB = (NodoRB<T>*)node;

            if (node->izq) {
                svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y << "\" x2=\"" << node->izq->x << "\" y2=\"" << node->izq->y << "\" stroke=\"black\"/>";
            }
            if (node->der) {
                svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y << "\" x2=\"" << node->der->x << "\" y2=\"" << node->der->y << "\" stroke=\"black\"/>";
            }

            string colorFill = (nodeRB->color == ROJO) ? "red" : "black";
            string textColor = "white";

            svgContent << "<circle cx=\"" << node->x << "\" cy=\"" << node->y << "\" r=\"20\" fill=\"" << colorFill << "\" stroke=\"black\"/>";
            svgContent << "<text x=\"" << node->x << "\" y=\"" << (node->y + 5) << "\" text-anchor=\"middle\" font-size=\"12\" fill=\"" << textColor << "\">" << node->toString() << "</text>";

            dibujar(node->izq);
            dibujar(node->der);
        };

        dibujar(root);
        svgContent << "</svg>";
        return svgContent.str();
    }

    bool deleteNode(const T& v) {
        NodoRB<T>* nodo = searchNode(v);
        if (!nodo) return false;

        NodoRB<T>* y = nodo;
        NodoRB<T>* x = nullptr;
        Color colorOriginalY = y->color;

        if (nodo->izq == nullptr) {
            x = (NodoRB<T>*)nodo->der;
            reemplazarSubarbol(nodo, nodo->der);
        } else if (nodo->der == nullptr) {
            x = (NodoRB<T>*)nodo->izq;
            reemplazarSubarbol(nodo, nodo->izq);
        } else {
            y = (NodoRB<T>*)encontrarMinimo(nodo->der);
            colorOriginalY = y->color;
            x = (NodoRB<T>*)y->der;

            if (y->padre == nodo) {
                if (x) x->padre = y;
            } else {
                reemplazarSubarbol(y, y->der);
                y->der = nodo->der;
                if (y->der) y->der->padre = y;
            }

            reemplazarSubarbol(nodo, y);
            y->izq = nodo->izq;
            if (y->izq) y->izq->padre = y;
            y->color = nodo->color;
        }

        // liberar datos si corresponde
        if (nodo->ownsData && nodo->dato) delete nodo->dato;
        delete nodo;

        if (colorOriginalY == NEGRO) {
            fixDelete(x);
        }

        return true;
    }

private:
    NodoRB<T>* getPadre(NodoRB<T>* n) { return (NodoRB<T>*)n->padre; }

    NodoRB<T>* getAbuelo(NodoRB<T>* n) {
        NodoRB<T>* p = getPadre(n);
        if (!p) return nullptr;
        return (NodoRB<T>*)p->padre;
    }

    NodoRB<T>* getTio(NodoRB<T>* n) {
        NodoRB<T>* abuelo = getAbuelo(n);
        if (!abuelo) return nullptr;
        NodoRB<T>* padre = getPadre(n);
        if (padre == abuelo->izq) return (NodoRB<T>*)abuelo->der;
        else return (NodoRB<T>*)abuelo->izq;
    }

    void rotarIzquierda(NodoRB<T>* x) {
        NodoRB<T>* y = (NodoRB<T>*)x->der;
        x->der = y->izq;
        if (y->izq) y->izq->padre = x;
        NodoSVG<T>* padre_y = x->padre;
        y->padre = padre_y;
        if (padre_y == nullptr) this->raiz = y;
        else if (x == padre_y->izq) padre_y->izq = y;
        else padre_y->der = y;
        y->izq = x;
        x->padre = y;
    }

    void rotarDerecha(NodoRB<T>* y) {
        NodoRB<T>* x = (NodoRB<T>*)y->izq;
        y->izq = x->der;
        if (x->der) x->der->padre = y;
        NodoSVG<T>* padre_x = y->padre;
        x->padre = padre_x;
        if (padre_x == nullptr) this->raiz = x;
        else if (y == padre_x->izq) padre_x->izq = x;
        else padre_x->der = x;
        x->der = y;
        y->padre = x;
    }

    void fixInsert(NodoRB<T>* n) {
        while (n != this->raiz && ((NodoRB<T>*)n->padre)->color == ROJO) {
            NodoRB<T>* padre = getPadre(n);
            NodoRB<T>* abuelo = getAbuelo(n);
            if (padre == abuelo->izq) {
                NodoRB<T>* tio = getTio(n);
                if (tio && tio->color == ROJO) {
                    padre->color = NEGRO; tio->color = NEGRO; abuelo->color = ROJO; n = abuelo;
                } else {
                    if (n == padre->der) { n = padre; rotarIzquierda(n); padre = getPadre(n); abuelo = getAbuelo(n); }
                    padre->color = NEGRO; abuelo->color = ROJO; rotarDerecha(abuelo);
                }
            } else {
                NodoRB<T>* tio = getTio(n);
                if (tio && tio->color == ROJO) {
                    padre->color = NEGRO; tio->color = NEGRO; abuelo->color = ROJO; n = abuelo;
                } else {
                    if (n == padre->izq) { n = padre; rotarDerecha(n); padre = getPadre(n); abuelo = getAbuelo(n); }
                    padre->color = NEGRO; abuelo->color = ROJO; rotarIzquierda(abuelo);
                }
            }
        }
        ((NodoRB<T>*)this->raiz)->color = NEGRO;
    }

    void reemplazarSubarbol(NodoSVG<T>* u, NodoSVG<T>* v) {
        if (u->padre == nullptr) this->raiz = v;
        else if (u == u->padre->izq) u->padre->izq = v;
        else u->padre->der = v;
        if (v) v->padre = u->padre;
    }

    NodoSVG<T>* encontrarMinimo(NodoSVG<T>* nodo) {
        while (nodo->izq != nullptr) nodo = nodo->izq;
        return nodo;
    }

    void fixDelete(NodoRB<T>* x) {
        while (x != this->raiz && (x == nullptr || x->color == NEGRO)) {
            if (x == nullptr || (x->padre && x == x->padre->izq)) {
                NodoRB<T>* w = (NodoRB<T>*)(x != nullptr && x->padre ? x->padre->der : nullptr);
                if (!w) break;
                if (w->color == ROJO) { w->color = NEGRO; ((NodoRB<T>*)w->padre)->color = ROJO; rotarIzquierda((NodoRB<T>*)w->padre); w = (NodoRB<T>*)(x->padre ? x->padre->der : nullptr); }
                if (!w) break;
                if ((w->izq == nullptr || ((NodoRB<T>*)w->izq)->color == NEGRO) && (w->der == nullptr || ((NodoRB<T>*)w->der)->color == NEGRO)) {
                    w->color = ROJO; x = (NodoRB<T>*)(x != nullptr ? x->padre : nullptr);
                } else {
                    if (w->der == nullptr || ((NodoRB<T>*)w->der)->color == NEGRO) {
                        if (w->izq) ((NodoRB<T>*)w->izq)->color = NEGRO;
                        w->color = ROJO; rotarDerecha(w); w = (NodoRB<T>*)(x->padre ? x->padre->der : nullptr);
                    }
                    if (w) {
                        w->color = (x != nullptr && x->padre) ? ((NodoRB<T>*)x->padre)->color : NEGRO;
                        if (x != nullptr && x->padre) ((NodoRB<T>*)x->padre)->color = NEGRO;
                        if (w->der) ((NodoRB<T>*)w->der)->color = NEGRO;
                        if (x != nullptr && x->padre) rotarIzquierda((NodoRB<T>*)x->padre);
                    }
                    x = (NodoRB<T>*)this->raiz;
                }
            } else {
                NodoRB<T>* w = (NodoRB<T>*)(x != nullptr && x->padre ? x->padre->izq : nullptr);
                if (!w) break;
                if (w->color == ROJO) { w->color = NEGRO; ((NodoRB<T>*)w->padre)->color = ROJO; rotarDerecha((NodoRB<T>*)w->padre); w = (NodoRB<T>*)(x->padre ? x->padre->izq : nullptr); }
                if (!w) break;
                if ((w->der == nullptr || ((NodoRB<T>*)w->der)->color == NEGRO) && (w->izq == nullptr || ((NodoRB<T>*)w->izq)->color == NEGRO)) {
                    w->color = ROJO; x = (NodoRB<T>*)(x != nullptr ? x->padre : nullptr);
                } else {
                    if (w->izq == nullptr || ((NodoRB<T>*)w->izq)->color == NEGRO) {
                        if (w->der) ((NodoRB<T>*)w->der)->color = NEGRO; w->color = ROJO; rotarIzquierda(w); w = (NodoRB<T>*)(x->padre ? x->padre->izq : nullptr);
                    }
                    if (w) {
                        w->color = (x != nullptr && x->padre) ? ((NodoRB<T>*)x->padre)->color : NEGRO;
                        if (x != nullptr && x->padre) ((NodoRB<T>*)x->padre)->color = NEGRO;
                        if (w->izq) ((NodoRB<T>*)w->izq)->color = NEGRO;
                        if (x != nullptr && x->padre) rotarDerecha((NodoRB<T>*)x->padre);
                    }
                    x = (NodoRB<T>*)this->raiz;
                }
            }
        }
        if (x) x->color = NEGRO;
    }
};

#endif