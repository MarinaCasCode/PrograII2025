# Árbol Roji-Negro (Red-Black Tree)

## Introducción

Un árbol roji-negro (Red-Black Tree) es una estructura de datos de árbol binario de búsqueda auto-balanceado que garantiza operaciones eficientes de inserción, eliminación y búsqueda en tiempo O(log n). A diferencia de los árboles binarios de búsqueda tradicionales, los árboles roji-negros mantienen el balance mediante el uso de colores (rojo y negro) en sus nodos y un conjunto de reglas específicas.

## Propiedades clave de un Árbol RB

1. **Color de nodo**: Cada nodo está coloreado ya sea rojo o negro.

2. **Propiedad de la raíz**: El nodo raíz siempre es negro.

3. **Propiedad del Rojo**: Los nodos rojos no pueden tener hijos rojos (no pueden existir dos nodos rojos consecutivos en un camino).

4. **Propiedad del Negro**: Cada camino desde un nodo hasta una de sus hojas descendientes tiene el mismo número de nodos negros (altura negra).

5. **Nodos hoja**: Todas las hojas (sean NULL o con centinela) son considerados de color negro.

## Operaciones básicas

### Insertion (Inserción)

Cuando un nodo se agrega es inicialmente coloreado rojo. Si viola las propiedades RB, se necesita hacer rotaciones y re-coloramiento para restaurar las propiedades.

**Casos de inserción:**
- **Caso 1**: El nodo insertado es la raíz → Se colorea de negro.
- **Caso 2**: El padre del nodo insertado es negro → No se viola ninguna propiedad.
- **Caso 3**: El padre y el tío son rojos → Se recolorean padre, tío y abuelo.
- **Caso 4**: El padre es rojo y el tío es negro → Se realizan rotaciones (simple o doble según la configuración).

### Deletion (Eliminación)

Eliminar un nodo puede provocar violaciones de las propiedades, las cuales también se arreglan mediante rotaciones y re-coloramiento, similares a los ajustes por inserción.

**Casos de eliminación:**
- Si el nodo a eliminar es rojo, simplemente se elimina.
- Si el nodo a eliminar es negro, se deben realizar ajustes para mantener la altura negra.
- Se pueden requerir rotaciones y recoloramiento del hermano y sus hijos.

### Search (Búsqueda)

La búsqueda estándar de un árbol de búsqueda binaria, se ve beneficiado del mantenimiento del balance.

**Complejidad temporal**: O(log n) debido al balanceo automático del árbol.

## Rotaciones

Las rotaciones son operaciones fundamentales para mantener el balance del árbol:

### Rotación Simple a la Derecha
```
    y                    x
   / \                  / \
  x   C    --->        A   y
 / \                      / \
A   B                    B   C
```

### Rotación Simple a la Izquierda
```
  x                      y
 / \                    / \
A   y      --->        x   C
   / \                / \
  B   C              A   B
```

## Complejidad Temporal

| Operación | Complejidad |
|-----------|-------------|
| Búsqueda  | O(log n)    |
| Inserción | O(log n)    |
| Eliminación | O(log n)  |
| Espacio   | O(n)        |

## Ventajas del Árbol Roji-Negro

1. **Auto-balanceo**: Mantiene automáticamente el balance del árbol, garantizando un rendimiento consistente.
2. **Garantías de tiempo**: Todas las operaciones principales tienen garantía de O(log n) en el peor caso.
3. **Menos rotaciones que AVL**: Aunque ambos son balanceados, los árboles RB requieren menos rotaciones en promedio durante inserciones y eliminaciones.
4. **Ampliamente utilizado**: Implementado en bibliotecas estándar de muchos lenguajes (C++ STL map/set, Java TreeMap/TreeSet, etc.).

## Desventajas

1. **Complejidad de implementación**: La lógica de mantenimiento de colores y rotaciones es más compleja que un BST simple.
2. **Overhead de memoria**: Requiere almacenar el bit de color adicional para cada nodo.
3. **Menos balanceado que AVL**: Los árboles AVL están más estrictamente balanceados, lo que puede ser mejor para aplicaciones con muchas búsquedas.

## Aplicaciones

- **Sistemas operativos**: Gestión de memoria y scheduling de procesos (Linux kernel).
- **Bases de datos**: Índices y estructuras de datos internas.
- **Librerías estándar**: Implementación de mapas y conjuntos ordenados.
- **Sistemas de archivos**: Organización eficiente de metadata.

## Requerimientos de Implementación para este Proyecto

1. **Herencia**: El ArbolRB es una derivación de un ArbolBST, es decir, el ArbolRB hereda el comportamiento de un ArbolBST.

2. **Extensión con métodos propios**: Debe extender el comportamiento original mediante atributos y métodos propios de los árboles rojinegros, tales como los métodos de rotación.

3. **Funcionalidad SVG**: Además del comportamiento original de un ArbolBST, el ArbolRB deberá proveer una funcionalidad extra que permita obtener una representación del árbol en formato SVG (Scalable Vector Graphics) para ser visualizado en gráficamente con todo y sus colores.

4. **Genericidad**: Al igual que la base, el ArbolRB deberá ser completamente genérico ("emplantillado"), es decir, deberá poder administrar cualquier tipo de dato primitivo (int, float, double, char, etc), así como tipo string y además tipos arbitrarios (clases personalizadas).

## Referencias

- Cormen, T. H., Leiserson, C. E., Rivest, R. L., & Stein, C. (2009). *Introduction to Algorithms* (3rd ed.). MIT Press.
- Sedgewick, R., & Wayne, K. (2011). *Algorithms* (4th ed.). Addison-Wesley.
- Weiss, M. A. (2014). *Data Structures and Algorithm Analysis in C++* (4th ed.). Pearson.
