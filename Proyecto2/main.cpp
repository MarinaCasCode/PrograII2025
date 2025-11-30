#include <iostream>
#include "rbTree.cpp"

using namespace std;

int main() {
    cout << "prueba del arbol rojiegro con fixInsert completo" << endl << endl;

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

    return 0;
}