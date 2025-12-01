#ifndef NODORB_H
#define NODORB_H

#include "bstTree.cpp"  // incluye NodoSVG

enum Color { ROJO, NEGRO };

// nodo rojo-negro que hereda de NodoSVG para obtener coordenadas x,y
class NodoRB : public NodoSVG {
public:
    Color color;
    NodoRB* padre;
    
    NodoRB(void* dato) : NodoSVG(dato) {
        color = ROJO;
        padre = nullptr;
        T = "NodoRB";
    }
    
    void postConstructor() override {
        this->x = 0;
        this->y = 0;
    }
    
    std::string toString() override {
        std::stringstream ss;
        std::string colorStr = (color == ROJO) ? "ROJO" : "NEGRO";
        // formatear segun el tipo de dato
        if(T == "int"){
            int* dato = (int*)this->dato;
            ss << *dato << "(" << colorStr << ")";
        }
        else if(T == "string"){
            std::string* dato = (std::string*)this->dato;
            ss << *dato << "(" << colorStr << ")";
        }
        else if(T == "pair"){
            // para ParClaveValor, mostrar solo clave
            ss << "{pair}(" << colorStr << ")";
        }
        else{
            ss << "{" << this->dato << "}(" << colorStr << ")";
        }
        return ss.str();
    }
};

#endif