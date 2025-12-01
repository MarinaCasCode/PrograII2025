#ifndef BSTTREE_H
#define BSTTREE_H

#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>
#include <string>

using namespace std;
// definiciones basicas: par clave-valor, nodos y arboles

// par clave-valor: almacena una clave (string) y un valor generico
template<typename V>
class ParClaveValor {
public:
    string clave;
    V valor;

    ParClaveValor() : clave(""), valor() {}
    ParClaveValor(const string& k, const V& v) : clave(k), valor(v) {}
    
    bool operator<(const ParClaveValor& otro) const {
        return clave < otro.clave;
    }
    
    bool operator>(const ParClaveValor& otro) const {
        return clave > otro.clave;
    }
    
    bool operator==(const ParClaveValor& otro) const {
        return clave == otro.clave;
    }
    
    bool operator!=(const ParClaveValor& otro) const {
        return clave != otro.clave;
    }
    
    string toString() const {
        stringstream ss;
        ss << clave << ":" << valor;
        return ss.str();
    }
};


// nodo generico del arbol: contiene puntero a dato, hijos y padre
template<typename T>
class Nodo {
public:
    T* dato;
    Nodo<T>* izq;
    Nodo<T>* der;
    Nodo<T>* padre;
    bool ownsData;
    
    Nodo(T* d = nullptr, bool owns = true) : dato(d), izq(nullptr), der(nullptr), padre(nullptr), ownsData(owns) {}
    
    virtual ~Nodo() {
        if (ownsData && dato) {
            delete dato;
        }
    }
    
    virtual string toString() {
        if (dato) {
            return dato->toString();
        }
        return "null";
    }
    
    virtual void postConstructor() {}
    // clona el nodo, las clases derivadas deben sobreescribir
    virtual Nodo<T>* cloneNode() {
        T* newDato = nullptr;
        if (dato) {
            if (ownsData) newDato = new T(*dato);
            else newDato = dato;
        }
        Nodo<T>* n = new Nodo<T>(newDato, ownsData);
        return n;
    }
};


// nodo para visualizacion svg qu extiende nodo con coordenadas x,y
template<typename T>
class NodoSVG : public Nodo<T> {
public:
    int x, y;
    
    NodoSVG(T* dato = nullptr, bool owns = true) : Nodo<T>(dato, owns) {
        postConstructor();
    }
    
    void postConstructor() override {
        x = 0;
        y = 0;
    }
};


// arbol binario de busqueda basico
template<typename T>
class ArbolBST {
protected:
    Nodo<T>* raiz;
    
    void liberarArbol(Nodo<T>* nodo) {
        // libera memoria de un subarbol de manera recursiva
        if (!nodo) return;
        liberarArbol(nodo->izq);
        liberarArbol(nodo->der);
        delete nodo;
    }

    // clona recursivamente un subarbol usando cloneNode polimorfico
    Nodo<T>* clonarSubarbol(Nodo<T>* nodo) {
        if (!nodo) return nullptr;
        Nodo<T>* nuevo = nodo->cloneNode();

        nuevo->izq = clonarSubarbol(nodo->izq);
        if (nuevo->izq) nuevo->izq->padre = nuevo;

        nuevo->der = clonarSubarbol(nodo->der);
        if (nuevo->der) nuevo->der->padre = nuevo;

        nuevo->padre = nullptr;
        return nuevo;
    }
    
    void inorder(Nodo<T>* n, stringstream& ss) {
        // recorrido inorder recursivo
        if (!n) return;
        inorder(n->izq, ss);
        ss << n->toString() << " ";
        inorder(n->der, ss);
    }
    
    void preorder(Nodo<T>* n, stringstream& ss) {
        // recorrido preorder recursivo
        if (!n) return;
        ss << n->toString() << " ";
        preorder(n->izq, ss);
        preorder(n->der, ss);
    }
    
    void postorder(Nodo<T>* n, stringstream& ss) {
        // recorrido postorder recursivo
        if (!n) return;
        postorder(n->izq, ss);
        postorder(n->der, ss);
        ss << n->toString() << " ";
    }
    
    void porNiveles(stringstream& ss) {
        // recorrido por niveles (bfs) usando un arreglo como cola
        if (!raiz) return;

        Nodo<T>* cola[1000];
        int frente = 0, fin = 0;
        cola[fin++] = raiz;

        while (frente < fin) {
            Nodo<T>* actual = cola[frente++];
            ss << actual->toString() << " ";

            if (actual->izq) cola[fin++] = actual->izq;
            if (actual->der) cola[fin++] = actual->der;
        }
    }
    
