#include <iostream>
#include "rbTree.cpp"

using namespace std;

int main() {
    cout << "prueba inicial del arbol rojo-negro" << endl;

    ArbolRB arbol("int");
    
    int valores[] = {10, 20, 5, 15, 30};
    for(int i = 0; i < 5; i++) {
        arbol.insert(&valores[i]);
    }

    cout << "recorrido por niveles: " << arbol.toString() << endl;

    return 0;
}