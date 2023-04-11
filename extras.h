/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel por conteudos extras do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data de apresentacao: 28/02/2023
 * Ultima modificacao:   11/04/2023
*/

#ifndef extras_h
#define extras_h


/*
 * Funcao responsavel pelo desenho dos vertices (gerados por algoritmos) no header extras.h
 * (possui permissao de desenhar na area da GUI)
*/
void desenhaVertices(forward_list<vertice> v)
{
    for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++)
    {
        drawPixel(it_v->x, it_v->y, 1);
    }
}




/*
 * Funcao responsavel pelo desenho da GUI da janela
*/
void desenhaGUI(bool nightMode)
{
    forward_list<vertice> v;


//  1) Area GUI (azul)
    if (nightMode == false)
    {
        glColor3f(0.11, 0.43, 0.11);
    }
    else
    {
        glColor3f(0.0, 0.0, 1.0);
    }
    for (int k = 0; k <= width; k++)
    {
        for (int l = 0; l <= 49; l++)
        {
            drawPixel(k, height-50+l, 1);
        }
    }



//  2) Barras de cor RGB

        // Contador para definicao de cor
        float cont;

        // Posicao da cor clicada dentro da barra
        int brancoX;

        // Desenho das barras
        for (int i = 0; i < 3; i++)
        {
            // Cor de cada caixote
            if (nightMode == false)
            {
                glColor3f(0.0, 1.0, 0.0);   // (verde)
            }
            else
            {
                glColor3f(0.0, 0.0, 0.0);   // (preto)
            }

            // Desenha o caixote
            for (int k = 0; k <= 82; k++)
            {
                drawPixel(width-91+k, height-16-(15*i), 1);
                drawPixel(width-91+k, height-4-(15*i) , 1);
            }
            for (int k = 0; k <= 12; k++)
            {
                drawPixel(width-91, height-16-(15*i)+k, 1);
                drawPixel(width-9 , height-16-(15*i)+k, 1);
            }


            // Zera as variaveis
            cont = 0.0; brancoX = width-88;
            
            // Desenha o degrade em cada barra
            for (int j = width-90; j <= width-10; j++)
            {
                // Encontra a posicao da linha branca (cor escolhida) em cada barra
                switch (i)
                {
                    case 0: // Coordenada x da linha branca na barra R
                            if ((0.0 + cont) < rSelec)
                            {
                                brancoX = j;
                            }

                            // Espectro preto -> vermelho
                            glColor3f(0.0 + cont, 0.0, 0.0);
                    break;

                    case 1: // Coordenada x da linha branca na barra G
                            if ((0.0 + cont) < gSelec)
                            {
                                brancoX = j;
                            }

                            // Espectro preto -> verde
                            glColor3f(0.0, 0.0 + cont, 0.0);
                    break;

                    case 2: // Coordenada x da linha branca na barra B
                            if ((0.0 + cont) < bSelec)
                            {
                                brancoX = j;
                            }

                            // Espectro preto -> azul
                            glColor3f(0.0, 0.0, 0.0 + cont);
                    break;
                }

                // Desenha progressivamente as retas coloridas da barra
                for (int k = 0; k <= 10; k++)
                {
                    drawPixel(j, height-15-(15*i)+k, 1);
                }

                cont += 0.0125;
            }

            // Desenha a linha vertical branca de cada barra
            // (representa o pigmento escolhido de cada barra de cor)
            glColor3f(1.0, 1.0, 1.0);
            for (int k = 0; k <= 10; k++)
            {
                drawPixel(brancoX, height-15-(15*i)+k, 1);
            }
        }


        // Quadrado com a cor escolhida para visualizacao

            // Cor do quadrado
            if (nightMode == false)
            {
                glColor3f(0.0, 0.0, 0.0);   // (preto)
            }
            else
            {
                glColor3f(1.0, 1.0, 1.0);   // (branco)
            }

            // Desenha o quadrado
            for (int k = 0; k <= 27; k++)
            {
                drawPixel(width-131+k, height-36  , 1);
                drawPixel(width-104  , height-36+k, 1);
                drawPixel(width-131+k, height-9   , 1);
                drawPixel(width-131  , height-36+k, 1);
            }
        
            // Colore o quadrado com a cor selecionada
            glColor3f(rSelec, gSelec, bSelec);
            for (int i = 0; i <= 25; i++)
            {
                for (int j = 0; j <= 25; j++)
                {
                    drawPixel(width-130+j, height-35+i, 1);
                }
            }




//  3) Botoes

        // Barra inferior

            // Cor do retangulo (cinza-claro)
            glColor3f(0.57, 0.58, 0.58);

            // Desenho do retangulo
            for (int k = 0; k <= 140; k++)
            {
                drawPixel(70+k , height-45, 1);
                drawPixel(210-k, height-26, 1);
            }
            for (int k = 0; k <= 19; k++)
            {
                drawPixel(70 , height-45+k, 1);
                drawPixel(210, height-45+k, 1);
            }

            // Colore o retangulo
            for (int i = 0; i <= 139; i++)
            {
                for (int j = 0; j <= 18; j++)
                {
                    drawPixel(71+i, height-44+j, 1);
                }
            }


            // Bordas de cada botao
            int qntBotoesInf = 7;
            for (int i = 0; i < 20*qntBotoesInf; i += 20)
            {
                // Cor do borda
                if (i / 20 == modoForma)
                {
                    // Botao selecionado (vermelho)
                    glColor3f(1.0, 0.0, 0.0);
                }
                else
                {
                    // Botoes nao selecionados (branco)
                    if (nightMode == false)
                    {
                        glColor3f(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glColor3f(1.0, 1.0, 1.0);
                    }
                }

                // Desenha a borda
                for (int k = 0; k <= 18; k++)
                {
                    drawPixel(71+i+k, height-44, 1);
                    drawPixel(89+i-k, height-27, 1);
                }
                for (int k = 0; k <= 17; k++)
                {
                    drawPixel(71+i, height-44+k, 1);
                    drawPixel(89+i, height-44+k, 1);
                }
            }


        // Barra superior

            // Cor do retangulo (cinza-claro)
            glColor3f(0.57, 0.58, 0.58);

            // Desenho do retangulo
            for (int k = 0; k <= 60; k++)
            {
                drawPixel(110+k, height-25, 1);
                drawPixel(170-k, height-6 , 1);
            }
            for (int k = 0; k <= 19; k++)
            {
                drawPixel(110, height-25+k, 1);
                drawPixel(170, height-25+k, 1);
            }

            // Colore o retangulo
            for (int i = 0; i <= 58; i++)
            {
                for (int j = 0; j <= 17; j++)
                {
                    drawPixel(111+i, height-24+j, 1);
                }
            }


            // Bordas de cada botao
            int qntBotoesSup = 3;
            for (int i = 0; i < 20*qntBotoesSup; i += 20)
            {
                // Cor da borda
                if ((modoForma >= 7) && ((i+40) / 20 == modoForma-5))
                {
                    // Botao selecionado (vermelho)
                    glColor3f(1.0, 0.0, 0.0);
                }
                else
                {
                    // Botoes nao selecionados (branco)
                    if (nightMode == false)
                    {
                        glColor3f(0.0, 0.0, 0.0);
                    }
                    else
                    {
                        glColor3f(1.0, 1.0, 1.0);
                    }
                }

                // Desenha a borda
                for (int k = 0; k <= 18; k++)
                {
                    drawPixel(111+i+k, height-24, 1);
                    drawPixel(129+i-k, height-7 , 1);
                }
                for (int k = 0; k <= 17; k++)
                {
                    drawPixel(111+i, height-24+k, 1);
                    drawPixel(129+i, height-24+k, 1);
                }
            }


        // Colunas das transformacoes

            // Visiveis apenas para o modo MOU
            if (modoForma == 0)
            {
                int qntBotoesTransf = 5;
                for (int i = 0; i < 2; i++)
                {
                    for (int j = 0; j <= (qntBotoesTransf-i) % 3; j++)
                    {
                        // Cor do botao (cinza-claro)
                        glColor3f(0.57, 0.58, 0.58);

                        // Desenha cada botao
                        for (int k = 0; k <= 15; k++)
                        {
                            drawPixel(250+(20*i)+k, height-17-(15*j)  , 1);
                            drawPixel(265+(20*i)  , height-17-(15*j)+k, 1);
                            drawPixel(265+(20*i)-k, height-2-(15*j)   , 1);
                            drawPixel(250+(20*i)  , height-2-(15*j)-k , 1);
                        }

                        // Colore cada botao
                        for (int k = 0; k <= 13; k++)
                        {
                            for (int l = 0; l <= 15; l++)
                            {
                                drawPixel(251+(20*i)+k, height-17-(15*j)+l, 1);
                            }
                        }
                        

                        // Bordas de cada botao

                            // Cor da borda
                            if ((i == (modoTransf-10) / 3) && (j == (modoTransf-10) % 3))
                            {
                                // Botao selecionado (vermelho)
                                glColor3f(1.0, 0.0, 0.0);
                            }
                            else
                            {
                                // Botoes nao selecionados (branco)
                                if (nightMode == false)
                                {
                                    glColor3f(0.0, 0.0, 0.0);
                                }
                                else
                                {
                                    glColor3f(1.0, 1.0, 1.0);
                                }
                            }

                            // Desenha a borda
                            for (int k = 0; k <= 13; k++)
                            {
                                for (int l = 0; l <= 13; l++)
                                {
                                    drawPixel(251+(20*i)+k, height-16-(15*j)  , 1);
                                    drawPixel(264+(20*i)  , height-16-(15*j)+l, 1);
                                    drawPixel(264+(20*i)-k, height-3-(15*j)   , 1);
                                    drawPixel(251+(20*i)  , height-3-(15*j)-l , 1);
                                }
                            }
                    }
                }
            }


        // Botao de claridade

            // Cor da borda do retangulo (cinza-claro)
            glColor3f(0.57, 0.58, 0.58);

            // Desenho do retangulo
            for (int k = 0; k <= 25; k++)
            {
                drawPixel(0+k , height-25, 1);
                drawPixel(25-k, height-1 , 1);
            }

            for (int k = 0; k <= 24; k++)
            {
                drawPixel(0 , height-25+k, 1);
                drawPixel(25, height-25+k, 1);
            }

            // Colore o retangulo
            cont = 0.0;
            if (nightMode == false)
            {
                for (int j = 0; j <= 22; j++)
                {
                    // Cor do ceu no anoitecer
                    glColor3f(0.02, 0.02, 0.15+cont);

                    for (int i = 0; i <= 23; i++)
                    {
                        drawPixel(1+i, height-24+j, 1);
                    }

                    cont += 0.0138;
                }
            }
            else
            {
                for (int j = 0; j <= 22; j++)
                {
                    // Cor do ceu no nascer do dia
                    glColor3f(0.0, 0.30+cont, 1.0);

                    for (int i = 0; i <= 23; i++)
                    {
                        drawPixel(1+i, height-24+j, 1);
                    }

                    cont += 0.0136;
                }
            }


        // Botao de reset

            // Cor da borda do retangulo (cinza-claro)
            glColor3f(0.57, 0.58, 0.58);

            // Desenho do retangulo
            for (int k = 0; k <= 25; k++)
            {
                drawPixel(25+k, height-25, 1);
                drawPixel(50-k, height-1 , 1);
            }

            for (int k = 0; k <= 24; k++)
            {
                drawPixel(25, height-25+k, 1);
                drawPixel(50, height-25+k, 1);
            }

            // Colore o retangulo
            cont = 0.0;
            for (int j = 0; j <= 22; j++)
            {
                // Cor do ceu no anoitecer
                glColor3f(0.02, 0.02, 0.15+cont);

                for (int i = 0; i <= 23; i++)
                {
                    drawPixel(26+i, height-24+j, 1);
                }

                cont += 0.0138;
            }


        // Botao de salvar

            // Cor da borda do retangulo (cinza-claro)
            glColor3f(0.57, 0.58, 0.58);

            // Desenho do retangulo
            for (int k = 0; k <= 25; k++)
            {
                drawPixel(0+k , height-50, 1);
                drawPixel(25-k, height-25, 1);
            }

            for (int k = 0; k <= 24; k++)
            {
                drawPixel(0 , height-50+k, 1);
                drawPixel(25, height-50+k, 1);
            }

            // Colore o retangulo
            cont = 0.0;
            for (int j = 0; j <= 23; j++)
            {
                // Cor do ceu no por do sol
                glColor3f(0.9, 0.30+cont, 0.0);

                for (int i = 0; i <= 23; i++)
                {
                    drawPixel(1+i, height-49+j, 1);
                }

                cont += 0.0136;
            }




//  4) Icones

        // Cor dos icones (preto)
        glColor3f(0.0, 0.0, 0.0);

        // Mouse
        for (int k = 0; k <= 4; k++)
        {
            drawPixel(81+k, height-43, 1);
        }

        v = retaBresenham(85, height-43, 83, height-38); desenhaVertices(v);

        for (int k = 0; k <= 4; k++)
        {
            drawPixel(83+k, height-38, 1);
        }

        v = retaBresenham(87, height-37, 74, height-28); desenhaVertices(v);

        for (int k = 0; k <= 14; k++)
        {
            drawPixel(74, height-28-k, 1);
        }

        v = retaBresenham(74, height-42, 79, height-39); desenhaVertices(v);
        v = retaBresenham(79, height-39, 81, height-43); desenhaVertices(v);



        // Reta
        v = retaBresenham(92, height-43, 107, height-28); desenhaVertices(v);



        // Retangulo

            // Barra inferior
            for (int k = 0; k <= 13; k++)
            {
                drawPixel(113+k, height-41, 1);
                drawPixel(127-k, height-30, 1);
            }
            for (int k = 0; k <= 11; k++)
            {
                drawPixel(113, height-41+k, 1);
                drawPixel(127, height-30-k, 1);
            }
            

            // Barra superior
            for (int k = 0; k <= 14; k++)
            {
                for (int l = 0; l <= 11; l++)
                {
                    drawPixel(113+k, height-21+l, 1);
                }
            }



        // Triangulo

            // Barra inferior
            for (int k = 0; k <= 12; k++)
            {
                drawPixel(134+k, height-41, 1);
            }

            v = retaBresenham(146, height-41, 140, height-29); desenhaVertices(v);
            v = retaBresenham(140, height-29, 134, height-41); desenhaVertices(v);


            // Barra superior
            for (int k = 0; k <= 12; k++)
            {
                v = retaBresenham(134+k, height-21, 140, height-9); desenhaVertices(v);
            }



        // Poligono

            // Barra inferior
            for (int k = 0; k <= 12; k++)
            {
                drawPixel(154+k, height-41, 1);
            }

            v = retaBresenham(166, height-41, 160, height-36); desenhaVertices(v);
            v = retaBresenham(160, height-36, 166, height-29); desenhaVertices(v);
            
            for (int k = 0; k <= 12; k++)
            {
                drawPixel(166-k, height-29, 1);
            }
            for (int k = 0; k <= 12; k++)
            {
                drawPixel(154, height-29-k, 1);
            }


            // Barra superior
            for (int l = 0; l <= 6; l++)
            {
                for (int k = 0; k <= 12-l; k++)
                {
                    drawPixel(154+k, height-21+l, 1);
                }
            }

            for (int l = 0; l <= 6; l++)
            {
                for (int k = 0; k <= 6+l; k++)
                {
                    drawPixel(154+k, height-15+l, 1);
                }
            }



        // Circunferencia
        v = circuloBresenham(180, height-36, 185, 7); desenhaVertices(v);



        // Balde

            // Lata
            for (int k = 0; k <= 10; k++)
            {
                drawPixel(195+k, height-42, 1);
                drawPixel(205-k, height-33, 1);
            }
            for (int k = 0; k <= 9; k++)
            {
                drawPixel(195, height-42+k, 1);
                drawPixel(205, height-33-k, 1);
            }


            // Alca
            for (int k = 0; k <= 3; k++)
            {
                drawPixel(196+k, height-32+k, 1);
                drawPixel(204-k, height-32+k, 1);
            }
            drawPixel(200, height-29, 1);


            // Tinta
            for (int l = 0; l <= 2; l++)
            {
                for (int k = 0; k <= 8-l; k++)
                {
                    drawPixel(197+k, height-32-l, 1);
                }
            }

            for (int k = 0; k <= 2; k++)
            {
                drawPixel(198, height-34-k, 1);
            }

            for (int k = 0; k <= 1; k++)
            {
                drawPixel(199, height-34-k, 1);
            }

            for (int k = 0; k <= 4; k++)
            {
                drawPixel(202, height-34-k, 1);
            }

            for (int k = 0; k <= 3; k++)
            {
                drawPixel(203, height-34-k, 1);
            }



        // Transformacoes geometricas
        if (modoForma == MOU)
        {
            // Translacao
            drawPixel(253, height-14, 1);
            drawPixel(254, height-14, 1);
            drawPixel(256, height-14, 1);
            drawPixel(257, height-14, 1);
            drawPixel(257, height-13, 1);
            drawPixel(257, height-11, 1);
            drawPixel(257, height-10, 1);
            drawPixel(257, height-8 , 1);
            drawPixel(257, height-7 , 1);
            drawPixel(257, height-6 , 1);
            drawPixel(256, height-6 , 1);
            drawPixel(254, height-6 , 1);
            drawPixel(253, height-6 , 1);
            drawPixel(253, height-7 , 1);
            drawPixel(253, height-9 , 1);
            drawPixel(253, height-10, 1);
            drawPixel(253, height-12, 1);
            drawPixel(253, height-13, 1);

            for (int k = 0; k <= 3; k++)
            {
                drawPixel(259+k, height-14, 1);
                drawPixel(262-k, height-6 , 1);
            }

            for (int k = 0; k <= 8; k++)
            {
                drawPixel(259, height-14+k, 1);
                drawPixel(262, height-6-k , 1);
            }



            // Escala
            drawPixel(253, height-29, 1);
            drawPixel(254, height-29, 1);
            drawPixel(256, height-29, 1);
            drawPixel(257, height-29, 1);
            drawPixel(257, height-28, 1);
            drawPixel(257, height-27, 1);
            
            drawPixel(257, height-25, 1);
            drawPixel(257, height-24, 1);
            drawPixel(256, height-24, 1);
            drawPixel(254, height-24, 1);
            drawPixel(253, height-24, 1);
            drawPixel(253, height-25, 1);
            drawPixel(253, height-27, 1);
            drawPixel(253, height-28, 1);

            for (int k = 0; k <= 3; k++)
            {
                drawPixel(259+k, height-29, 1);
                drawPixel(262-k, height-21, 1);
            }

            for (int k = 0; k <= 8; k++)
            {
                drawPixel(259, height-29+k, 1);
                drawPixel(262, height-21-k , 1);
            }



            // Cisalhamento
            drawPixel(253, height-44, 1);
            drawPixel(254, height-44, 1);
            drawPixel(256, height-44, 1);
            drawPixel(257, height-44, 1);
            drawPixel(257, height-43, 1);
            drawPixel(257, height-41, 1);
            drawPixel(257, height-40, 1);
            drawPixel(257, height-38, 1);
            drawPixel(257, height-37, 1);
            drawPixel(257, height-36, 1);
            drawPixel(256, height-36, 1);
            drawPixel(254, height-36, 1);
            drawPixel(253, height-36, 1);
            drawPixel(253, height-37, 1);
            drawPixel(253, height-39, 1);
            drawPixel(253, height-40, 1);
            drawPixel(253, height-42, 1);
            drawPixel(253, height-43, 1);

            for (int k = 0; k <= 3; k++)
            {
                drawPixel(259+k, height-44, 1);
            }

            v = retaBresenham(259, height-44, 260, height-36); desenhaVertices(v);
            v = retaBresenham(262, height-44, 263, height-36); desenhaVertices(v);

            for (int k = 0; k <= 2; k++)
            {
                drawPixel(263-k, height-36, 1);
            }



            // Reflexao
            drawPixel(273, height-14, 1);

            v = retaBresenham(273, height-14, 277, height-13); desenhaVertices(v);
            drawPixel(277, height-11, 1);
            drawPixel(277, height-10, 1);
            drawPixel(277, height-8 , 1);
            drawPixel(277, height-7 , 1);
            drawPixel(277, height-6 , 1);
            drawPixel(276, height-6 , 1);
            drawPixel(275, height-6 , 1);

            v = retaBresenham(275, height-6, 273, height-8); desenhaVertices(v);
            drawPixel(273, height-10, 1);
            drawPixel(273, height-12, 1);


            v = retaBresenham(279, height-13, 283, height-14); desenhaVertices(v);
            for (int k = 0; k <= 6; k++)
            {
                drawPixel(283, height-14+k, 1);
            }

            v = retaBresenham(283, height-8, 279, height-6); desenhaVertices(v);
            for (int k = 0; k <= 7; k++)
            {
                drawPixel(279, height-6-k, 1);
            }



            // Rotacao
            drawPixel(273, height-29, 1);
            drawPixel(274, height-29, 1);
            drawPixel(276, height-29, 1);
            drawPixel(277, height-29, 1);
            drawPixel(277, height-28, 1);
            drawPixel(277, height-26, 1);
            drawPixel(277, height-25, 1);
            drawPixel(277, height-23, 1);
            drawPixel(277, height-22, 1);
            drawPixel(277, height-21, 1);
            drawPixel(276, height-21, 1);
            drawPixel(274, height-21, 1);
            drawPixel(273, height-21, 1);
            drawPixel(273, height-22, 1);
            drawPixel(273, height-24, 1);
            drawPixel(273, height-25, 1);
            drawPixel(273, height-27, 1);
            drawPixel(273, height-28, 1);

            for (int k = 0; k <= 4; k++)
            {
                drawPixel(279+k, height-29, 1);
                drawPixel(282-k, height-25, 1);
            }
            
            for (int k = 0; k <= 4; k++)
            {
                drawPixel(279, height-29+k, 1);
                drawPixel(282, height-25-k, 1);
            }
        }



        // Cor e icones do nightmode
        if (nightMode == false)
        {
            glColor3f(1.0, 1.0, 1.0);   // (branco)

            // Lua
            for (int k = 0; k <= 6; k++)
            {
                v = circuloBresenham(13, height-13, 17, 7-k); desenhaVertices(v);
            }
        }
        else
        {
            glColor3f(1.0, 1.0, 0.0);   // (amarelo)

            // Sol
            for (int k = 0; k <= 6; k++)
            {
                v = circuloBresenham(13, height-13, 17, 7-k); desenhaVertices(v);
            }
            for (int k = 0; k <= 1; k++)
            {
                drawPixel(13, height-24+k, 1);
            }
            for (int k = 0; k <= 1; k++)
            {
                drawPixel(13, height-2-k, 1);
            }

            v = retaBresenham(6 , height-6 , 2 , height-3 ); desenhaVertices(v);
            v = retaBresenham(20, height-6 , 23, height-3 ); desenhaVertices(v);
            v = retaBresenham(20, height-20, 23, height-23); desenhaVertices(v);
            v = retaBresenham(6 , height-20, 2 , height-23); desenhaVertices(v);
        }


        // Borracha

            // Cor
            glColor3f(1.0, 1.0, 1.0);
            for (int j = 0; j <= 3; j++)
            {
                v = retaBresenham(30, height-17+j, 37, height-18+j); desenhaVertices(v);
            }
            glColor3f(0.25, 0.25, 0.25);
            for (int j = 0; j <= 2; j++)
            {
                v = retaBresenham(38, height-17+j, 47, height-11+j); desenhaVertices(v);
            }
            glColor3f(0.5, 0.5, 0.5);
            v = retaBresenham(33, height-12, 38, height-7); desenhaVertices(v);
            for (int j = 0; j <= 8; j++)
            {
                v = retaBresenham(34+j, height-12, 38+j, height-8); desenhaVertices(v);
            }
            v = retaBresenham(32, height-13, 39, height-13); desenhaVertices(v);
            v = retaBresenham(35, height-14, 38, height-14); desenhaVertices(v);
            v = retaBresenham(35, height-15, 38, height-15); desenhaVertices(v);


            // Contorno
            glColor3f(0.0, 0.0, 0.0);
            v = retaBresenham(29, height-13, 29, height-18); desenhaVertices(v);
            v = retaBresenham(29, height-18, 38, height-19); desenhaVertices(v);
            v = retaBresenham(38, height-18, 47, height-12); desenhaVertices(v);
            v = retaBresenham(47, height-12, 47, height-8 ); desenhaVertices(v);
            v = retaBresenham(47, height-8 , 38, height-7 ); desenhaVertices(v);
            v = retaBresenham(38, height-7 , 29, height-13); desenhaVertices(v);

            v = retaBresenham(29, height-13, 38, height-15); desenhaVertices(v);
            v = retaBresenham(38, height-15, 38, height-19); desenhaVertices(v);
            v = retaBresenham(38, height-15, 47, height-8 ); desenhaVertices(v);


        // Disquete

            // Caixa (preto)
            glColor3f(0.0, 0.0, 0.0);
            v = retaBresenham(4 , height-48, 21, height-48); desenhaVertices(v);
            v = retaBresenham(21, height-48, 21, height-29); desenhaVertices(v);
            v = retaBresenham(21, height-29, 19, height-27); desenhaVertices(v);
            v = retaBresenham(19, height-27,  4, height-27); desenhaVertices(v);
            v = retaBresenham( 4, height-27,  4, height-48); desenhaVertices(v);
            for (int j = 0; j < 20; j++)
            {
                v = retaBresenham(4, height-48+j, 21, height-48+j); desenhaVertices(v);
            }
            v = retaBresenham(4 , height-28, 20, height-28); desenhaVertices(v);
            v = retaBresenham(4 , height-27, 19, height-27); desenhaVertices(v);


            // Lamina de metal (cinza-claro)
            glColor3f(0.67, 0.67, 0.67);
            for (int j = 0; j < 6; j++)
            {
                v = retaBresenham(7, height-32+j, 18, height-32+j); desenhaVertices(v);
            }
            glColor3f(0.0, 0.0, 0.0);
            v = retaBresenham(16, height-32, 16, height-27); desenhaVertices(v);


            // Area de escrever (cinza)
            glColor3f(0.3, 0.3, 0.3);
            for (int j = 0; j < 11; j++)
            {
                v = retaBresenham(6, height-48+j, 19, height-48+j); desenhaVertices(v);
            }


    glutPostRedisplay();
}









/*
 * Funcao responsavel por todos os desenhos envolvendo textos no projeto
*/
void desenhaTextos()
{
    /* ====== Desenha o par de coordenadas na tela ====== */
    if (m_y < height - 50)
    {
        if (nightMode == false)
        {
            glColor3f(0.0, 0.0, 0.0);
        }
        else
        {
            glColor3f(1.0, 1.0, 1.0);
        }

        draw_text_stroke(0, 3, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.1);
    }



    /* ====== Mouse em um botao de forma ====== */
    double mouse_x = m_x, mouse_y = m_y;

        // Botao MOU
        if (m_x > 70 && m_x < 90)
        {
            if (m_y >= height-45 && m_y <= height-25 && modoForma != MOU)
            {
                double x[] = {mouse_x+8 , mouse_x+40, mouse_x+40, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Mouse", 0.08);
            }
        }

        // Botao LIN
        else if (m_x > 90 && m_x < 110)
        {
            if (m_y >= height-45 && m_y <= height-25 && modoForma != LIN)
            {
                double x[] = {mouse_x+8 , mouse_x+35, mouse_x+35, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Linha", 0.08);
            }
        }

        else if (m_x > 110 && m_x < 130)
        {
            // Botao RET
            if (m_y >= height-45 && m_y <= height-25 && modoForma != RET)
            {
                double x[] = {mouse_x+8 , mouse_x+56, mouse_x+56, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Retangulo", 0.08);
            }

            // Botao RETp
            else if (m_y >= height-25 && m_y <= height-5 && modoForma != RETp)
            {
                double x[] = {mouse_x+8 , mouse_x+116, mouse_x+116, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27 , mouse_y-15 , mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Retangulo Preenchido", 0.08);
            }
        }

        else if (m_x > 130 && m_x < 150)
        {
            // Botao TRI
            if (m_y >= height-45 && m_y <= height-25 && modoForma != TRI)
            {
                double x[] = {mouse_x+8 , mouse_x+53, mouse_x+53, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Triangulo", 0.08);
            }

            // Botao TRIp
            else if (m_y >= height-25 && m_y <= height-5 && modoForma != TRIp)
            {
                double x[] = {mouse_x+8 , mouse_x+112, mouse_x+112, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27 , mouse_y-15 , mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Triangulo Preenchido", 0.08);
            }
        }

        else if (m_x > 150 && m_x < 170)
        {
            // Botao POL
            if (m_y >= height-45 && m_y <= height-25 && modoForma != POL)
            {
                double x[] = {mouse_x+8 , mouse_x+50, mouse_x+50, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Poligono", 0.08);
            }

            // Botao POLp
            else if (m_y >= height-25 && m_y <= height-5 && modoForma != POLp)
            {
                double x[] = {mouse_x+8 , mouse_x+110, mouse_x+110, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27 , mouse_y-15 , mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Poligono Preenchido", 0.08);
            }
        }

        // Botao CIR
        else if (m_x > 170 && m_x < 190)
        {
            if (m_y >= height-45 && m_y <= height-25 && modoForma != CIR)
            {
                double x[] = {mouse_x+8 , mouse_x+78, mouse_x+78, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Circunferencia", 0.08);
            }
        }

        // Botao BAL
        else if (m_x > 190 && m_x < 210)
        {
            if (m_y >= height-45 && m_y <= height-25 && modoForma != BAL)
            {
                double x[] = {mouse_x+8 , mouse_x+35, mouse_x+35, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Balde", 0.08);
            }
        }




    /* ====== Mouse em um botao de transformacao ====== */
        else if (m_x > 250 && m_x < 265)
        {
            if (modoForma == MOU || modoForma > TRAN)
            {
                // Botao TRAN
                if (m_y >= height-17 && m_y <= height-2 && modoTransf != TRAN)
                {
                    double x[] = {mouse_x+8 , mouse_x+60, mouse_x+60, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Translacao", 0.08);
                }

                // Botao ESCA
                else if (m_y >= height-32 && m_y <= height-17 && modoTransf != ESCA)
                {
                    double x[] = {mouse_x+8 , mouse_x+40, mouse_x+40, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Escala", 0.08);
                }

                // Botao CISA
                else if (m_y >= height-47 && m_y <= height-32 && modoTransf != CISA)
                {
                    double x[] = {mouse_x+8 , mouse_x+72, mouse_x+72, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Cisalhamento", 0.08);
                }
            }
        }

        else if (m_x > 270 && m_x < 285)
        {
            if (modoForma == MOU || modoForma > TRAN)
            {
                // Botao REFL
                if (m_y >= height-17 && m_y <= height-2 && modoTransf != REFL)
                {
                    double x[] = {mouse_x+8 , mouse_x+48, mouse_x+48, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Reflexao", 0.08);
                }

                // Botao ROTA
                else if (m_y >= height-32 && m_y <= height-17 && modoTransf != ROTA)
                {
                    double x[] = {mouse_x+8 , mouse_x+48, mouse_x+48, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Rotacao", 0.08);
                }
            }
        }




    /* ====== Mouse em uma barra RGB ====== */
        else if (m_x >= width-90 && m_x <= width-10)
        {
            // Barra R
            if (m_y > height-15 && m_y < height-5)
            {
                float corEscolhida[3];
                glReadPixels(m_x, m_y, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);
                
                double x[] = {mouse_x+8 , mouse_x+32, mouse_x+32, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, to_string(corEscolhida[0]).substr(0,4), 0.08);
            }

            // Barra G
            else if (m_y > height-30 && m_y < height-20)
            {
                float corEscolhida[3];
                glReadPixels(m_x, m_y, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);
                
                double x[] = {mouse_x+8 , mouse_x+32, mouse_x+32, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, to_string(corEscolhida[1]).substr(0,4), 0.08);
            }

            // Barra B
            else if (m_y > height-45 && m_y < height-35)
            {
                float corEscolhida[3];
                glReadPixels(m_x, m_y, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);
                
                double x[] = {mouse_x+8 , mouse_x+32, mouse_x+32, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, to_string(corEscolhida[2]).substr(0,4), 0.08);
            }
        }




    /* ====== Mouse no quadrado de cor ====== */
        else if (m_y > height-36 && m_y < height-9)
        {
            if (m_x > width-131 && m_x < width-104)
            {
                double x[] = {mouse_x+8 , mouse_x+85, mouse_x+85, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "(" + to_string(rSelec).substr(0,4) + ","
                                                             + to_string(gSelec).substr(0,4) + ","
                                                             + to_string(bSelec).substr(0,4) + ")", 0.08);
            }
        }




    /* ====== Mouse em um botao especial ====== */
        if (m_y >= height-25 && m_y <= height-1)
        {
            // Botao de claridade
            if (m_x > 0 && m_x < 25)
            {
                if (nightMode == false)
                {                    
                    double x[] = {mouse_x+8 , mouse_x+102, mouse_x+102, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27 , mouse_y-15 , mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Alternar pra Noite", 0.08);
                }
                else
                {
                    double x[] = {mouse_x+8 , mouse_x+94, mouse_x+94, mouse_x+8};
                    double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                    glColor3f(1.0, 1.0, 0.0);
                    forward_list<vertice> v = preenchePoligono(x, y, 4);
                    desenhaVertices(v);

                    glColor3f(0.0, 0.0, 0.0);
                    draw_text_stroke(mouse_x+10, mouse_y-25, "Alternar pra Dia", 0.08);
                }
            }

            // Botao de reset
            else if (m_x > 25 && m_x < 50)
            {
                double x[] = {mouse_x+8 , mouse_x+37, mouse_x+37, mouse_x+8};
                double y[] = {mouse_y-27, mouse_y-27, mouse_y-15, mouse_y-15};

                glColor3f(1.0, 1.0, 0.0);
                forward_list<vertice> v = preenchePoligono(x, y, 4);
                desenhaVertices(v);

                glColor3f(0.0, 0.0, 0.0);
                draw_text_stroke(mouse_x+10, mouse_y-25, "Reset", 0.08);
            }
        }
}


#endif
