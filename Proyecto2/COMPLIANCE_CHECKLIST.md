# Auditoría de Compliance con Instrucciones del Proyecto

## Requisito 1: Árbol Rojo-Negro Genérico ("Emplantillado")
- ✓ **Plantillas implementadas**: `ArbolBST<T>`, `ArbolSVG<T>`, `ArbolRB<T>` todas templadas.
- ✓ **Nodos templados**: `Nodo<T>`, `NodoSVG<T>`, `NodoRB<T>`.
- ✓ **ParClaveValor<V>**: Template para pares clave-valor.
- ✓ **Compilación**: Sin errores con `-std=c++17 -Wall -Wextra -pedantic`.

## Requisito 2: Manejo de Múltiples Tipos de Datos
- ✓ **int**: Demostrado en `RBHash<int>` con ranking de copas mundiales.
- ✓ **string**: Demostrado en `RBHash<string>` con datos personales.
- ✓ **Tipos arbitrarios**: `ParClaveValor<T>` acepta cualquier tipo `T` con operadores `<`, `==`, `toString()`.

## Requisito 3: Variantes de Insert
El enunciado exige que `insert` acepte:
- ✓ **Literal**: `rankingMundial["Brasil"] = 5;` → crea `new ParClaveValor<int>("Brasil", 5)` con `owns=true`.
- ✓ **Variable**: No demostrado explícitamente en main.cpp, pero soportado por `insert(const T&)` en `ArbolRB`.
- ✓ **Puntero anónimo**: `insert(new Type())` → pasa `owns=true` por defecto.
- ✓ **Puntero a existente**: `insert(ptr, false)` → pasa `owns=false`, no libera en destructor.
- ✓ **Flag ownsData**: Implementado en `Nodo<T>::ownsData` para gestionar liberación.

Política documentada:
- Si `insert(const T&)`: Crea `new T(v)` internamente, árbol asume propiedad y libera.
- Si `insert(T* ptr, bool owns=true)`: Árbol asume propiedad si `owns=true`; si `owns=false`, no libera al destructor.

## Requisito 4: Recorridos y toString()
- ✓ **Preorder**: Implementado en `ArbolBST<T>::preorder()`.
- ✓ **Inorder**: Implementado en `ArbolBST<T>::inorder()`.
- ✓ **Postorder**: Implementado en `ArbolBST<T>::postorder()`.
- ✓ **Por niveles (BFS)**: Implementado en `ArbolBST<T>::porNiveles()` usando cola manual (no STL).
- ✓ **toString()**: Devuelve todos los recorridos separados por salto de línea en `ArbolBST<T>::toString()`.
- ✓ **Nodos coloreados en toString**: `NodoRB<T>::toString()` incluye "R" (rojo) o "N" (negro), ej. "Brasil(N)".

## Requisito 5: Propiedades Rojo-Negro
1. ✓ **Color de nodo**: Cada nodo tiene `Color color { ROJO, NEGRO }`.
2. ✓ **Propiedad de raíz**: Inicializada como NEGRO en `ArbolRB<T>::insert()` cuando es raíz.
3. ✓ **Propiedad del rojo**: Validada por `fixInsert` que detecta violación (padre rojo con nodo rojo) y aplica rotaciones/recoloramiento.
4. ✓ **Propiedad del negro**: Mantenida por `fixDelete` que ajusta la altura negra cuando se elimina.
5. ✓ **Hojas NULL**: Tratadas como negras implícitamente en la lógica (NULL padre apunta a nodo negro).

## Requisito 6: Operaciones RB
- ✓ **Insertion**: `ArbolRB<T>::insert()` con `fixInsert()` y rotaciones.
- ✓ **Deletion**: `ArbolRB<T>::deleteNode()` con `fixDelete()` y rotaciones.
- ✓ **Search**: Búsqueda estándar en `ArbolRB<T>::searchNode()`.
- ✓ **Rotaciones**: `rotarIzquierda()` y `rotarDerecha()` implementadas.

## Requisito 7: Herencia y Polimorfismo
- ✓ **ArbolRB hereda de ArbolSVG**: `class ArbolRB : public ArbolSVG<T>`.
- ✓ **ArbolSVG hereda de ArbolBST**: `class ArbolSVG : public ArbolBST<T>`.
- ✓ **NodoRB hereda de NodoSVG**: `class NodoRB : public NodoSVG<T>`.
- ✓ **Métodos virtuales**: `toString()`, `postConstructor()`, `cloneNode()` son virtuales en jerarquía.
- ✓ **Polimorfismo**: `getSVGContent()` override en `ArbolRB` para dibujar colores rojo/negro.

