#include <iostream>
#include "rbTree.cpp"

using namespace std;

int main() {
    cout << "prueba del arbol rojo-negro con rotaciones" << endl << endl;

    ArbolRB arbol("int");
    
    // Insertar valores que provocarán rotaciones
    int valores[] = {10, 20, 30, 15, 25, 5, 1};
    
    for(int i = 0; i < 7; i++) {
        cout << valores[i] << " ";
        arbol.insert(&valores[i]);
    }
    cout << endl << endl;

    cout << "recorridos del arbol:" << endl;
    cout << arbol.toString() << endl;

    return 0;
}