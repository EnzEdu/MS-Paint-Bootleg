/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pela definicao das estruturas (structs) do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data de apresentacao: 28/02/2023
 * Ultima modificacao:   11/04/2023
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
    double vtf[2];							// Vertices de transformacao final
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
