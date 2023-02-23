/*
 * Atividade 15 - Computacao Grafica
 * Codigo OpenGL responsavel pela GUI do editor grafico
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: ??/02/2023
*/

#ifndef extras_h
#define extras_h


void desenhaGUI()
{
    // Area GUI (azul)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0, height - 50);
        glVertex2f(width, height - 50);
        glVertex2f(width, height);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(width, height);
        glVertex2f(0, height);
        glVertex2f(0, height - 50);
    glEnd();


    // Barras de cor RGB

        // Contador para definicao de cor
        float cont;

        // Posicao da cor clicada dentro da barra
        int brancoX;

        // Desenho das barras
        for (int i = 0; i < 3; i++)
        {
            // Desenha os extremos de cada barra (branco)
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
                glVertex2f(width-91, height-4  - 15*i);
                glVertex2f(width-9 , height-4  - 15*i);
                glVertex2f(width-9 , height-16 - 15*i);
                glVertex2f(width-91, height-16 - 15*i);
            glEnd();

            // Zera as variaveis
            cont = 0.0; brancoX = 0;


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

                // Desenha progressivamente a barra de cor
                glBegin(GL_LINES);
                    glVertex2f(j, height-5  - 15*i);
                    glVertex2f(j, height-15 - 15*i);
                glEnd();

                cont += 0.0125;
            }

            // Desenha a linha branca (cor escolhida) de cada barra
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
                glVertex2f(brancoX, height-5  - 15*i);
                glVertex2f(brancoX, height-15 - 15*i);
            glEnd();
        }

        // Quadrado com a cor escolhida para visualizacao
        glColor3f(rSelec, gSelec, bSelec);
        glBegin(GL_POLYGON);
            glVertex2f(width-130, height-35);
            glVertex2f(width-105, height-35);
            glVertex2f(width-105, height-10);
            glVertex2f(width-130, height-10);
        glEnd();    


    // Botoes

        // Barra inferior
        int qntBotoesSup = 7;
        for (int i = 0; i < 20*qntBotoesSup; i += 20)
        {
            // Background (cinza)
            glColor3f(0.57, 0.58, 0.58);
            glBegin(GL_TRIANGLES);
                glVertex2f(70 + i, height - 45);
                glVertex2f(90 + i, height - 45);
                glVertex2f(90 + i, height - 25);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex2f(90 + i, height - 25);
                glVertex2f(70 + i, height - 25);
                glVertex2f(70 + i, height - 45);
            glEnd();


            // Bordas
                // Cor
                if (i / 20 == modoForma)
                {
                    // Botao selecionado (borda vermelha)
                    glColor3f(1.0, 0.0, 0.0);
                }
                else
                {
                    // Botoes nao selecionados (borda branca)
                    glColor3f(1.0, 1.0, 1.0);
                }
        
                // Desenho
                glBegin(GL_LINE_LOOP);
                    glVertex2f(71 + i, height - 44);
                    glVertex2f(89 + i, height - 44);
                    glVertex2f(89 + i, height - 27);
                    glVertex2f(71 + i, height - 27);
                glEnd();
        }
        

        // Barra superior
        int qntBotoesInf = 3;
        for (int i = 0; i < 20*qntBotoesInf; i += 20)
        {
            // Background (cinza)
            glColor3f(0.57, 0.58, 0.58);
            glBegin(GL_TRIANGLES);
                glVertex2f(110 + i, height - 25);
                glVertex2f(130 + i, height - 25);
                glVertex2f(130 + i, height -  5);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex2f(130 + i, height -  5);
                glVertex2f(110 + i, height -  5);
                glVertex2f(110 + i, height - 25);
            glEnd();


            // Bordas
                // Cor
                if ((modoForma >= 7) && ((i+40) / 20 == modoForma-5))
                {
                    // Botao selecionado (borda vermelha)
                    glColor3f(1.0, 0.0, 0.0);
                }
                else
                {
                    // Botoes nao selecionados (borda branca)
                    glColor3f(1.0, 1.0, 1.0);
                }
        
                // Desenho
                glBegin(GL_LINE_LOOP);
                    glVertex2f(111 + i, height - 24);
                    glVertex2f(129 + i, height - 24);
                    glVertex2f(129 + i, height -  7);
                    glVertex2f(111 + i, height -  7);
                glEnd();
        }


        // Barra de transformacoes
        if (modoForma == 0)                  // Se o modo atual eh MOU
        {
            int qntBotoesTrans = 5;
            for (int i = 0; i < 2; i++)
            {
                for (int j = 0; j <= (qntBotoesTrans-i) % 3; j++)
                {
                    // Background (cinza)
                    glColor3f(0.57, 0.58, 0.58);
                    glBegin(GL_TRIANGLES);
                        glVertex2f(250 + 20*i, height - 17 - 15*j);
                        glVertex2f(265 + 20*i, height - 17 - 15*j);
                        glVertex2f(265 + 20*i, height - 2  - 15*j);
                    glEnd();

                    glBegin(GL_TRIANGLES);
                        glVertex2f(265 + 20*i, height - 2  - 15*j);
                        glVertex2f(250 + 20*i, height - 2  - 15*j);
                        glVertex2f(250 + 20*i, height - 17 - 15*j);
                    glEnd();

                    // Bordas
                        // Cor
                        if ((i == (modoTransf-10) / 3) && (j == (modoTransf-10) % 3))
                        {
                            // Botao selecionado (borda vermelha)
                            glColor3f(1.0, 0.0, 0.0);
                        }
                        else
                        {
                            // Botoes nao selecionados (borda branca)
                            glColor3f(1.0, 1.0, 1.0);
                        }
        
                        // Desenho
                        glBegin(GL_LINE_LOOP);
                            glVertex2f(251 + 20*i, height - 16 - 15*j);
                            glVertex2f(264 + 20*i, height - 16 - 15*j);
                            glVertex2f(264 + 20*i, height - 4  - 15*j);
                            glVertex2f(251 + 20*i, height - 4  - 15*j);
                        glEnd();
                }
            }
        }
        

        // Icones        
            // Mouse
            glColor3f(0.0, 0.0, 0.0);   // Contorno
            glBegin(GL_LINE_LOOP);
                glVertex2f(81, height - 43); // Comecando de baixo
                glVertex2f(85, height - 43); // Esquerda - direita
                glVertex2f(81, height - 38); //
                glVertex2f(86, height - 37);
                glVertex2f(74, height - 28); // Ponto mais alto
                glVertex2f(74, height - 42);
                glVertex2f(78, height - 38);
            glEnd();


            // Reta
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINES);
                    glVertex2f(91 + i, height - 43);
                    glVertex2f(107 + i, height - 27);
                glEnd();
            }


            // Retangulo
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(113 + i, height - 41 + i);
                        glVertex2f(126 + i, height - 41 + i);
                        glVertex2f(126 + i, height - 30 - i);
                        glVertex2f(113 + i, height - 30 - i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(113, height - 21);
                    glVertex2f(126, height - 21);
                    glVertex2f(126, height - 10);
                    glVertex2f(113, height - 10);
                glEnd();


            // Triangulo
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(134 + i, height - 41 + i);
                        glVertex2f(146 - i, height - 41 + i);
                        glVertex2f(140, height - 29 - 2*i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(134, height - 21);
                    glVertex2f(146, height - 21);
                    glVertex2f(140, height -  9);
                glEnd();
            

            // Poligono generico
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(154 + i, height - 41 + i);
                        glVertex2f(166 - i, height - 41 + i);
                        glVertex2f(158 - i, height - 36);
                        glVertex2f(166 - i, height - 30 - i);
                        glVertex2f(154 + i, height - 30 - i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(154, height - 21);
                    glVertex2f(166, height - 21);
                    glVertex2f(158, height - 16);
                    glVertex2f(166, height - 10);
                    glVertex2f(154, height - 10);
                glEnd();


            // Circulo
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POINTS);
                for (int i = 0; i < 1; i++) {

                    int p0 = 177 + i;
                    int p1 = 181 - i;
                    int h0 = height - 42 + i;

                    for (int j = 0; j < 5; j++) {
                        glVertex2f(p0 + 1*j, h0);
                    }

                    for (int j = 0; j < 4; j++) {
                        glVertex2f(p0, h0);
                        glVertex2f(p1, h0);
                        p0 -= 1;
                        p1 += 1;
                        h0 += 1;
                    }
                
                    for (int j = 0; j < 4; j++) {
                        glVertex2f(p0, h0);
                        glVertex2f(p1, h0);
                        h0 += 1;
                    }

                    for (int j = 0; j < 4; j++) {
                        glVertex2f(p0, h0);
                        glVertex2f(p1, h0);
                        p0 += 1;
                        p1 -= 1;
                        h0 += 1;
                    }

                    for (int j = 0; j < 5; j++) {
                        glVertex2f(p0 + 1*j, h0);
                    }
                }
            glEnd();


            // Balde
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POINTS);

                // Lata
                for(int j = 0; j < 10; j++) {
                    glVertex2f(194    , height-42 + j);
                    glVertex2f(194 + j, height-42);
                    glVertex2f(204    , height-33 - j);
                    glVertex2f(204 - j, height-33);
                }

                // Alca
                for (int j = 0; j < 4; j++) {
                    glVertex2f(196 + j, height-32 + j);
                    glVertex2f(202 - j, height-32 + j);
                }

                
                // Tinta

            glEnd();

    glutPostRedisplay();
}


















































#endif