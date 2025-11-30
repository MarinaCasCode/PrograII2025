#include <iostream>
#include <sstream>
#include <fstream>
#include <cstdlib>
#include <sys/stat.h>

using namespace std;

#define T(t) #t

// Declaracion forward para ParClaveValor
template<typename T>
class ParClaveValor {
public:
    string clave;
    T valor;

    ParClaveValor() : clave(""), valor() { }
    ParClaveValor(const string& k, const T& v) : clave(k), valor(v) { }
    string toString() const { return clave; }
};

class Nodo{
  public:
  void* dato;
  Nodo *izq, *der, *padre;
  string T;
  Nodo(void* dato){
    this->dato = dato;
    izq = NULL;
    der = NULL;
    padre = NULL;
  }

  virtual ~Nodo(){}

  virtual string toString(){
    stringstream ss;
    if(T == "int"){
      int* dato = (int*)this->dato;
      ss << *dato;
    }
    else if(T == "string"){
      string* dato = (string*)this->dato;
      ss << *dato;
    }
    else if(T == "ParClaveValor"){
      // Usar un cast generico y acceder al metodo toString
      // que todas las instancias de ParClaveValor tienen
      void* ptr = this->dato;
      // Acceder a la clave que es el primer campo de ParClaveValor
      string* clave = (string*)ptr;
      if(clave != NULL && !clave->empty()) {
        ss << *clave;
      } else {
        ss << "{null}";
      }
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
    // metodo para inicializacion de clases derivadas
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
    nuevoNodo->postConstructor();
    
    if(raiz == NULL){
      raiz = nuevoNodo;
    }else{
      Nodo *it = raiz, *p = NULL;
      char donde = 'D';
      
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
        else if( T == "ParClaveValor" ){
          ParClaveValor<int>* d1 = (ParClaveValor<int>*)dato;
          ParClaveValor<int>* d2 = (ParClaveValor<int>*)it->dato;
          if( d1->clave < d2->clave ){
            it = it->izq; donde = 'I';
          }
          else{
            it = it->der; donde = 'D';
          }
        }
        else{
          if( dato < it->dato ){
            it = it->izq; donde = 'I';
          }
          else{
            it = it->der; donde = 'D';
          }
        }
      }
      
      if(donde == 'I') p->izq = nuevoNodo;
      else p->der = nuevoNodo;
      nuevoNodo->padre = p;
    }
  }
  
  Nodo* search(void* dato) {
    Nodo* it = raiz;
    while(it != NULL) {
      int cmp = 0;
      if(T == "string") {
        string* d1 = (string*)dato;
        string* d2 = (string*)it->dato;
        cmp = d1->compare(*d2);
      }
      else if(T == "int" || T == "float" || T == "double") {
        int* d1 = (int*)dato;
        int* d2 = (int*)it->dato;
        if(*d1 < *d2) cmp = -1;
        else if(*d1 > *d2) cmp = 1;
        else cmp = 0;
      }
      else if(T == "ParClaveValor") {
        ParClaveValor<int>* d1 = (ParClaveValor<int>*)dato;
        ParClaveValor<int>* d2 = (ParClaveValor<int>*)it->dato;
        cmp = d1->clave.compare(d2->clave);
      }
      else {
        if(dato < it->dato) cmp = -1;
        else if(dato > it->dato) cmp = 1;
        else cmp = 0;
      }
      
      if(cmp == 0) return it;
      else if(cmp < 0) it = it->izq;
      else it = it->der;
    }
    return NULL;
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
    ss << n->toString() << ", ";
    if(n->der != NULL) inorder(n->der,ss);
  }
  
  void preorder(Nodo* n, stringstream& ss){
    ss << n->toString() << ", ";
    if(n->izq != NULL) preorder(n->izq,ss);
    if(n->der != NULL) preorder(n->der,ss);
  }
  
  void postorder(Nodo* n, stringstream& ss){
    if(n->izq != NULL) postorder(n->izq,ss);
    if(n->der != NULL) postorder(n->der,ss);
    ss << n->toString() << ", ";
  }
  
  Nodo* encontrarMinimo(Nodo* nodo){
    while(nodo->izq != NULL){
      nodo = nodo->izq;
    }
    return nodo;
  }
  
  Nodo* deleteRec(Nodo* nodo, void* dato){
    if(nodo == NULL) return NULL;
    
    // Comparar para encontrar el nodo a eliminar
    int cmp = 0;
    if(T == T(string)){
      string* d1 = (string*)dato;
      string* d2 = (string*)nodo->dato;
      cmp = d1->compare(*d2);
    }
    else if(T == T(int) || T == T(float) || T == T(double)){
      int* d1 = (int*)dato;
      int* d2 = (int*)nodo->dato;
      if(*d1 == *d2) cmp = 0;
      else if(*d1 < *d2) cmp = -1;
      else cmp = 1;
    }
    else if(T == "ParClaveValor"){
      string* clave1 = (string*)dato;
      string* clave2 = (string*)nodo->dato;
      cmp = clave1->compare(*clave2);
    }
    else{
      if(dato == nodo->dato) cmp = 0;
      else if(dato < nodo->dato) cmp = -1;
      else cmp = 1;
    }
    
    // Buscar el nodo a eliminar
    if(cmp < 0){
      nodo->izq = deleteRec(nodo->izq, dato);
      if(nodo->izq != NULL) nodo->izq->padre = nodo;
    }
    else if(cmp > 0){
      nodo->der = deleteRec(nodo->der, dato);
      if(nodo->der != NULL) nodo->der->padre = nodo;
    }
    else{
      // Nodo encontrado - eliminar
      
      // Caso 1: Nodo hoja o con un solo hijo
      if(nodo->izq == NULL){
        Nodo* temp = nodo->der;
        delete nodo;
        return temp;
      }
      else if(nodo->der == NULL){
        Nodo* temp = nodo->izq;
        delete nodo;
        return temp;
      }
      
      // Caso 2: Nodo con dos hijos
      // Encontrar el sucesor inorder (minimo del subarbol derecho)
      Nodo* sucesor = encontrarMinimo(nodo->der);
      
      // Copiar el dato del sucesor al nodo actual
      nodo->dato = sucesor->dato;
      
      // Eliminar el sucesor
      nodo->der = deleteRec(nodo->der, sucesor->dato);
      if(nodo->der != NULL) nodo->der->padre = nodo;
    }
    
    return nodo;
  }
  
