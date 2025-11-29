# Proyecto 2 – Árbol Rojo-Negro, SVG y Tabla Hash  
**Pseudocódigo**

---

## Archivo: `bstTree.cpp`
Árbol binario de búsqueda **base**, dado en el proyecto.  

```pseudocode
TIPO Nodo:
    campo dato : puntero_generico
    campo izq  : Nodo
    campo der  : Nodo
    campo T    : string  // tipo: int, string, float...

    METODO toString() -> string:
        SI T == "int":
            return string( *(int*)dato )
        SI T == "string":
            return *(string*)dato
        return "{" + direccion_puntero(dato) + "}"

TIPO ArbolBST:
    campo raiz : Nodo
    campo T    : string

    CONSTRUCTOR(T_str):
        raiz = NULL
        T = T_str

    METODO insert(dato_ptr):
        nuevo = Nodo(dato_ptr)
        nuevo.T = T

        SI raiz == NULL:
            raiz = nuevo
            RETORNAR

        it = raiz
        p = NULL

        MIENTRAS it != NULL:
            p = it
            SI comparar(dato_ptr, it.dato) < 0:
                it = it.izq
                lado = "I"
            SINO:
                it = it.der
                lado = "D"

        SI lado == "I": p.izq = nuevo
        SINO: p.der = nuevo

    // Otros métodos que forman parte de la interfaz:
    METODO search(dato_ptr) -> Nodo
    METODO delete(dato_ptr)
    METODO inorder()   -> string
    METODO preorder()  -> string
    METODO postorder() -> string
    METODO porNiveles() -> string
    METODO toString() -> string  // Une todos los recorridos
```

---

## Archivo: `svgTreeViewer.cpp`  

```pseudocode
TIPO NodoSVG HEREDA Nodo:
    campo x, y : entero

    METODO postConstructor():
        x = 0
        y = 0

TIPO ArbolSVG HEREDA ArbolBST:

    METODO asignarCoordenadas(nodo, profundidad, ref xRef, hSpace, vSpace):
        SI nodo == NULL: RETORNAR

        asignarCoordenadas(nodo.izq, profundidad+1, xRef, hSpace, vSpace)

        nodo.x = xRef * hSpace
        nodo.y = profundidad * vSpace
        xRef++

        asignarCoordenadas(nodo.der, profundidad+1, xRef, hSpace, vSpace)

    METODO toSVG() -> string:
        asignarCoordenadas(raiz, 0, ref xRef, 50, 80)
        svg = "<svg>"

        PROCEDIMIENTO recDibujar(n):
            SI n == NULL: RETORNAR
            
            // Dibujar línea a hijos
            SI n.izq != NULL: svg += linea(n, n.izq)
            SI n.der != NULL: svg += linea(n, n.der)

            // Si el nodo es rojo-negro, usar su color:
            SI n.color == ROJO:
                svg += circulo(n.x, n.y, color="red")
            SINO:
                svg += circulo(n.x, n.y, color="black")

            svg += texto(n.x, n.y, n.toString())

            recDibujar(n.izq)
            recDibujar(n.der)

        recDibujar(raiz)
        svg += "</svg>"
        RETORNAR svg
```

---

## Archivo: `rbTree.cpp`  

