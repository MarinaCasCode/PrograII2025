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
private:
    void** datosCreados;
    int capacidad;
    int cantidad;

    void expandirCapacidad() {
        int nuevaCapacidad = capacidad * 3;
        void** nuevoArray = new void*[nuevaCapacidad];
        
        for (int i = 0; i < cantidad; i++) {
            nuevoArray[i] = datosCreados[i];
        }
        
        delete[] datosCreados;
        datosCreados = nuevoArray;
        capacidad = nuevaCapacidad;
    }

public:
    ArbolRB(string T) : ArbolSVG(T) {
        capacidad = 5;
        cantidad = 0;
        datosCreados = new void*[capacidad];
    }

    ~ArbolRB() {
        for (int i = 0; i < cantidad; i++) {
            if (T == "int") {
                delete (int*)datosCreados[i];
            } else if (T == "float") {
                delete (float*)datosCreados[i];
            } else if (T == "double") {
                delete (double*)datosCreados[i];
            } else if (T == "string") {
                delete (string*)datosCreados[i];
            }
        }
        delete[] datosCreados;
    }

    void insert(int valor) {
        int* datoPtr = new int(valor);
        
        if (cantidad >= capacidad) {
            expandirCapacidad();
        }
        
        datosCreados[cantidad++] = datoPtr;
        insert((void*)datoPtr);
    }

    void insert(int* dato) {
        insert((void*)dato);
    }

    // insercion con balanceo automatico rojo-negro
    void insert(void* dato) {
        NodoRB* nuevoNodo = new NodoRB(dato);
        nuevoNodo->T = T;

        // si el arbol esta vacio, la raiz es negra
        if (raiz == NULL) {
            raiz = nuevoNodo;
            ((NodoRB*)raiz)->color = NEGRO;
            return;
        }

        // insercion BST normal
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

        // arreglar violaciones rojo-negro
        fixInsert(nuevoNodo);
    }

    // generar contenido SVG con circulos coloreados
    string getSVGContent() {
        NodoSVG* root = (NodoSVG*)raiz;
        int espacioHorizontal = 50;
        int espacioVertical = 80;

        int contadorX = 1;
        asignarCoordenadas(root, 0, contadorX, espacioHorizontal, espacioVertical);

        stringstream svgContent;
        svgContent << "<svg viewBox=\"0 0 1000 1000\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

        auto dibujarNodo = [&](const auto &self, NodoSVG* node, stringstream& svgContent) -> void {
            if(node == NULL) return;
            
            NodoRB* nodeRB = (NodoRB*)node;

            if(node->izq != NULL){
                svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->izq)->x<<"\" y2=\""<<((NodoSVG*)node->izq)->y<<"\" stroke=\"black\"/>";
            }

            if(node->der != NULL){
                svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->der)->x<<"\" y2=\""<<((NodoSVG*)node->der)->y<<"\" stroke=\"black\"/>";
            }
            
            string colorFill = (nodeRB->color == ROJO) ? "red" : "black";
            string textColor = "white";
            
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
        string filename = "svgTreeViewerTemplate.html";
        if(outfilename.compare(filename) == 0){
            cout << "No se puede sobrescribir el archivo" << filename << endl;
            return -1;
        }

        struct stat fileStat;
        if( ( stat (filename.c_str(), &fileStat) == 0) ){
            // template exists
        }

        ifstream fileIn;
        fileIn.open(filename.c_str(), ios::in | ios::binary );
        if( !fileIn.is_open() ){
            cout << "No se pudo abrir el archivo" << filename << endl;
            return -1;
        }

        stringstream svgHTMLJS;
        svgHTMLJS.str(string());
        svgHTMLJS << fileIn.rdbuf();
        fileIn.close();

        string svgHTMLJS_str = svgHTMLJS.str();
        string svg = getSVGContent();

        string replacePattern = "%s";
        int replacePatternIndex = svgHTMLJS_str.find(replacePattern);
        if( replacePatternIndex == -1 ){
            cout << "No se pudo encontrar el patron de reemplazo." << endl;
            return -1;
        }

        ofstream fileOut;
        fileOut.open(outfilename.c_str(), ios::out | ios::binary );
        if( !fileOut.is_open() ){
            cout << "No se pudo abrir el archivo" << outfilename << endl;
            return -1;
        }
        fileOut << svgHTMLJS_str.substr(0, replacePatternIndex);
        fileOut << svg;
        fileOut << svgHTMLJS_str.substr( replacePatternIndex + replacePattern.length() );
        fileOut.close();

        cout << "Se genero el visualizador '"<< outfilename <<"'." << endl;

        stringstream openBrowser;
        #if _WIN32
        openBrowser << "start " <<  outfilename;
        #elif __APPLE__
        openBrowser << "open " <<  outfilename;
        #elif __linux__
        openBrowser << "xdg-open " <<  outfilename;
        #endif
        string openBrowserStr = openBrowser.str();
        system(openBrowserStr.c_str());

        return 0;
    }

    // Método delete con balanceo rojo-negro
    bool deleteNode(void* dato) {
        NodoRB* nodo = (NodoRB*)search(dato);
        if (nodo == NULL) return false;

        NodoRB* y = nodo;
        NodoRB* x;
        Color colorOriginalY = y->color;

        if (nodo->izq == NULL) {
            x = (NodoRB*)nodo->der;
            reemplazarSubarbol(nodo, nodo->der);
        } else if (nodo->der == NULL) {
            x = (NodoRB*)nodo->izq;
            reemplazarSubarbol(nodo, nodo->izq);
        } else {
            y = (NodoRB*)encontrarMinimo(nodo->der);
            colorOriginalY = y->color;
            x = (NodoRB*)y->der;

            if (y->padre == nodo) {
                if (x != NULL) x->padre = y;
            } else {
                reemplazarSubarbol(y, y->der);
                y->der = nodo->der;
                if (y->der != NULL) y->der->padre = y;
            }

            reemplazarSubarbol(nodo, y);
            y->izq = nodo->izq;
            if (y->izq != NULL) y->izq->padre = y;
            y->color = nodo->color;
        }

        delete nodo;

        if (colorOriginalY == NEGRO) {
            fixDelete(x);
        }

        return true;
    }

