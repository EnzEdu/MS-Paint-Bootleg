/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pelas transformacoes geometricas do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: 28/02/2023
*/

#ifndef transformacoes_h
#define transformacoes_h


/*
 * Funcao responsavel por calcular a transformacao de translacao de um determinado vertice
*/
void translacao(double *desenhoX, double *desenhoY, double Tx, double Ty)
{
    double matriz[] = {1 , 0 , 0,
                       0 , 1 , 0,
                       Tx, Ty, 1};

    // Coordenadas
    *desenhoX = *desenhoX + matriz[6];
    *desenhoY = *desenhoY + matriz[7];
}


/*
 * Funcao responsavel por calcular a transformacao de escala de um determinado vertice
*/
void escala(double *desenhoX, double *desenhoY, double Sx, double Sy)
{
    double matriz[] = {Sx, 0 , 0,
                       0 , Sy, 0,
                       0 , 0 , 1};

    // Coordenadas
    *desenhoX = (*desenhoX * matriz[0]);
    *desenhoY = (*desenhoY * matriz[4]);
}


/*
 * Funcao responsavel por calcular a transformacao de cisalhamento de um determinado vertice
*/
void cisalhamento(double *desenhoX, double *desenhoY, double eixo, double valor)
{
    switch ((char) eixo)
    {
        case 'x':
            {
                double c = valor - 48;

                double matriz[] = {1, 0, 0,
                                   c, 1, 0,
                                   0, 0, 1};

                // Coordenadas
                *desenhoX = *desenhoX + (*desenhoY * matriz[3]);
                *desenhoY = *desenhoY;
            }
        break;

        case 'y':
            {
                double c = valor - 48;

                double matriz[] = {1, c, 0,
                                   0, 1, 0,
                                   0, 0, 1};

                // Coordenadas
                *desenhoX = *desenhoX;
                *desenhoY = *desenhoY + (*desenhoX * matriz[1]);
            }
        break;
    }
}


/*
 * Funcao responsavel por calcular a transformacao de reflexao de um determinado vertice
*/
void reflexao(double *desenhoX, double *desenhoY, double eixo)
{
    switch ((char) eixo)
    {
        case 'x':
            {
                double matriz[] = {1,  0, 0,
                                   0, -1, 0,
                                   0,  0, 1};

                // Coordenadas
                *desenhoX  = *desenhoX;
                *desenhoY *= matriz[4];
            }
        break;

        case 'y':
            {
                double matriz[] = {-1, 0, 0,
                                    0, 1, 0,
                                    0, 0, 1};

                // Coordenadas
                *desenhoX *= matriz[0];
                *desenhoY  = *desenhoY;
            }
        break;

        case '0':
            {
                double matriz[] = {-1,  0, 0,
                                    0, -1, 0,
                                    0,  0, 1};

                // Coordenadas
                *desenhoX *= matriz[0];
                *desenhoY *= matriz[4];
            }
        break;
    }
}


/*
 * Funcao responsavel por calcular a transformacao de rotacao de um determinado vertice
*/
void rotacao(double *desenhoX, double *desenhoY, double teta)
{
    double matriz[] = { cos(teta), sin(teta), 0,
                       -sin(teta), cos(teta), 0,
                                0,         0, 1};

    double tempX = (*desenhoX * matriz[0]) + (*desenhoY * matriz[3]);
    double tempY = (*desenhoX * matriz[1]) + (*desenhoY * matriz[4]);

    // Coordenadas
    *desenhoX = tempX;
    *desenhoY = tempY;
}


#endif
