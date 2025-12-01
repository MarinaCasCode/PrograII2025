#include <iostream>
#include "RBHash.h"

using namespace std;

int main() {
    cout << "prueba d RBHash" << endl << endl;
    
    cout << "RBHash<int> rnking de copas mundiales" << endl;
    
    RBHash<int> rankingMundial;
    
    rankingMundial["Brasil"] = 5;
    rankingMundial["Italia"] = 4;
    rankingMundial["Alemania"] = 4;
    rankingMundial["Argentina"] = 3;
    rankingMundial["Francia"] = 2;
    rankingMundial["Uruguay"] = 2;
    rankingMundial["Inglaterra"] = 1;
    rankingMundial["España"] = 1;
    
    cout << "Brasil: " << rankingMundial["Brasil"] << " copas" << endl;
    cout << "Italia: " << rankingMundial["Italia"] << " copas" << endl;
    cout << "Alemania: " << rankingMundial["Alemania"] << " copas" << endl;
    cout << "Argentina: " << rankingMundial["Argentina"] << " copas" << endl;
    cout << "Francia: " << rankingMundial["Francia"] << " copas" << endl;
    cout << "Uruguay: " << rankingMundial["Uruguay"] << " copas" << endl;
    
    cout << endl << "Generando visualizacion SVG..." << endl;
    rankingMundial.toSVG("Mundiales.html");
    cout << "Archivo 'Mundiales.html' generado exitosamente." << endl;
    
    cout << endl << "RBHash<string> datos personales" << endl;
    
    RBHash<string> datosPersona;
    
    datosPersona["nombre"] = "Maria";
    datosPersona["apellido"] = "Gonzalez";
    datosPersona["ciudad"] = "San Jose";
    datosPersona["pais"] = "Costa Rica";
    datosPersona["edad"] = "28";
    
    cout << "Nombre: " << datosPersona["nombre"] << endl;
    cout << "Apellido: " << datosPersona["apellido"] << endl;
    cout << "Ciudad: " << datosPersona["ciudad"] << endl;
    cout << "Pais: " << datosPersona["pais"] << endl;
    
    cout << endl << "Generando visualizacion SVG..." << endl;
    datosPersona.toSVG("Persona.html");
    cout << "Archivo 'Persona.html' generado exitosamente." << endl;
    
    cout << endl << "prueba de copia y ssignacion" << endl;
    
    RBHash<int> copiaRanking = rankingMundial;
    cout << "Copia Brasil: " << copiaRanking["Brasil"] << " copas" << endl;
    cout << "Copia Argentina: " << copiaRanking["Argentina"] << " copas" << endl;
    
    // Modificar la copia
    copiaRanking["Brasil"] = 6;
    cout << "Copia modificada Brasil: " << copiaRanking["Brasil"] << " copas" << endl;
    cout << "Original Brasil: " << rankingMundial["Brasil"] << " copas" << endl;
    
    
    return 0;
}