private:
    int comparar(void* dato1, void* dato2) {
        if (T == TYPESTR(string)) {
            string* d1 = (string*)dato1;
            string* d2 = (string*)dato2;
            return d1->compare(*d2);
        }
        else if (T == TYPESTR(int) || T == TYPESTR(float) || T == TYPESTR(double)) {
            int* d1 = (int*)dato1;
            int* d2 = (int*)dato2;
            if (*d1 == *d2) return 0;
            else if (*d1 < *d2) return -1;
            else return 1;
        }
        else if (T == "pair") {
            ParClaveValor<int>* p1 = (ParClaveValor<int>*)dato1;
            ParClaveValor<int>* p2 = (ParClaveValor<int>*)dato2;
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

    // arreglar violaciones rojo-negro tras insercion
    void fixInsert(NodoRB* n) {
        // mientras el padre sea rojo, hay violacion
        while (n != raiz && ((NodoRB*)n->padre)->color == ROJO) {
            NodoRB* padre  = getPadre(n);
            NodoRB* abuelo = getAbuelo(n);

            if (padre == abuelo->izq) {
                NodoRB* tio = getTio(n);

                // caso 1: tio rojo - recolorear
                if (tio != NULL && tio->color == ROJO) {
                    padre->color  = NEGRO;
                    tio->color    = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // caso 2: n es hijo derecho - rotar
                    if (n == padre->der) {
                        n = padre;
                        rotarIzquierda(n);
                        padre  = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // caso 3: n es hijo izquierdo - rotar y recolorear
                    padre->color  = NEGRO;
                    abuelo->color = ROJO;
                    rotarDerecha(abuelo);
                }
            } else {
                NodoRB* tio = getTio(n);

                // caso 1: tio rojo - recolorear
                if (tio != NULL && tio->color == ROJO) {
                    padre->color  = NEGRO;
                    tio->color    = NEGRO;
                    abuelo->color = ROJO;
                    n = abuelo;
                } else {
                    // caso 2: n es hijo izquierdo - rotar
                    if (n == padre->izq) {
                        n = padre;
                        rotarDerecha(n);
                        padre  = getPadre(n);
                        abuelo = getAbuelo(n);
                    }
                    // caso 3: n es hijo derecho - rotar y recolorear
                    padre->color  = NEGRO;
                    abuelo->color = ROJO;
                    rotarIzquierda(abuelo);
                }
            }
        }

        ((NodoRB*)raiz)->color = NEGRO;
    }

    void reemplazarSubarbol(Nodo* u, Nodo* v) {
        if (u->padre == NULL) {
            raiz = v;
        } else if (u == u->padre->izq) {
            u->padre->izq = v;
        } else {
            u->padre->der = v;
        }
        if (v != NULL) {
            v->padre = u->padre;
        }
    }

    Nodo* encontrarMinimo(Nodo* nodo) {
        while (nodo->izq != NULL) {
            nodo = nodo->izq;
        }
        return nodo;
    }

    NodoRB* getHermano(NodoRB* n) {
        if (n == NULL || n->padre == NULL) return NULL;
        
        if (n == n->padre->izq) {
            return (NodoRB*)n->padre->der;
        } else {
            return (NodoRB*)n->padre->izq;
        }
    }

    void fixDelete(NodoRB* x) {
        while (x != raiz && (x == NULL || x->color == NEGRO)) {
            if (x == NULL || (x->padre && x == x->padre->izq)) {
                NodoRB* w = (NodoRB*)(x != NULL && x->padre ? x->padre->der : NULL);
                
                if (w == NULL) break;

                // caso 1: hermano es rojo
                if (w->color == ROJO) {
                    w->color = NEGRO;
                    ((NodoRB*)w->padre)->color = ROJO;
                    rotarIzquierda((NodoRB*)w->padre);
                    w = (NodoRB*)(x->padre ? x->padre->der : NULL);
                }

                if (w == NULL) break;

                // caso 2: hermano es negro y ambos hijos son negros
                if ((w->izq == NULL || ((NodoRB*)w->izq)->color == NEGRO) &&
                    (w->der == NULL || ((NodoRB*)w->der)->color == NEGRO)) {
                    w->color = ROJO;
                    x = (NodoRB*)(x != NULL ? x->padre : NULL);
                } else {
                    // caso 3: hermano es negro, hijo izquierdo rojo, hijo derecho negro
                    if (w->der == NULL || ((NodoRB*)w->der)->color == NEGRO) {
                        if (w->izq != NULL) ((NodoRB*)w->izq)->color = NEGRO;
                        w->color = ROJO;
                        rotarDerecha(w);
                        w = (NodoRB*)(x->padre ? x->padre->der : NULL);
                    }

                    // caso 4: hermano es negro, hijo derecho es rojo
                    if (w != NULL) {
                        w->color = (x != NULL && x->padre) ? ((NodoRB*)x->padre)->color : NEGRO;
                        if (x != NULL && x->padre) ((NodoRB*)x->padre)->color = NEGRO;
                        if (w->der != NULL) ((NodoRB*)w->der)->color = NEGRO;
                        if (x != NULL && x->padre) rotarIzquierda((NodoRB*)x->padre);
                    }
                    x = (NodoRB*)raiz;
                }
            } else {
                NodoRB* w = (NodoRB*)(x != NULL && x->padre ? x->padre->izq : NULL);
                
                if (w == NULL) break;

                // caso 1: hermano es rojo
                if (w->color == ROJO) {
                    w->color = NEGRO;
                    ((NodoRB*)w->padre)->color = ROJO;
                    rotarDerecha((NodoRB*)w->padre);
                    w = (NodoRB*)(x->padre ? x->padre->izq : NULL);
                }

                if (w == NULL) break;

                // caso 2: hermano es negro y ambos hijos son negros
                if ((w->der == NULL || ((NodoRB*)w->der)->color == NEGRO) &&
                    (w->izq == NULL || ((NodoRB*)w->izq)->color == NEGRO)) {
                    w->color = ROJO;
                    x = (NodoRB*)(x != NULL ? x->padre : NULL);
                } else {
                    // caso 3: hermano es negro, hijo derecho rojo, hijo izquierdo negro
                    if (w->izq == NULL || ((NodoRB*)w->izq)->color == NEGRO) {
                        if (w->der != NULL) ((NodoRB*)w->der)->color = NEGRO;
                        w->color = ROJO;
                        rotarIzquierda(w);
                        w = (NodoRB*)(x->padre ? x->padre->izq : NULL);
                    }

                    // caso 4: hermano es negro, hijo izquierdo es rojo
                    if (w != NULL) {
                        w->color = (x != NULL && x->padre) ? ((NodoRB*)x->padre)->color : NEGRO;
                        if (x != NULL && x->padre) ((NodoRB*)x->padre)->color = NEGRO;
                        if (w->izq != NULL) ((NodoRB*)w->izq)->color = NEGRO;
                        if (x != NULL && x->padre) rotarDerecha((NodoRB*)x->padre);
                    }
                    x = (NodoRB*)raiz;
                }
            }
        }

        if (x != NULL) x->color = NEGRO;
    }
};

#endif