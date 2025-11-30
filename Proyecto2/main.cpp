#include <iostream>
#include "rbTree.cpp"
#include "rbHash.cpp"

using namespace std;

int main() {
    cout << "prueba del arbol rojinegro" << endl << endl;

    ArbolRB arbol("int");
    
    // Insertar valores que provocaran rotaciones y recoloreos
    int valores[] = {10, 20, 30, 15, 25, 5, 1, 40, 35};
    
    cout << "insertando valores: ";
    for(int i = 0; i < 9; i++) {
        cout << valores[i] << " ";
        arbol.insert(&valores[i]);
    }
    cout << endl << endl;

    cout << "recorrido por niveles del arbol RB: " << endl;
    cout << arbol.toString() << endl;

    // generar visualizacion SVG
    cout << "generando visualizacion SVG" << endl;
    arbol.toSVG("arbolRB_test.html");

    cout << endl << "prueba del RBHash" << endl << endl;

    RBHash<int> rankingMundial;

    rankingMundial["Brasil"] = 5;
    rankingMundial["Italia"] = 4;
    rankingMundial["Alemania"] = 4;
    rankingMundial["Argentina"] = 3;

    cout << "Ranking Mundial de Copas del Mundo:" << endl;
    cout << "Brasil: " << rankingMundial["Brasil"] << endl;
    cout << "Italia: " << rankingMundial["Italia"] << endl;
    cout << "Alemania: " << rankingMundial["Alemania"] << endl;
    cout << "Argentina: " << rankingMundial["Argentina"] << endl;

    cout << endl << "Generando visualizacion SVG del hash" << endl;
    rankingMundial.getArbol().toSVG("Mundiales.html");

    return 0;
}