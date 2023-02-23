/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pela definicao das estruturas do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: ??/02/2023
*/

#ifndef estruturas_h
#define estruturas_h

#include <forward_list>


// Definicao de vertice
struct vertice
{
    int x;
    int y;
};


// Definicao das transformacoes geometricas
struct transformacao
{
    int tipo;
    float vtf[2];							// Vertices de transformacao final
};


// Definicao das formas geometricas
struct forma
{
    int tipo;
    float rCor, gCor, bCor;
    forward_list<vertice> v;				// Lista encadeada de vertices
    forward_list<transformacao> t;			// Lista encadeada de transformacoes
};


#endif