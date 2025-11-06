#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

#define T(t) #t

class Nodo{
  public:
  void* dato;
  Nodo *izq, *der;
  string T;
  Nodo(void* dato){
    this->dato = dato;
    izq = NULL;
    der = NULL;
  }
  
  string toString(){
    stringstream ss;
	if(T == "int"){
	  int* dato = (int*)this->dato;
      ss << *dato;
	}
	else if(T == "string"){
	  string* dato = (string*)this->dato;
      ss << *dato;
	}
	else{
      ss << "{" << this->dato << "}";
	}
	return ss.str();
  }
  
  friend ostream& operator<<(ostream& os, Nodo*& node){
    os << node->toString();
    return os;
  }
  
  virtual void postConstructor(){
    /* Otras clases heredan de esta clase, pero sus constructores no serán invocados con el insert base.
	Si esos constructores no se invocan, el programa queda con 'undefined behavior' y provoca segmentation faults.
	Cada clase derivada debe por obligación implementar este método; sin embargo, no podemos declararlo como virtual puro,
	ya que eso provocaría que la clase Nodo se convierta en clase abstracta y no se podría instanciar. */
  }
};

class ArbolBST{
  public:
  Nodo* raiz;
  string T;
  
  ArbolBST(string T){
    raiz = NULL;
    this->T = T;
  }
  
  void insert(void* dato){
    Nodo* nuevoNodo = new Nodo(dato);
    nuevoNodo->T = T;
    nuevoNodo->postConstructor(); // IMPORTANTE para que las clases derivadas se inicialicen !!
	
    if(raiz == NULL){
      raiz = nuevoNodo;
    }else{
      Nodo *it = raiz, *p = NULL;
      char donde = 'D';
	  
      // buscar el campo
      while(it != NULL){
        p = it;
        if( T == T(string) ){
          string* d1 = (string*)dato;
          string* d2 = (string*)it->dato;
          if( d1->compare(*d2) < 0 ){
            it = it->izq; donde = 'I';
          }
          else{
            it = it->der; donde = 'D';
          }
        }
        else if( T == T(int) || T == T(float) || T == T(double) ){
		
          int* d1 = (int*)dato;
          int* d2 = (int*)it->dato;
		  
          if( *d1 < *d2 ){
            it = it->izq; donde = 'I';
          }
          else{
            it = it->der; donde = 'D';
          }
        }
        else{
          if( dato < it->dato ){ // OJO...
            it = it->izq; donde = 'I';
          }
          else{
            it = it->der; donde = 'D';
          }
        }
		
      }// while
	  
      // realizar la inserción
      if(donde == 'I') p->izq = nuevoNodo;
      else p->der = nuevoNodo;
	
    }//else
  
  }
  
  void encolar(Nodo*& cola, Nodo*& dato){
    Nodo* nuevoNodo = new Nodo(dato);
    if(cola == NULL) cola = nuevoNodo;
    else{
      Nodo* it = cola;
      while(it->der != NULL) it = it->der;
      it->der = nuevoNodo;
    }
  }
  
  void porNiveles(stringstream& ss){
    Nodo* cola = NULL;
    encolar(cola, raiz);
    while(cola != NULL){
      Nodo* sacarDeCola = cola;
      cola = cola->der;
	  
      Nodo* nodoArbol = (Nodo*)sacarDeCola->dato;
      delete sacarDeCola;
	  
      ss << nodoArbol->toString() << ", ";
	  
      if(nodoArbol->izq != NULL) encolar(cola, nodoArbol->izq);
      if(nodoArbol->der != NULL) encolar(cola, nodoArbol->der);
	   
    }
  }
  
  void inorder(Nodo* n, stringstream& ss){
    if(n->izq != NULL) inorder(n->izq,ss);
    if(n->der != NULL) inorder(n->der,ss);
    ss << n->toString() << ", ";
  }
  
  string toString(){
    stringstream ss;
    //inorder(raiz, ss);
    porNiveles(ss);
    return ss.str();
  }
};

class NodoSVG : public Nodo {
  public:
  int x, y;
  NodoSVG(void* dato) : Nodo(dato){
    postConstructor();
  }
  
  void postConstructor(){ 
    this->x = 0; // coordenada x
    this->y = 0; // coordenada y
  }
};

class ArbolSVG : public ArbolBST {
  public:
  
  ArbolSVG(string T) : ArbolBST(T){ }
  
  // Función para asignar coordenadas a cada nodo
  void asignarCoordenadas(NodoSVG* node, int depth, int& xRef, int horizontalSpacing, int verticalSpacing){
    if(node == NULL) return;

    // Recorremos en orden para asignar coordenadas horizontales
	
	if(node->izq != NULL){
      asignarCoordenadas((NodoSVG*)node->izq, depth + 1, xRef, horizontalSpacing, verticalSpacing);
	}

    // La coordenada x se asigna en función del contador xRef
    node->x = xRef * horizontalSpacing;
    node->y = depth * verticalSpacing;
    xRef += 1;

	if(node->der != NULL){
      asignarCoordenadas((NodoSVG*)node->der, depth + 1, xRef, horizontalSpacing, verticalSpacing);
	}
  }

