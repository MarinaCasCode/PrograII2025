#ifndef RBHASH_H
#define RBHASH_H

#include "rbTree.cpp"
#include <sstream>

using namespace std;

template <typename T>
class RBHash {
private:
    ArbolRB arbol;
    
    // busca un nodo por clave usando ParClaveValor temporal
    NodoRB* buscarNodo(string clave) {
        // crear par temporal dinamicamente para buscar
        ParClaveValor<T>* temp = new ParClaveValor<T>(clave, T());
        
        // buscar en el arbol
        Nodo* nodo = arbol.search((void*)temp);
        
        // liberar el temporal
        delete temp;
        
        return (NodoRB*)nodo;
    }

public:
    // inicializar hash con arbol RB de tipo "pair"
    RBHash() : arbol("pair") {
    }

    // operador [] para acceso y asignacion como diccionario
    // retorna referencia al valor, crea la clave si no existe
    T& operator[](string clave) {
        NodoRB* nodo = buscarNodo(clave);
        
        // si la clave no existe, crearla con valor por defecto
        if (nodo == NULL) {
            T defaultValue = T();
            ParClaveValor<T>* nuevoPar = new ParClaveValor<T>(clave, defaultValue);
            arbol.insert((void*)nuevoPar);
            nodo = buscarNodo(clave);
        }
        
        // obtener el par del nodo y retornar su valor
        ParClaveValor<T>* par = (ParClaveValor<T>*)nodo->dato;
        return par->valor;
    }

    // generar archivo HTML con visualizacion SVG del arbol
    int toSVG(string outfilename) {
        return arbol.toSVG(outfilename);
    }

    // obtener representacion textual del arbol
    string toString() {
        return arbol.toString();
    }

    // imprimir contenido en consola
    void imprimir() {
        cout << toString() << endl;
    }
};

#endif
 