  void deleteNode(void* dato){
    raiz = deleteRec(raiz, dato);
    if(raiz != NULL) raiz->padre = NULL;
  }
  
  string toString(){
    if(raiz == NULL) return "";
    
    stringstream ss;
    
    ss << "Preorder: ";
    preorder(raiz, ss);
    ss << "\n";
    
    ss << "Inorder: ";
    inorder(raiz, ss);
    ss << "\n";
    
    ss << "Postorder: ";
    postorder(raiz, ss);
    ss << "\n";
    
    ss << "Por niveles: ";
    porNiveles(ss);
    ss << "\n";
    
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
    this->x = 0;
    this->y = 0;
  }
};

class ArbolSVG : public ArbolBST {
  public:
  
  ArbolSVG(string T) : ArbolBST(T){ }
  
  void asignarCoordenadas(NodoSVG* node, int depth, int& xRef, int horizontalSpacing, int verticalSpacing){
    if(node == NULL) return;

    if(node->izq != NULL){
      asignarCoordenadas((NodoSVG*)node->izq, depth + 1, xRef, horizontalSpacing, verticalSpacing);
    }

    node->x = xRef * horizontalSpacing;
    node->y = depth * verticalSpacing;
    xRef += 1;

    if(node->der != NULL){
      asignarCoordenadas((NodoSVG*)node->der, depth + 1, xRef, horizontalSpacing, verticalSpacing);
    }
  }

  int getTreeSize(NodoSVG* node){
    if(node == NULL) return 0;
    return 1 + max(getTreeSize((NodoSVG*)node->izq), getTreeSize((NodoSVG*)node->der));
  }

  string toSVG(){
    NodoSVG* root = (NodoSVG*)raiz;
      int anchoSVG = 800;
      int altoSVG  = 600;
      int espacioHorizontal = 50;
      int espacioVertical   = 80;

      int contadorX = 1;
      asignarCoordenadas(root, 0, contadorX, espacioHorizontal, espacioVertical);

      stringstream svgContent;
      svgContent << "<svg viewBox=\"0 0 " << anchoSVG << " " << altoSVG << "\" xmlns=\"http://www.w3.org/2000/svg\" style=\"width: 100%; height: auto;\">";

    auto dibujarNodo = [&](const auto &self, NodoSVG* node, stringstream& svgContent) -> void {
      if(node == NULL) return;
      
      if(node->izq != NULL){
        svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->izq)->x<<"\" y2=\""<<((NodoSVG*)node->izq)->y<<"\" stroke=\"black\"/>";
      }
      
      if(node->der != NULL){
        svgContent << "<line x1=\""<<node->x<<"\" y1=\""<<node->y<<"\" x2=\""<<((NodoSVG*)node->der)->x<<"\" y2=\""<<((NodoSVG*)node->der)->y<<"\" stroke=\"black\"/>";
      }
      
      svgContent << "<circle cx=\""<<node->x<<"\" cy=\""<<node->y<<"\" r=\"20\" fill=\"lightblue\" stroke=\"black\"/>";
      svgContent << "<text x=\""<<node->x<<"\" y=\""<<(node->y + 5)<<"\" text-anchor=\"middle\" font-size=\"12\" fill=\"black\">"<<node->toString()<<"</text>";
      
      self(self, (NodoSVG*)node->izq, svgContent);
      self(self, (NodoSVG*)node->der, svgContent);
    };

    dibujarNodo(dibujarNodo, root, svgContent);
    svgContent << "</svg>";

    return svgContent.str();
  }
  
  int toSVG(string outfilename){
    string filename = "svgTreeViewerTemplate.html";
    if(outfilename.compare(filename) == 0){
      cout << "No se puede sobrescribir el archivo" << filename << endl;
      return -1;
    }
    
    struct stat fileStat;
    if( ( stat (filename.c_str(), &fileStat) == 0) ){
      // plantilla disponible (fileStat) si se necesita
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
    
    string svgHTMLJS_str = svgHTMLJS.str();
    string svg = toSVG();
    
    string replacePattern = "%s";
    int replacePatternIndex = svgHTMLJS_str.find(replacePattern);
    if( replacePatternIndex == -1 ){
      cout << "No se pudo encontrar el patron de reemplazo." << endl;
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
    
    cout << "Se genero el visualizador '"<< outfilename <<"'." << endl;
    
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