    Nodo<T>* encontrarMinimo(Nodo<T>* nodo) {
        // encuentra el nodo minimo (mas a la izquierda)
        while (nodo && nodo->izq) {
            nodo = nodo->izq;
        }
        return nodo;
    }

    Nodo<T>* deleteRec(Nodo<T>* nodo, T* dato) {
        // elimina el dato indicado del subarbol (recursivo)
        if (!nodo) return nullptr;

        if (*dato < *(nodo->dato)) {
            nodo->izq = deleteRec(nodo->izq, dato);
            if (nodo->izq) nodo->izq->padre = nodo;
        } else if (*dato > *(nodo->dato)) {
            nodo->der = deleteRec(nodo->der, dato);
            if (nodo->der) nodo->der->padre = nodo;
        } else {
            // caso 1: sin hijos o un hijo
            if (!nodo->izq) {
                Nodo<T>* temp = nodo->der;
                delete nodo;
                return temp;
            } else if (!nodo->der) {
                Nodo<T>* temp = nodo->izq;
                delete nodo;
                return temp;
            }

            // caso 2: dos hijos -> usar sucesor minimo en subarbol derecho
            Nodo<T>* temp = encontrarMinimo(nodo->der);

            // copiar el dato del sucesor (ajustar ownership)
            if (nodo->ownsData && nodo->dato) delete nodo->dato;
            nodo->dato = temp->dato;
            nodo->ownsData = temp->ownsData;
            temp->ownsData = false;

            // eliminar el sucesor
            nodo->der = deleteRec(nodo->der, temp->dato);
            if (nodo->der) nodo->der->padre = nodo;
        }
        return nodo;
    }

public:
    ArbolBST() : raiz(nullptr) {}
    // constructor de copia realiza clonacion profunda
    ArbolBST(const ArbolBST& other) : raiz(nullptr) {
        // copia profunda del arbol
        if (other.raiz) raiz = clonarSubarbol(other.raiz);
    }

    ArbolBST& operator=(const ArbolBST& other) {
        // asignacion con clonacion profunda
        if (this == &other) return *this;
        liberarArbol(raiz);
        raiz = nullptr;
        if (other.raiz) raiz = clonarSubarbol(other.raiz);
        return *this;
    }

    virtual ~ArbolBST() {
        liberarArbol(raiz);
    }
    
    virtual void insert(T* dato, bool owns = true) {
        // inserta un nuevo nodo; owns indica si el arbol es responsable del dato
        Nodo<T>* nuevo = new Nodo<T>(dato, owns);
        
        if (!raiz) {
            raiz = nuevo;
            return;
        }
        
        Nodo<T>* it = raiz;
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
    }
    
    Nodo<T>* search(const T& v) {
        // busca iterativa por comparacion
        Nodo<T>* it = raiz;
        while (it) {
            if (*(it->dato) == v) return it;
            if (v < *(it->dato)) it = it->izq;
            else it = it->der;
        }
        return nullptr;
    }
    
    bool deleteNode(T* dato) {
        // elimina el nodo que contiene el dato (ajusta raiz)
        raiz = deleteRec(raiz, dato);
        return true;
    }
    
    string toString() {
        // devuelve representacion con varios recorridos
        stringstream ss;

        ss << "Preorder: ";
        preorder(raiz, ss);
        ss << "\n";

        ss << "Inorder: ";
        inorder(raiz, ss);
        ss << "\n";

        ss << "Postorder: ";
        postorder(raiz, ss);
        ss << "\n";

        ss << "Por niveles: ";
        porNiveles(ss);
        ss << "\n";

        return ss.str();
    }
    
    Nodo<T>* getRaiz() { return raiz; }
};

template<typename T>
class ArbolSVG : public ArbolBST<T> {
public:
    ArbolSVG() : ArbolBST<T>() {}
    