```pseudocode
ENUM Color { ROJO, NEGRO }

TIPO NodoRB HEREDA NodoSVG:
    campo color : Color

    CONSTRUCTOR(dato):
        llamar CONSTRUCTOR NodoSVG(dato)
        color = ROJO  // todo nodo nuevo nace ROJO

// template T
TIPO ArbolRB HEREDA ArbolSVG:

    CONSTRUCTOR(T_str):
        llamar CONSTRUCTOR ArbolSVG(T_str)

    METODO insert(dato_ptr):
        nuevo = NodoRB(dato_ptr)
        nuevo.T = T

        // Inserción BST normal sobre el campo "dato"
        SI raiz == NULL:
            raiz = nuevo
            nuevo.color = NEGRO   // la raíz siempre es NEGRA
            RETORNAR

        it = raiz
        p  = NULL

        MIENTRAS it != NULL:
            p = it
            SI comparar(dato_ptr, it.dato) < 0:
                it = it.izq
            SINO:
                it = it.der

        SI comparar(dato_ptr, p.dato) < 0:
            p.izq = nuevo
        SINO:
            p.der = nuevo

        // nuevo entra como ROJO
        nuevo.color = ROJO

        // Arreglar posibles violaciones a las propiedades RB
        fixInsert(nuevo)


    METODO rotarIzquierda(x : NodoRB):
        y = x.der
        x.der = y.izq

        SI y.izq != NULL:
            padre(y.izq) = x

        // enlazar y con el padre de x
        padre_y = padre(x)
        padre(y) = padre_y

        SI padre_y == NULL:
            raiz = y
        SINO SI x == padre_y.izq:
            padre_y.izq = y
        SINO:
            padre_y.der = y

        y.izq = x
        padre(x) = y


    METODO rotarDerecha(y : NodoRB):
        x = y.izq
        y.izq = x.der

        SI x.der != NULL:
            padre(x.der) = y

        padre_x = padre(y)
        padre(x) = padre_x

        SI padre_x == NULL:
            raiz = x
        SINO SI y == padre_x.izq:
            padre_x.izq = x
        SINO:
            padre_x.der = x

        x.der = y
        padre(y) = x


    METODO fixInsert(n : NodoRB):
        // Mientras el padre sea ROJO, hay violación
        MIENTRAS n != raiz Y color(padre(n)) == ROJO:
            SI padre(n) es hijo IZQ de abuelo(n):
                tio = abuelo(n).der

                // Caso 1: tío ROJO -> recoloreo
                SI tio != NULL Y tio.color == ROJO:
                    padre(n).color   = NEGRO
                    tio.color        = NEGRO
                    abuelo(n).color  = ROJO
                    n = abuelo(n)

                SINO:
                    // Caso 2: n es hijo derecho -> rotación IZQ en padre
                    SI n == padre(n).der:
                        n = padre(n)
                        rotarIzquierda(n)

                    // Caso 3: n es hijo izquierdo -> rotación DER en abuelo
                    padre(n).color   = NEGRO
                    abuelo(n).color  = ROJO
                    rotarDerecha(abuelo(n))

            SINO:  // padre(n) es hijo DER (caso simétrico)
                tio = abuelo(n).izq

                SI tio != NULL Y tio.color == ROJO:
                    padre(n).color   = NEGRO
                    tio.color        = NEGRO
                    abuelo(n).color  = ROJO
                    n = abuelo(n)

                SINO:
                    SI n == padre(n).izq:
                        n = padre(n)
                        rotarDerecha(n)

                    padre(n).color   = NEGRO
                    abuelo(n).color  = ROJO
                    rotarIzquierda(abuelo(n))

        raiz.color = NEGRO

    METODO search(dato_ptr) -> NodoRB:
        it = raiz

        MIENTRAS it != NULL:
            cmp = comparar(dato_ptr, it.dato)

            SI cmp == 0:
                RETORNAR it
            SI cmp < 0:
                it = it.izq
            SINO:
                it = it.der

        RETORNAR NULL


```

---

## Archivo: `rbHash.cpp`  

```pseudocode
// tempalte T
TIPO ParClaveValor<T>:
    campo clave : string
    campo valor : T

// template T
TIPO RBHash<T>:
    campo arbol : ArbolRB

    CONSTRUCTOR():
        arbol = ArbolRB("pair")
    
    METODO buscarNodo(clave : string) -> NodoRB:
        // Crear un ParClaveValor "falso" solo para buscar por la clave
        temp = ParClaveValor<T>()
        temp.clave = clave

        // buscar en el ArbolRB usando search
        nodo = arbol.search(&temp)
        RETORNAR nodo

    METODO operator[](clave) -> referencia valor:
        nodo = buscarNodo(clave)

        SI nodo == NULL:
            nuevoPar = new ParClaveValor<T>
            nuevoPar.clave = clave
            arbol.insert(nuevoPar)
            nodo = buscarNodo(clave)

        par = (ParClaveValor<T>*) nodo.dato
        RETORNAR referencia par.valor
```

---

## Archivo: `main.cpp`  

```pseudocode
INICIO MAIN:

    RBHash<int> rankingMundial

    rankingMundial["Brasil"]    = 5
    rankingMundial["Italia"]    = 4
    rankingMundial["Alemania"]  = 4
    rankingMundial["Argentina"] = 3

    imprimir rankingMundial["Brasil"]
    imprimir rankingMundial["Argentina"]

    rankingMundial.arbol.toSVG("Mundiales.html")

FIN MAIN
```

