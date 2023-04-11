/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pelos algoritmos usados no editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data de apresentacao: 28/02/2023
 * Ultima modificacao:   11/04/2023
*/

#ifndef algoritmos_h
#define algoritmos_h


/*
 * Funcao para desenhar apenas um pixel na tela
 */
void drawPixel(int x, int y, int modo)
{
    // Restringe o desenho de pontos para a area de desenho
    // (modo 1 = permissao para desenhar na area da GUI, 0 = sem permissao)
    if ((modo == 1) || (modo == 0 && y <= height - 50))
    {
        glBegin(GL_POINTS);     // Seleciona a primitiva GL_POINTS para desenhar
            glVertex2i(x, y);
        glEnd();                // Indica o fim do ponto
    }
}


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
 * Funcao que implementa o Algoritmo de Preenchimento de Poligonos
 * para o desenho de poligonos coloridos
*/
forward_list<vertice> preenchePoligono(double x[], double y[], int tamanhoVetor)
{
    // Lista de vertices a serem desenhados
    forward_list<vertice> listaVertices;


    // Descobre a quantidade de arestas a serem analisadas no poligono
    int contArestas = 0;
    for (int i = 0; i < tamanhoVetor; i++)
    {
        double x0, y0, x1, y1;

        if (i != tamanhoVetor-1)
        {
            x0 = x[i];
            y0 = y[i];
            x1 = x[i+1];
            y1 = y[i+1];
        }
        else
        {
            x0 = x[i];
            y0 = y[i];
            x1 = x[0];
            y1 = y[0];
        }

        // Verifica se os pontos formam uma aresta horizontal
        // Se sim, nao conta a aresta
        if (x0 != x1 && y0 == y1)
        {
            continue;
        }
        else
        {
            contArestas++;
        }
    }


    // Listas com os vertices das arestas validas
    double coordsX[2*contArestas], coordsY[2*contArestas];
    int k = 0;
    for (int i = 0; i < tamanhoVetor; i++)
    {
        double x0, y0, x1, y1;

        if (i != tamanhoVetor-1)
        {
            x0 = x[i];
            y0 = y[i];
            x1 = x[i+1];
            y1 = y[i+1];
        }
        else
        {
            x0 = x[i];
            y0 = y[i];
            x1 = x[0];
            y1 = y[0];
        }


        if (x0 != x1 && y0 == y1)
        {
            continue;
        }
        else
        {
            coordsX[k]   = x0;
            coordsY[k]   = y0;
            coordsX[k+1] = x1;
            coordsY[k+1] = y1;
            k += 2;
        }
    }




    // Cria a Tabela de Arestas
    // Cada linha eh uma aresta com os dados: [yMin | yMax | x_yMin | mInvert]
    double ta[contArestas][4];

    int pos = 0;
    double menorY = 1.79769e+308, maiorY = 0.0;
    for (int i = 0; i < 2*contArestas; i += 2)
    {
        double x0 = coordsX[i];
        double y0 = coordsY[i];
        double x1 = coordsX[i+1];
        double y1 = coordsY[i+1];

        // Calcula as variaveis de cada aresta
        double yMin, yMax, x_yMin, mInvert;
        if (y1 <= y0)
        {
            yMin = y1;
            yMax = y0;
            x_yMin = x1;
        }
        else
        {
            yMin = y0;
            yMax = y1;
            x_yMin = x0;
        }

        
        // Encontra mInvert, evitando a divisao 0/0
        if (x1 != x0 && y1 != y0)
        {
            mInvert = (x1-x0) / (y1-y0);
        }
        else
        {
            mInvert = 0.0;
        }


        // Salva o menor e o maior Y dentre todos os vertices
        // (usados futuramente como intervalo no algoritmo da TAA)
        if (yMin < menorY)
        {
            menorY = yMin;
        }

        if (yMax > maiorY)
        {
            maiorY = yMax;
        }


        // Insere na tabela
        ta[pos][0] = yMin;
        ta[pos][1] = yMax;
        ta[pos][2] = x_yMin;
        ta[pos][3] = mInvert;
        pos++;
    }



    // Ordenacoes pra garantir a seguinte leitura 
    /*               
        (      ^
         \    /
          `__´
    */
    // dos vertices das arestas na TA


    // Ordena a TA com base nos yMin (InsertionSort)
    for (int i = 1; i < contArestas; i++)
    {
        double yMin = ta[i][0], yMax = ta[i][1], x_yMin = ta[i][2], mInvert = ta[i][3];
        int j = i-1;

        while (j >= 0 && yMin < ta[j][0])
        {
            ta[j+1][0] = ta[j][0];
            ta[j+1][1] = ta[j][1];
            ta[j+1][2] = ta[j][2];
            ta[j+1][3] = ta[j][3];
            j--;
        }

        ta[j+1][0] = yMin;
        ta[j+1][1] = yMax;
        ta[j+1][2] = x_yMin;
        ta[j+1][3] = mInvert;
    }


    // Ordena a TA com base nos yMax de mesmo yMin (InsertionSort)
    int pos_yMin = 0;
    double yMinAtual = menorY;
    for (int i = 0; i < contArestas; i++)
    {
        if (ta[i][0] != yMinAtual)
        {
            for (int j = pos_yMin+1; j < i; j++)
            {
                double yMin = ta[j][0], yMax = ta[j][1], x_yMin = ta[j][2], mInvert = ta[j][3];
                int k = j-1;

                while (k >= pos_yMin && yMax < ta[k][1])
                {
                    ta[k+1][0] = ta[k][0];
                    ta[k+1][1] = ta[k][1];
                    ta[k+1][2] = ta[k][2];
                    ta[k+1][3] = ta[k][3];
                    k--;
                }

                ta[k+1][0] = yMin;
                ta[k+1][1] = yMax;
                ta[k+1][2] = x_yMin;
                ta[k+1][3] = mInvert;
            }

            yMinAtual = ta[i][0];
            pos_yMin = i;
        }

        else if (i == contArestas-1)
        {
            for (int j = pos_yMin+1; j <= i; j++)
            {
                double yMin = ta[j][0], yMax = ta[j][1], x_yMin = ta[j][2], mInvert = ta[j][3];
                int k = j-1;

                while (k >= pos_yMin && yMax < ta[k][1])
                {
                    ta[k+1][0] = ta[k][0];
                    ta[k+1][1] = ta[k][1];
                    ta[k+1][2] = ta[k][2];
                    ta[k+1][3] = ta[k][3];
                    k--;
                }

                ta[k+1][0] = yMin;
                ta[k+1][1] = yMax;
                ta[k+1][2] = x_yMin;
                ta[k+1][3] = mInvert;
            }
        }
    }




    // Cria a Tabela de Arestas Ativas
    double taa[contArestas][4];
    for (int i = 0; i < contArestas; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            taa[i][j] = -1;
        }
    }

    // Registra a posicao da maior linha da TAA com uma aresta carregada
    int indexTAA = -1;




    // Algoritmo de Rasterizacao
    for (int i = menorY; i < maiorY; i++)
    {

        // Atualiza o indexTAA
        for (int j = 0; j < contArestas; j++)
        {
            if (taa[j][0] == -1 && taa[j][1] == -1 && taa[j][2] == -1 && taa[j][3] == -1)
            {
                indexTAA = j;
                break;
            }
            else if (j == contArestas-1)
            {
                indexTAA = j;
            }
        }

        
        // Carrega as arestas da TA na TAA a cada y
        for (int j = 0; j < contArestas; j++)
        {
            if (ta[j][0] == i)
            {
                taa[indexTAA][0] = ta[j][0];
                taa[indexTAA][1] = ta[j][1];
                taa[indexTAA][2] = ta[j][2];
                taa[indexTAA][3] = ta[j][3];

                ta[j][0] = -1;
                ta[j][1] = -1;
                ta[j][2] = -1;
                ta[j][3] = -1;

                indexTAA++;
            }
        }


        // Indica TAA vazia (fim do algoritmo)
        if (indexTAA == 0)
        {
            break;
        }


        // Ordena a TAA pelo x_yMin (InsertionSort)
        for (int j = 1; j < indexTAA; j++)
        {
            double yMin0 = taa[j][0], yMax0 = taa[j][1], x_yMin0 = taa[j][2], mInvert0 = taa[j][3];
            double yMin1 = taa[j+1][0], yMax1 = taa[j+1][1], x_yMin1 = taa[j+1][2], mInvert1 = taa[j+1][3];
            int k = j-1;

            while (k >= 0 && x_yMin0 < taa[k][2])
            {
                taa[k+1][0] = taa[k][0];
                taa[k+1][1] = taa[k][1];
                taa[k+1][2] = taa[k][2];
                taa[k+1][3] = taa[k][3];
                k--;
            }

            taa[k+1][0] = yMin0;
            taa[k+1][1] = yMax0;
            taa[k+1][2] = x_yMin0;
            taa[k+1][3] = mInvert0;
        }


        // Adiciona os vertices da scanline na tabela de vertices
        for (int j = 0; j < indexTAA; j += 2)
        {
            int span0, span1;

            // Verifica se necessita truncar pra cima o primeiro span
            // (casting sozinho trunca pra baixo)
            if (taa[j][2] != (int) taa[j][2])
            {
                span0 = (int) taa[j][2] + 1;
            }
            else
            {
                span0 = (int) taa[j][2];
            }

            // Trunca pra baixo o segundo span
            span1 = (int) taa[j+1][2];


            // Adiciona o span de vertices na lista
            for (int k = span0; k <= span1; k++)
            {
                vertice v;
                v.x = k; v.y = i;
                listaVertices.push_front(v);
            }
        }


        // Remove da TAA todas as arestas onde y = yMax-1
        int quantRemovidos = 0;
        for (int j = 0; j < indexTAA; j++)
        {
            if (i+1 == taa[j][1])
            {
                for (int k = j; k < indexTAA; k++)
                {
                    taa[k][0] = taa[k+1][0];
                    taa[k][1] = taa[k+1][1];
                    taa[k][2] = taa[k+1][2];
                    taa[k][3] = taa[k+1][3];
                }
                j--;
                indexTAA--;
            }
        }


        // Incrementa os x_yMin de cada aresta da TAA
        // (para cada aresta na TAA, incrementa seu x_yMin com seu mInvert)
        for (int j = 0; j < indexTAA; j++)
        {
            taa[j][2] += taa[j][3];
        }


        // Reordena a TAA pelo x_yMin (necessario p/ poligonos intersectantes) (InsertionSort)
        for (int j = 1; j < indexTAA; j++)
        {
            double yMin0 = taa[j][0], yMax0 = taa[j][1], x_yMin0 = taa[j][2], mInvert0 = taa[j][3];
            double yMin1 = taa[j+1][0], yMax1 = taa[j+1][1], x_yMin1 = taa[j+1][2], mInvert1 = taa[j+1][3];
            int k = j-1;

            while (k >= 0 && x_yMin0 < taa[k][2])
            {
                taa[k+1][0] = taa[k][0];
                taa[k+1][1] = taa[k][1];
                taa[k+1][2] = taa[k][2];
                taa[k+1][3] = taa[k][3];
                k--;
            }

            taa[k+1][0] = yMin0;
            taa[k+1][1] = yMax0;
            taa[k+1][2] = x_yMin0;
            taa[k+1][3] = mInvert0;
        }
    }

    return listaVertices;
}