    // asigna coordenadas x,y a cada nodo para la visualizacion svg
    void asignarCoordenadas(NodoSVG<T>* node, int depth, int& xRef, int horizontalSpacing, int verticalSpacing) {
        if (!node) return;
        
        if (node->izq) {
            asignarCoordenadas(static_cast<NodoSVG<T>*>(node->izq), depth + 1, xRef, horizontalSpacing, verticalSpacing);
        }
        
        node->x = xRef * horizontalSpacing;
        node->y = depth * verticalSpacing;
        xRef += 1;
        
        if (node->der) {
            asignarCoordenadas(static_cast<NodoSVG<T>*>(node->der), depth + 1, xRef, horizontalSpacing, verticalSpacing);
        }
    }
    
    virtual string getSVGContent() {
        NodoSVG<T>* root = static_cast<NodoSVG<T>*>(this->raiz);
        if (!root) return "";
        
        int espacioHorizontal = 50;
        int espacioVertical = 80;
        int contadorX = 1;
        
        asignarCoordenadas(root, 0, contadorX, espacioHorizontal, espacioVertical);
        
        // construye el contenido svg completo del arbol
        stringstream svgContent;
        svgContent << "<svg viewBox=\"0 0 2000 2000\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

        dibujarSVG(root, svgContent);
        svgContent << "</svg>";
        return svgContent.str();
    }
    
    int toSVG(const string& outfilename) {
        // inserta el svg en la plantilla html y escribe el archivo outfilename
        string filename = "svgTreeViewerTemplate.html";
        if (outfilename == filename) {
            cout << "No se puede sobrescribir el archivo " << filename << endl;
            return -1;
        }
        
        struct stat fileStat;
        if (stat(filename.c_str(), &fileStat) == 0) {}
        
        ifstream fileIn(filename.c_str(), ios::in | ios::binary);
        if (!fileIn.is_open()) {
            cout << "No se pudo abrir el archivo " << filename << endl;
            return -1;
        }
        
        stringstream svgHTMLJS;
        svgHTMLJS << fileIn.rdbuf();
        fileIn.close();
        
        string svgHTMLJS_str = svgHTMLJS.str();
        string svg = getSVGContent();
        
        string replacePattern = "%s";
        size_t replacePatternIndex = svgHTMLJS_str.find(replacePattern);
        if (replacePatternIndex == string::npos) {
            cout << "No se pudo encontrar el patron de reemplazo." << endl;
            return -1;
        }
        
        ofstream fileOut(outfilename.c_str(), ios::out | ios::binary);
        if (!fileOut.is_open()) {
            cout << "No se pudo abrir el archivo " << outfilename << endl;
            return -1;
        }
        
        fileOut << svgHTMLJS_str.substr(0, replacePatternIndex);
        fileOut << svg;
        fileOut << svgHTMLJS_str.substr(replacePatternIndex + replacePattern.length());
        fileOut.close();
        
        cout << "Se genero el visualizador '" << outfilename << "'." << endl;
        
        // Abrir en el navegador
        stringstream openBrowser;
        #ifdef _WIN32
            openBrowser << "start " << outfilename;
        #elif __APPLE__
            openBrowser << "open " << outfilename;
        #elif __linux__
            openBrowser << "xdg-open " << outfilename;
        #endif
        
        system(openBrowser.str().c_str());
        return 0;
    }

private:
    // dibuja lineas, circulos y texto de cada nodo recursivamente
    void dibujarSVG(NodoSVG<T>* node, stringstream& svgContent) {
        if (!node) return;

        if (node->izq) {
            NodoSVG<T>* izq = static_cast<NodoSVG<T>*>(node->izq);
            svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y
                       << "\" x2=\"" << izq->x << "\" y2=\"" << izq->y << "\" stroke=\"black\"/>";
        }
        if (node->der) {
            NodoSVG<T>* der = static_cast<NodoSVG<T>*>(node->der);
            svgContent << "<line x1=\"" << node->x << "\" y1=\"" << node->y
                       << "\" x2=\"" << der->x << "\" y2=\"" << der->y << "\" stroke=\"black\"/>";
        }

        svgContent << "<circle cx=\"" << node->x << "\" cy=\"" << node->y
                   << "\" r=\"50\" fill=\"lightblue\" stroke=\"black\"/>";
        svgContent << "<text x=\"" << node->x << "\" y=\"" << (node->y + 5)
                   << "\" text-anchor=\"middle\" font-size=\"12\" font-weight=\"bold\" fill=\"black\">"
                   << node->toString() << "</text>";

        dibujarSVG(static_cast<NodoSVG<T>*>(node->izq), svgContent);
        dibujarSVG(static_cast<NodoSVG<T>*>(node->der), svgContent);
    }
};

#endif