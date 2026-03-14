<div align="center">
 
# 🧠 Teoría de la Computación
### ESCOM · Instituto Politécnico Nacional
 
[![Language: C](https://img.shields.io/badge/Language-C-00599C?style=flat-square&logo=c)](https://en.wikipedia.org/wiki/C_(programming_language))
[![Language: C++](https://img.shields.io/badge/Language-C++-004482?style=flat-square&logo=cplusplus)](https://isocpp.org/)
[![Institution](https://img.shields.io/badge/Institution-ESCOM--IPN-9B1C31?style=flat-square)](https://www.escom.ipn.mx/)
[![Topics](https://img.shields.io/badge/Topics-Automata%20%7C%20Grammars%20%7C%20Turing-6D28D9?style=flat-square)](#-contenido)
 
> Implementaciones en **C/C++** de los modelos computacionales fundamentales: autómatas finitos, gramáticas formales, autómatas de pila y máquinas de Turing — construidos desde cero como parte del programa de **Ingeniería en Sistemas Computacionales** en la ESCOM, IPN.
 
</div>
 
---
 
## 📌 Descripción
 
Este repositorio contiene las prácticas de la unidad de aprendizaje **Teoría de la Computación**, impartida por la **Profra. Luz María Sánchez García** en la Escuela Superior de Cómputo (ESCOM) del Instituto Politécnico Nacional.
 
El objetivo central de la materia es estudiar los fundamentos matemáticos de la computación: qué problemas pueden resolverse algorítmicamente, qué modelos de máquinas los reconocen y cuáles son sus límites de expresividad. Las implementaciones están escritas en **ANSI C** y **C++** sin dependencias externas, priorizando la comprensión del modelo formal sobre el uso de bibliotecas.
 
---
 
## 🗂 Contenido
 
```
Teoria-de-la-computacion/
│
└── Practicas/
    ├── Practica 1 — CADENAS Y LENGUAJES
    ├── Practica 2 — OPERACIONES CON LENGUAJES
    ├── Practica 3 — OTRAS OPERACIONES CON CADENAS Y LENGUAJES
    
``` 
---
 
## 🔬 Temas cubiertos
 
| Unidad | Tema | Conceptos clave |
|--------|------|-----------------|
| 1 | Lenguajes y cadenas | Alfabetos, operaciones con cadenas, lenguajes formales |
| 2 | Lenguajes regulares | AFD, AFN, transiciones-ε, equivalencia AFN↔AFD |
| 3 | Propiedades de regulares | Lema de bombeo, Teorema de Myhill-Nerode |
| 4 | Lenguajes libres de contexto | Gramáticas GLC, FNC, FNG, árbol de derivación |
| 5 | Autómatas de pila | AP determinista y no determinista |
| 6 | Máquinas de Turing | Modelo estándar, variantes, decidibilidad |
| 7 | Límites de la computación | Problemas indecidibles, reducción, halting problem |
 
---
 
## ⚙️ Compilación y ejecución
 
Las prácticas están escritas en **ANSI C** o **C++** y compilan con `gcc`/`g++` estándar — sin dependencias externas.
 
```bash
# Clonar el repositorio
git clone https://github.com/AlanRLG/Teoria-de-la-computacion.git
cd Teoria-de-la-computacion
 
# Compilar una práctica individual (C)
gcc -ansi -Wall -o practica Practicas/Practica\ 2/afd.c
 
# Compilar con C++
g++ -std=c++17 -Wall -o practica Practicas/Practica\ 4/afn_to_afd.cpp
 
# Ejecutar
./practica
```
---
 
## 📚 Bibliografía
 
- Hopcroft, J., Motwani, R., Ullman, J. — *Introducción a la Teoría de Autómatas, Lenguajes y Computación* — Addison Wesley, 2008. ISBN: 978-84-7829-088-8
- Linz, P. — *An Introduction to Formal Languages and Automata* — Jones & Bartlett, 2001. ISBN: 0-7637-1422-4
- Kelley, D. — *Teoría de Autómatas y Lenguajes Formales* — Prentice Hall, 1995. ISBN: 0-13-518705-2
 
---
 
## 🏫 Contexto académico
 
| Campo | Detalle |
|-------|---------|
| **Institución** | Escuela Superior de Cómputo — IPN |
| **Carrera** | Ingeniería en Sistemas Computacionales (ISC) |
| **Profesora** | Luz María Sánchez García |
| **Materia** | Teoría de la Computación (Teoría + Práctica) |
| **Lenguajes** | C (ANSI) · C++ |
 
---
 
## 👤 Autores
 
**Alan R. L. G.**
Estudiante de ISC en ESCOM · IPN
 
[![GitHub](https://img.shields.io/badge/GitHub-AlanRLG-181717?style=flat-square&logo=github)](https://github.com/AlanRLG)

**Garcia Ambrosio**
Estudiante de ISC en ESCOM · IPN
[![GitHub](https://img.shields.io/badge/GitHub-AbsorbedSun-181717?style=flat-square&logo=github)](https://github.com/AbsorbedSun)

**Andrawi Pérez**
Estudiante de ISC en ESCOM · IPN
[![GitHub](https://img.shields.io/badge/GitHub-andrawiP801-181717?style=flat-square&logo=github)](https://github.com/andrawiP801)
 
---
 
<div align="center">
<sub>Escuela Superior de Cómputo · Instituto Politécnico Nacional · México</sub>
</div>
 
