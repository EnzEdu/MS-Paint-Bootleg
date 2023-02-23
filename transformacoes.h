/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pelas transformacoes geometricas do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: ??/02/2023
*/

#ifndef transformacoes_h
#define transformacoes_h


void translacao(float *desenhoX, float *desenhoY, float Tx, float Ty)
{
    float matriz[] = {1 , 0 , 0,
                      0 , 1 , 0,
                      Tx, Ty, 1};

    // Coordenadas da translacao
    *desenhoX = *desenhoX + matriz[6];
    *desenhoY = *desenhoY + matriz[7];
}


void escala(float *desenhoX, float *desenhoY, float Sx, float Sy)
{
    float matriz[] = {Sx, 0 , 0,
                      0 , Sy, 0,
                      0 , 0 , 1};

    // Coordenadas da escala
    *desenhoX = (*desenhoX * matriz[0]);
    *desenhoY = (*desenhoY * matriz[4]);
}


void cisalhamento(float *desenhoX, float *desenhoY, float eixo, float valor)
{
    switch ((char) eixo)
    {
        case 'x':
            {
                float c = valor - 48;

                float matriz[] = {1, 0, 0,
                                  c, 1, 0,
                                  0, 0, 1};

                *desenhoX = *desenhoX + (*desenhoY * matriz[3]);
                *desenhoY = *desenhoY;
            }
        break;

        case 'y':
            {
                float c = valor - 48;

                float matriz[] = {1, c, 0,
                                  0, 1, 0,
                                  0, 0, 1};

                *desenhoX = *desenhoX;
                *desenhoY = *desenhoY + (*desenhoX * matriz[1]);
            }
        break;
    }
}


void reflexao(float *desenhoX, float *desenhoY, float eixo)
{
    switch ((char) eixo)
    {
        case 'x':
            {
                float matriz[] = {1,  0, 0,
                                  0, -1, 0,
                                  0,  0, 1};

                *desenhoX  = *desenhoX;
                *desenhoY *= matriz[4];
            }
        break;

        case 'y':
            {
                float matriz[] = {-1, 0, 0,
                                   0, 1, 0,
                                   0, 0, 1};

                *desenhoX *= matriz[0];
                *desenhoY  = *desenhoY;
            }
        break;

        case '0':
            {
                float matriz[] = {-1,  0, 0,
                                   0, -1, 0,
                                   0,  0, 1};

                *desenhoX *= matriz[0];
                *desenhoY *= matriz[4];
            }
        break;
    }
}


void rotacao(float *desenhoX, float *desenhoY, float teta)
{
    double matriz[] = { cos(teta), sin(teta), 0,
                       -sin(teta), cos(teta), 0,
                                0,         0, 1};

    float tempX = (*desenhoX * matriz[0]) + (*desenhoY * matriz[3]);
    float tempY = (*desenhoX * matriz[1]) + (*desenhoY * matriz[4]);

    *desenhoX = tempX;
    *desenhoY = tempY;
}


#endif