/*
 * Funcao que implementa o Algoritmo Flood Fill para preenchimento de poligonos
*/
void algoritmoFloodFill(int x1, int y1, float *corAntiga, float *corNova, forward_list<vertice> *vertices)
{
    // Verifica se este pixel ja foi analisado pelo algoritmo
    bool jaAnalisado = false;
    for (forward_list<vertice>::iterator it_v = vertices->begin(); it_v != vertices->end(); it_v++)
    {
        if (it_v->x == x1 && it_v->y == y1)
        {
            jaAnalisado = true;
            break;
        }
    }


    // Se nao tenha sido, faz a verificacao
    if (jaAnalisado == false)
    {
        // Guarda a cor do pixel analisado
        float corPixelAtual[3];
        glReadPixels(x1, y1, 1, 1, GL_RGB, GL_FLOAT, corPixelAtual);

        // Caso o pixel seja da cor antiga
        if (corPixelAtual[0] == corAntiga[0] &&
            corPixelAtual[1] == corAntiga[1] &&
            corPixelAtual[2] == corAntiga[2])
        {
            // Pinta o pixel com a nova cor
            glColor3f(corNova[0], corNova[1], corNova[2]);
            drawPixel(x1, y1, 0);


            // Salva o vertice na lista de vertices ja analisados
            vertice v;
            v.x = x1;
            v.y = y1;
            vertices->push_front(v);


            // Chamadas recursivas pros seus vizinhos cardeais (FloodFill com vizinhanca 4)
            algoritmoFloodFill(x1+1, y1, corAntiga, corNova, vertices);
            algoritmoFloodFill(x1, y1+1, corAntiga, corNova, vertices);
            algoritmoFloodFill(x1-1, y1, corAntiga, corNova, vertices);
            algoritmoFloodFill(x1, y1-1, corAntiga, corNova, vertices);
        }
    }
}


#endif
