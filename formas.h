/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pelo desenho das formas no editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data de apresentacao: 28/02/2023
 * Ultima modificacao: 11/04/2023
*/

#ifndef formas_h
#define formas_h

#include <forward_list>


/*
 * Funcao responsavel por desenhar uma linha no editor grafico
 */
void desenhaLinha(double *x, double *y)
{
    forward_list<vertice> vertices = retaBresenham(x[0], y[0], x[1], y[1]);
    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
    {
        double desenhoX = v->x;
        double desenhoY = v->y;
        drawPixel(desenhoX, desenhoY, 0);
    }
}


/*
 * Funcao responsavel por desenhar um retangulo no editor grafico
 */
void desenhaRetangulo(double *x, double *y)
{
    forward_list<vertice> vertices;
    for (int k = 0; k < 4; k++)
    {
        if (k != 3)
        {
            vertices = retaBresenham(x[k], y[k], x[k+1], y[k+1]);
        }
        else
        {
            vertices = retaBresenham(x[k], y[k], x[0], y[0]);
        }

        for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
        {
            double desenhoX = v->x;
            double desenhoY = v->y;
            drawPixel(desenhoX, desenhoY, 0);
        }
    }
}


/*
 * Funcao responsavel por desenhar um triangulo no editor grafico
 */
void desenhaTriangulo(double *x, double *y)
{
    forward_list<vertice> vertices;
    for (int j = 0; j < 3; j++)
    {
        if (j != 2)
        {
            vertices = retaBresenham(x[j], y[j], x[j+1], y[j+1]);
        }
        else
        {
            vertices = retaBresenham(x[j], y[j], x[0], y[0]);
        }

        for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
        {
            double desenhoX = v->x;
            double desenhoY = v->y;
            drawPixel(desenhoX, desenhoY, 0);
        }
    }
}


/*
 * Funcao responsavel por desenhar um poligono no editor grafico
 * O modo se encerra quando o poligono possui no minimo quatro vertices,
 * e o usuario clica no vertice inicial
 */
void desenhaPoligono(double *x, double *y, int numVertices)
{
    forward_list<vertice> vertices;
    for (int j = 0; j < numVertices-1; j++)
    {
        vertices = retaBresenham(x[j], y[j], x[j+1], y[j+1]);

        for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
        {
            double desenhoX = v->x;
            double desenhoY = v->y;
            drawPixel(desenhoX, desenhoY, 0);
        }
    }
}


/*
 * Funcao responsavel por desenhar uma circunferencia no editor grafico
 */
void desenhaCircunferencia(double *x, double *y)
{
    forward_list<vertice> vertices = circuloBresenham(x[1], y[1], x[0], y[0]);
    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
    {
        double desenhoX = v->x;
        double desenhoY = v->y;
        drawPixel(desenhoX, desenhoY, 0);
    }
}


/*
 * Funcao responsavel por colorir o interior de uma forma
 */
void desenhaBalde(double x, double y, float *corAntiga, float *corNova)
{
    forward_list<vertice> vertices;
    algoritmoFloodFill(x, y, corAntiga, corNova, &vertices);
}



/*
 * Funcao responsavel por desenhar um retangulo preenchido no editor grafico
 */
void desenhaRetanguloPreenchido(double *x, double *y)
{
    forward_list<vertice> vertices = preenchePoligono(x, y, 4);
    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
    {
        double desenhoX = v->x;
        double desenhoY = v->y;
        drawPixel(desenhoX, desenhoY, 0);
    }
}


/*
 * Funcao responsavel por desenhar um triangulo preenchido no editor grafico
 */
void desenhaTrianguloPreenchido(double *x, double *y)
{
    forward_list<vertice> vertices = preenchePoligono(x, y, 3);
    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
    {
        double desenhoX = v->x;
        double desenhoY = v->y;
        drawPixel(desenhoX, desenhoY, 0);
    }
}


/*
 * Funcao responsavel por desenhar um poligono preenchido no editor grafico
 * O modo se encerra quando o poligono possui no minimo quatro vertices,
 * e o usuario clica no vertice inicial
 */
void desenhaPoligonoPreenchido(double *x, double *y, int numVertices)
{
    forward_list<vertice> vertices = preenchePoligono(x, y, numVertices);
    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
    {
        double desenhoX = v->x;
        double desenhoY = v->y;
        drawPixel(desenhoX, desenhoY, 0);
    }
}


#endif
