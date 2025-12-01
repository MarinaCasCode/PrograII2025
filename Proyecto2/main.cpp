#include <iostream>
#include "rbTree.cpp"
#include "RBHash.h"

using namespace std;

int main() {
    // test 1: RBHash con datos enteros
    cout << "RBHash<int> ranking de copas mundiales" << endl;
    RBHash<int> rankingMundial;

    // insertar pares clave-valor
    rankingMundial["Brasil"] = 5;
    rankingMundial["Italia"] = 4;
    rankingMundial["Alemania"] = 4;
    rankingMundial["Argentina"] = 3;

    // acceder a valores mediante operador []
    cout << "Brasil: " << rankingMundial["Brasil"] << endl;
    cout << "Argentina: " << rankingMundial["Argentina"] << endl;

    // generar visualizacion SVG del arbol rojo-negro
    rankingMundial.toSVG("Mundiales.html");
    cout << "se genero mundiales.html" << endl << endl;

    // test 2: RBHash con datos string
    cout << "RBHash<string> de dato personales" << endl;
    RBHash<string> datosPersona;

    // insertar pares clave-valor string
    datosPersona["nombre"] = "Maria";
    datosPersona["apellido"] = "Gonzalez";
    datosPersona["ciudad"] = "San Jose";

    // acceder a valores
    cout << "Nombre: " << datosPersona["nombre"] << endl;
    cout << "Ciudad: " << datosPersona["ciudad"] << endl;

    // generar SVG
    datosPersona.toSVG("Persona.html");
    cout << "se genero Persona.html" << endl;

    return 0;
}
