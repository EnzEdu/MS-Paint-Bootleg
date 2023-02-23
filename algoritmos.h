/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pelos algoritmos usados no editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: ??/02/2023
*/

#ifndef algoritmos_h
#define algoritmos_h


/*
 * Funcao que implementa o Algoritmo de Bresenham para rasterizacao de segmentos de retas
*/
forward_list<vertice> retaBresenham(double x1, double y1, double x2, double y2)
{
    // Lista de vertices a serem desenhados
    forward_list<vertice> listaVertices;


    // Coordenadas originais
    int xInicio = (int) x1;
    int yInicio = (int) y1;
    vertice v1; v1.x = x1; v1.y = y1;
    listaVertices.push_front(v1);

    int xFim    = (int) x2;
    int yFim    = (int) y2;
    vertice v2; v2.x = x2; v2.y = y2;
    listaVertices.push_front(v2);
 
    // Variaveis - parte 1
    int variacaoX = xFim - xInicio;
    int variacaoY = yFim - yInicio;


    // Reducao ao primeiro octante
    int tmp;

        // Verifica se o declive eh negativo
        bool simetrico = false;

        int coeficienteAngular = variacaoX * variacaoY;
        if (coeficienteAngular < 0)
        {
            simetrico = true;

            // Troca o sinal das coordenadas Y
            yInicio   *= (-1);
            yFim      *= (-1);
            variacaoY *= (-1);
        }

        // Verifica se o declive eh superior a 1
        bool declive = false;

        if (abs(variacaoX) < abs(variacaoY))
        {
            declive = true;

            // Troca a posicao de x e y em cada coordenada
            tmp = xInicio;
            xInicio = yInicio;
            yInicio = tmp;

            tmp = xFim;
            xFim = yFim;
            yFim = tmp;

            tmp = variacaoX;
            variacaoX = variacaoY;
            variacaoY = tmp;
        }

        // Verifica se xInicio eh maior que xFim
        if (xInicio > xFim)
        {
            // Troca a ordem das coordenadas
            tmp = xInicio;
            xInicio = xFim;
            xFim = tmp;

            tmp = yInicio;
            yInicio = yFim;
            yFim = tmp;

            variacaoX *= (-1);
            variacaoY *= (-1);
        }


    // Variaveis - parte 2
    int d     = (2 * variacaoY) - variacaoX;
    int incE  = (2 * variacaoY);
    int incNE = 2 * (variacaoY - variacaoX);



    // Algoritmo de Bresenham
    int bresenhamX, bresenhamY;
    int Yi = yInicio;

    for (int Xi = xInicio; Xi <= xFim; Xi++)
    {
        // Primeiro ponto
        if (Xi == xInicio)
        {
            bresenhamX = xInicio;
            bresenhamY = yInicio;
        }

        // Ultimo ponto
        else if (Xi == xFim)
        {
            bresenhamX = xFim;
            bresenhamY = yFim;
        }

        // Restante
        else
        {
            bresenhamX = Xi;

            if (d <= 0)
            {
                d += incE;      // Avanco pro Leste
            }
            else
            {
                d  += incNE;    // Avanco pro Nordeste
                Yi += 1;
            }

            bresenhamY = Yi;
        }


        // Transformacao inversa
        if (declive == true)
        {
            tmp = bresenhamX;
            bresenhamX = bresenhamY;
            bresenhamY = tmp;
        }

        if (simetrico == true)
        {
            bresenhamY *= (-1);
        }

        vertice v;
        v.x = bresenhamX;
        v.y = bresenhamY;
        listaVertices.push_front(v);
    }

    return listaVertices;
}




/*
 * Funcao que implementa o Algoritmo de Bresenham para rasterizacao de circunferencias
*/
forward_list<vertice> circuloBresenham(double x1, double y1, double x2, double y2)
{
    // Lista de vertices a serem desenhados
    forward_list<vertice> listaVertices;


    // Coordenadas originais
    int xCentro = (int) x1;
    int yCentro = (int) y1;
    int xRaio   = (int) x2;
    int yRaio   = (int) y2;

    // Variaveis    
    int d     = 1 - yRaio;
    int incE  = 3;
    int incSE = (-2 * yRaio) + 5;


    // Algoritmo de Bresenham
    int bresenhamX, bresenhamY;
    int Yi = yRaio;

    for (int Xi = 0; Yi > Xi; Xi++)
    {
        // Primeiro ponto e Ultimo ponto
        if (Xi == 0 || Xi == Yi)
        {
            bresenhamX = Xi;
            bresenhamY = Yi;
        }

        // Restante
        else
        {
            bresenhamX = Xi;

            if (d < 0)
            {
                d     += incE;      // Avanco pro Leste
                incE  += 2;
                incSE += 2;
            }
            else
            {
                d     += incSE;     // Avanco pro Sudeste
                incE  += 2;
                incSE += 4;
                Yi--;
            }

            bresenhamY = Yi;
        }


        // Desenha os pontos (Translacao + Simetria dos Octantes)

        // Coordenada a ser rasterizada eh (0, R)
        vertice v;
        if (Xi == 0)
        {
            v.x = xCentro + bresenhamX; v.y = yCentro + bresenhamY;
            listaVertices.push_front(v);
            v.x = xCentro + bresenhamX; v.y = yCentro - bresenhamY;
            listaVertices.push_front(v);
            v.x = xCentro + bresenhamY; v.y = yCentro + bresenhamX;
            listaVertices.push_front(v);
            v.x = xCentro - bresenhamY; v.y = yCentro - bresenhamX;
            listaVertices.push_front(v);
        }
        else
        {
            v.x = xCentro + bresenhamX; v.y = yCentro + bresenhamY;
            listaVertices.push_front(v);
            v.x = xCentro + bresenhamY; v.y = yCentro + bresenhamX;
            listaVertices.push_front(v);
            v.x = xCentro + bresenhamY; v.y = yCentro - bresenhamX;
            listaVertices.push_front(v);
            v.x = xCentro + bresenhamX; v.y = yCentro - bresenhamY;
            listaVertices.push_front(v);

            v.x = xCentro - bresenhamX; v.y = yCentro - bresenhamY;
            listaVertices.push_front(v);
            v.x = xCentro - bresenhamY; v.y = yCentro - bresenhamX;
            listaVertices.push_front(v);
            v.x = xCentro - bresenhamY; v.y = yCentro + bresenhamX;
            listaVertices.push_front(v);
            v.x = xCentro - bresenhamX; v.y = yCentro + bresenhamY;
            listaVertices.push_front(v);
        }
    }

    return listaVertices;
}




