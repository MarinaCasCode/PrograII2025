# Proyecto 2 — Red-Black Hash con Visualización SVG

## Descripción General

Este proyecto implementa una estructura de datos **RBHash**, que simula el comportamiento de una **tabla hash**, pero internamente utiliza un **árbol Rojo-Negro (balanced BST)** para mantener los datos ordenados, balanceados y con acceso eficiente.

Además, el programa permite visualizar el árbol generado en formato **SVG interactivo**, mostrando los nodos con sus respectivos **colores rojo/negro**, así como la estructura completa del árbol.

---

## Funcionalidades implementadas

### Árbol Rojo-Negro (RBTree)
Basado en `ArbolBST`, pero mejorado con:
- Inserción con **balanceo automático**
- Colores **rojo y negro**
- **Rotaciones izquierda y derecha**
- **Arreglos de los casos clásicos de inserción y eliminación**
- Soporte para claves de **diferentes tipos**

---

### Hash Rojo-Negro (RBHash)
Tabla hash implementada con árbol rojo-negro como estructura interna. Combina la interfaz de un mapa asociativo (acceso mediante claves tipo string con el operador `[]`) con las ventajas de un árbol balanceado.

---

### Visualización SVG
El proyecto genera archivos `.html` que muestran gráficamente el árbol rojinegro con colores.

Ejemplo de archivos generados:
```
arbolRB_test.html
mundiales_rbhash.html
persona_rbhash.html
```

---

## Ejecución del Programa

### Compilación

```bash
g++ -o proyecto2 main.cpp
```

### Ejecución

```bash
./proyecto2
```

## Estructura del Proyecto

```
Proyecto2/
│── main.cpp
│── bstTree.cpp
│── rbTree.cpp
│── rbHash.cpp
│── svgTreeViewer.cpp
│── svgTreeViewerTemplate.html
└── README.md
```

---

## Componentes Principales

| Archivo | Rol |
|---------|-----|
| `bstTree.cpp` | Árbol binario base con búsqueda, recorridos y delete básico |
| `rbTree.cpp` | Implementación del árbol rojinegro con rotaciones y balanceo |
| `rbHash.cpp` | Hash basado en árbol rojo-negro, con `operator[]` |
| `svgTreeViewer.cpp` | Generación del archivo SVG/HTML interactivo |
| `main.cpp` | Programa demostrativo del proyecto |

---

## Créditos

**Marina Castro Peralta C31886**
**Gabriel Herrera Rodríguez C33830**

**Curso:** CI-0113 Programación II  
**Profesor:** Enrique Muñoz Monge  
**Año:** 2025

---