  // Función para obtener el tamaño del árbol
  int getTreeSize(NodoSVG* node){
    if(node == NULL) return 0;
    return 1 + max(getTreeSize((NodoSVG*)node->izq), getTreeSize((NodoSVG*)node->der));
  }

  // Función para generar el SVG
  string toSVG(){
    NodoSVG* root = (NodoSVG*)raiz;
    int svgWidth = 800;
    int svgHeight = 600;
    int horizontalSpacing = 50; // espacio entre nodos en x
    int verticalSpacing = 80;   // espacio entre niveles en y

    // Asignar coordenadas a los nodos
    int xRef = 1; // referencia para el x
    asignarCoordenadas(root, 0, xRef, horizontalSpacing, verticalSpacing);

    // Crear el SVG
    stringstream svgContent;
	svgContent /*=*/<< "<svg viewBox=\"0 0 200 200\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

    // Función lambda para dibujar nodos y conexiones
	/*[&]: capturar todas las variables externas por referencia. */
	
	auto dibujarNodo = [&](const auto &self, NodoSVG* node, stringstream& svgContent) -> void {
      if(node == NULL) return;
      
      // Dibujar línea hacia el hijo izquierdo
      if(node->izq != NULL){
        svgContent /*+=*/<< "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->izq)->x<<"\" y2=\""<<((NodoSVG*)node->izq)->y<<"\" stroke=\"black\"/>";
      }
      
      // Dibujar línea hacia el hijo derecho
      if(node->der != NULL){
        svgContent /*+=*/<< "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->der)->x<<"\" y2=\""<<((NodoSVG*)node->der)->y<<"\" stroke=\"black\"/>";
      }
      
      // Dibujar el nodo como un círculo
      svgContent /*+=*/<< "<circle cx=\""<<node->x<<"\" cy=\""<<node->y<<"\" r=\"20\" fill=\"lightblue\" stroke=\"black\"/>";
      
      // Añadir el valor del nodo como texto
      svgContent /*+=*/<< "<text x=\""<<node->x<<"\" y=\""<<(node->y + 5)<<"\" text-anchor=\"middle\" font-size=\"12\" fill=\"black\">"<<node->toString()<<"</text>";
      //svgContent /*+=*/<< "<image x=\""<<node->x<<"\" y=\""<<(node->y + 5)<<"\" xlink:href=\"imagen_0.png\" height=\"200\" width=\"200\"/>";
      
      // Recursivamente dibujar los hijos
      self(self, (NodoSVG*)node->izq, svgContent);//dibujarNodo(node->izq, svgContent);
      self(self, (NodoSVG*)node->der, svgContent);//dibujarNodo(node->der, svgContent);
    };

    dibujarNodo(dibujarNodo, root, svgContent);

    // Cerrar el SVG
    svgContent /*+=*/<< "</svg>";

    return svgContent.str();
  }
  
  int toSVG(string outfilename){
  
    string filename = "svgTreeViewerTemplate.html";
    if(outfilename.compare(filename) == 0){
      cout << "No se puede sobrescribir el archivo" << filename << endl;
	  return -1;
    }
	
    size_t fileLen = 0;
    struct stat fileStat;
    if( ( stat (filename.c_str(), &fileStat) == 0) ){
      fileLen = fileStat.st_size;
    }
    
    ifstream fileIn;
    fileIn.open(filename.c_str(), ios::in | ios::binary );
    if( !fileIn.is_open() ){
      cout << "No se pudo abrir el archivo" << filename << endl;
	  return -1;
    }
    
    stringstream svgHTMLJS;
    svgHTMLJS.str(string());
    svgHTMLJS << fileIn.rdbuf();
    fileIn.close();
	
	
	/* generación del archivo de salida */
  
    string svgHTMLJS_str = svgHTMLJS.str();
    string svg = toSVG();
	
	string replacePattern = "%s";
	int replacePatternIndex = svgHTMLJS_str.find(replacePattern);
    if( replacePatternIndex == -1 ){
      cout << "No se pudo encontrar el patrón de reemplazo." << endl;
	  return -1;
    }
    
    ofstream fileOut;
    fileOut.open(outfilename.c_str(), ios::out | ios::binary );
    if( !fileOut.is_open() ){
      cout << "No se pudo abrir el archivo" << outfilename << endl;
	  return -1;
    }
    fileOut << svgHTMLJS_str.substr(0, replacePatternIndex);
    fileOut << svg;
    fileOut << svgHTMLJS_str.substr( replacePatternIndex + replacePattern.length() );
    fileOut.close();
	
    cout << "Se generó el visualizador '"<< outfilename <<"'.\nPor favor, proceda a abrilo desde un navegador web." << endl;
    
    stringstream openBrowser;
    #if _WIN32
    openBrowser << "start " <<  outfilename;
    #elif __APPLE__
    openBrowser << "open " <<  outfilename;
    #elif __linux__
    openBrowser << "xdg-open " <<  outfilename;
    #endif
	string openBrowserStr = openBrowser.str();
    system(openBrowserStr.c_str());
	
	return 0;
  }
};
