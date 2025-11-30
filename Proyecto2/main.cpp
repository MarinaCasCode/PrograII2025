#include <iostream>
#include "rbTree.cpp"
#include "rbHash.cpp"

using namespace std;

int main() {
    cout << "prueba del arbol rojinegro" << endl << endl;

    ArbolRB arbol("int");
    
    int valores[] = {10, 20, 30, 15, 25, 5, 1, 40, 35};
    
    cout << "insertando valores: ";
    for(int i = 0; i < 9; i++) {
        cout << valores[i] << " ";
        arbol.insert(&valores[i]);
    }
    cout << endl << endl;

    cout << "recorrido por niveles del arbol RB: " << endl;
    cout << arbol.toString() << endl;

    cout << "generando visualizacion SVG" << endl;
    arbol.toSVG("arbolRB_test.html");

    cout << endl << "prbando el rbHash" << endl << endl;

    RBHash<int> rankingMundial;

    rankingMundial["Brasil"] = 5;
    rankingMundial["Italia"] = 4;
    rankingMundial["Alemania"] = 4;
    rankingMundial["Argentina"] = 3;
    rankingMundial["Francia"] = 2;
    rankingMundial["Uruguay"] = 2;

    cout << "Ranking Mundial de Copas del Mundo:" << endl;
    cout << "Brasil: " << rankingMundial["Brasil"] << " copas" << endl;
    cout << "Italia: " << rankingMundial["Italia"] << " copas" << endl;
    cout << "Alemania: " << rankingMundial["Alemania"] << " copas" << endl;
    cout << "Argentina: " << rankingMundial["Argentina"] << " copas" << endl;
    cout << "Francia: " << rankingMundial["Francia"] << " copas" << endl;
    cout << "Uruguay: " << rankingMundial["Uruguay"] << " copas" << endl;

    RBHash<string> datosPersona;
    datosPersona["nombre"] = "Maria";
    datosPersona["apellido"] = "Gonzalez";
    datosPersona["ciudad"] = "San Jose";
    
    cout << endl << "Datos personales:" << endl;
    cout << "Nombre: " << datosPersona["nombre"] << endl;
    cout << "Apellido: " << datosPersona["apellido"] << endl;
    cout << "Ciudad: " << datosPersona["ciudad"] << endl;

    cout << endl << "Generando visualizaciones SVG..." << endl;
    rankingMundial.generarSVG("mundiales_rbhash.html");
    datosPersona.generarSVG("persona_rbhash.html");

    cout << "Visualizaciones generadas exitosamente!" << endl;

    return 0;
}