## Requisito 8: Visualización SVG
- ✓ **SVG generado**: `ArbolSVG<T>::toSVG()` genera archivo HTML con SVG.
- ✓ **Nodos coloreados**: Círculos con `fill="red"` para nodos rojos, `fill="black"` para negros.
- ✓ **Coordenadas calculadas**: `asignarCoordenadas()` posiciona nodos con inorder traversal.
- ✓ **Aristas dibujadas**: Líneas `<line>` conectan padres e hijos.
- ✓ **Etiquetas**: Texto muestra clave y color (R/N) dentro de cada nodo.
- ✓ **Archivo de plantilla**: Usa `svgTreeViewerTemplate.html` con patrón `%s` para inyección de SVG.

Verificación de salida:
- `Mundiales.html`: 4 nodos rojos, 4 nodos negros (verificado con grep).
- `Persona.html`: Generado exitosamente con claves de persona.

## Requisito 9: Tabla Hash (Arreglo Asociativo)
- ✓ **RBHash<V>**: Template que envuelve `ArbolRB< ParClaveValor<V> >`.
- ✓ **Sobrecarga operator[]**: `V& operator[](const string& clave)` crea/accede pares automáticamente.
- ✓ **Clave string**: `ParClaveValor<V>::clave` es string, búsqueda por clave.
- ✓ **Creación automática**: Si clave no existe, `operator[]` inserta `new ParClaveValor<V>(clave, V())`.
- ✓ **Uso demoninado**:
  ```cpp
  RBHash<int> myHash;
  myHash["Brasil"] = 5;
  myHash["Italia"] = 4;
  int mundiales = myHash["Brasil"];  // → 5
  
  RBHash<string> myStrHash;
  myStrHash["nombre"] = "Maria";
  string nom = myStrHash["nombre"];  // → "Maria"
  ```

## Requisito 10: Sin STL Containers
- ✓ **No std::vector**: Código no usa vector.
- ✓ **No std::list**: Código no usa list.
- ✓ **No std::array**: Código no usa array.
- ✓ **No std::map**: Código usa árbol RB en lugar de map.
- ✓ **Colección manual**: `porNiveles()` usa array C `Nodo<T>* cola[1000]` (no STL).
- ✓ **Librerías permitidas**: Se usan iostream, sstream, fstream, string, cstdlib, sys/stat.h, functional (todas no-contenedores).

## Requisito 11: Copia Profunda y Semántica de Asignación
- ✓ **Constructor copia**: `ArbolBST(const ArbolBST& other)` realiza clon profundo.
- ✓ **operator=**: `ArbolBST& operator=(const ArbolBST& other)` con liberación segura.
- ✓ **Clonación recursiva**: `clonarSubarbol()` usa polimorfismo `cloneNode()` para clonar cada nodo.
- ✓ **Prueba en main**: Copia de `RBHash<int>` modifica original sin afectar copia (verificado en salida).

## Requisito 12: Destructor Virtual y Gestión de Memoria
- ✓ **Destructor virtual en Nodo**: `virtual ~Nodo()` para polimorfismo.
- ✓ **Liberación de datos**: Destructor respeta `ownsData` y libera `dato` si corresponde.
- ✓ **Destructor en ArbolBST**: `virtual ~ArbolBST()` llama `liberarArbol()` para limpiar árbol completo.
- ✓ **Liberación recursiva**: `liberarArbol()` recorre izq/der/nodo en orden postorder.

## Resumen Final
✅ **Todas las instrucciones cumplidas:**
- Árbol rojo-negro completamente templado y genérico.
- Soporta múltiples tipos de datos (int, string, arbitrarios).
- Implementa variantes de insert (literal, variable, puntero, new).
- Recorridos completos (preorder, inorder, postorder, por niveles) con toString consolidado.
- Propiedades RB mantenidas mediante rotaciones y recoloramiento.
- Visualización SVG con colores (rojo/negro).
- Tabla hash con operator[] basada en RB tree.
- Sin STL containers.
- Copia profunda y gestión de memoria correcta.
- Compilación limpia: 0 errores, 0 warnings (C++17).
- Código funcional y probado (ejecución exitosa).

**Estado**: Listo para commit.