/*
 * question mark
*/
forward_list<vertice> rasterizaPoligono(int coordsX[], int tamX, int coordsY[], int tamY)
{
    // Lista de vertices a serem desenhados
    forward_list<vertice> listaVertices;

    return listaVertices;
}












bool confereCor(int corPixelArea[], int corPixelClick[])
{
    return (corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]);
}




/*
 * Funcao que implementa o Algoritmo Flood Fill para preenchimento de poligonos
*/
forward_list<vertice> algoritmoFloodFill(int x1, int y1)
{
    // Lista de vertices a serem desenhados
    forward_list<vertice> listaVertices;


    // Guarda a cor do pixel clicado
    int corPixelClick[4];
    glReadPixels(x1, y1, 1, 1, GL_RGB, GL_INT, corPixelClick);

    // Guarda a cor do pixel atual
    int corPixelArea[4];



    // Para cada pixel acima do pixel clicado
    int y2 = y1 + 1;
    while (y2 <= height - 50)
    {
        int leftLimSup = x1, rightLimSup = x1;
        glReadPixels(x1, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);

        if (confereCor(corPixelArea, corPixelClick) == true)
        {
            // Procura pelo limite esquerdo da linha
            while (leftLimSup >= 0)
            {
                glReadPixels(leftLimSup, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    leftLimSup--;
                    continue;
                }

                break;
            }

            // Procura pelo limite direito da linha
            while (rightLimSup <= width)
            {
                glReadPixels(rightLimSup, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    rightLimSup++;
                    continue;
                }

                break;
            }

            // Desenha a reta do pixel acima
            vertice v;
            for (int i = leftLimSup+1; i < rightLimSup; i++)
            {
                v.x = i; v.y = y2;
                listaVertices.push_front(v);
            }

            // Segue para o pixel de cima
            y2++;
            continue;
        }

        break;
    }


    
    // Para o pixel clicado
    int leftLimClick = x1, rightLimClick = x1;

    // Procura pelo limite esquerdo da linha
    while (leftLimClick >= 0)
    {
        glReadPixels(leftLimClick, y1, 1, 1, GL_RGB, GL_INT, corPixelArea);
        if (confereCor(corPixelArea, corPixelClick) == true)
        {
            leftLimClick--;
            continue;
        }

        break;
    }

    // Procura pelo limite direito da linha
    while (rightLimClick <= width)
    {
        glReadPixels(rightLimClick, y1, 1, 1, GL_RGB, GL_INT, corPixelArea);
        if (confereCor(corPixelArea, corPixelClick) == true)
        {
            rightLimClick++;
            continue;
        }

        break;
    }

    // Desenha a linha do pixel clicado
    for (int i = leftLimClick+1; i < rightLimClick; i++)
    {
        vertice v;
        v.x = i; v.y = y1;
        listaVertices.push_front(v);
    }

    

    // Para cada pixel abaixo do pixel clicado
    int y0 = y1 - 1;
    while (y0 >= 0)
    {
        int leftLimInf = x1, rightLimInf = x1;
        glReadPixels(x1, y0, 1, 1, GL_RGB, GL_INT, corPixelArea);

        if (confereCor(corPixelArea, corPixelClick) == true)
        {
            // Procura pelo limite esquerdo da linha
            while (leftLimInf >= 0)
            {
                glReadPixels(leftLimInf, y0, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    leftLimInf--;
                    continue;
                }

                break;
            }

            // Procura pelo limite direito da linha
            while (rightLimInf <= width)
            {
                glReadPixels(rightLimInf, y0, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    rightLimInf++;
                    continue;
                }

                break;
            }

            // Desenha a reta do pixel abaixo
            vertice v;
            for (int i = leftLimInf+1; i < rightLimInf; i++)
            {
                v.x = i; v.y = y0;
                listaVertices.push_front(v);
            }

            // Segue para o pixel de baixo
            y0--;
            continue;
        }

        break;
    }

    return listaVertices;
}








#endif