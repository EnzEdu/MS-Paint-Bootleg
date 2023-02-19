/*
 * Atividade 06 - Computacao Grafica
 * Codigo OpenGL responsavel pelo plano de fundo e outros extras
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: 22/12/2022
*/

#ifndef extras_h
#define extras_h


void desenhaGUI(int w, int h, int modo, float rSelec, float gSelec, float bSelec) 
{
    // Area GUI (azul)
    glColor3f(0.0, 0.0, 1.0);
    glBegin(GL_TRIANGLES);
        glVertex2f(0, h - 50);
        glVertex2f(w, h - 50);
        glVertex2f(w, h);
    glEnd();

    glBegin(GL_TRIANGLES);
        glVertex2f(w, h);
        glVertex2f(0, h);
        glVertex2f(0, h - 50);
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
                glVertex2f(w-91, h-4  - 15*i);
                glVertex2f(w-9 , h-4  - 15*i);
                glVertex2f(w-9 , h-16 - 15*i);
                glVertex2f(w-91, h-16 - 15*i);
            glEnd();

            // Zera as variaveis
            cont = 0.0; brancoX = 0;


            for (int j = w-90; j <= w-10; j++)
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
                    glVertex2f(j, h-5  - 15*i);
                    glVertex2f(j, h-15 - 15*i);
                glEnd();

                cont += 0.0125;
            }

            // Desenha a linha branca (cor escolhida) de cada barra
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINES);
                glVertex2f(brancoX, h-5  - 15*i);
                glVertex2f(brancoX, h-15 - 15*i);
            glEnd();
        }

        // Quadrado com a cor escolhida para visualizacao
        glColor3f(rSelec, gSelec, bSelec);
        glBegin(GL_POLYGON);
            glVertex2f(w-130, h-35);
            glVertex2f(w-105, h-35);
            glVertex2f(w-105, h-10);
            glVertex2f(w-130, h-10);
        glEnd();    


    // Botoes

        // Barra inferior
        int qntBotoesSup = 7;
        for (int i = 0; i < 20*qntBotoesSup; i += 20)
        {
            // Background (cinza)
            glColor3f(0.57, 0.58, 0.58);
            glBegin(GL_TRIANGLES);
                glVertex2f(70 + i, h - 45);
                glVertex2f(90 + i, h - 45);
                glVertex2f(90 + i, h - 25);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex2f(90 + i, h - 25);
                glVertex2f(70 + i, h - 25);
                glVertex2f(70 + i, h - 45);
            glEnd();


            // Bordas
                // Cor
                if (i / 20 == modo)
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
                    glVertex2f(71 + i, h - 44);
                    glVertex2f(89 + i, h - 44);
                    glVertex2f(89 + i, h - 27);
                    glVertex2f(71 + i, h - 27);
                glEnd();
        }
        
        // Barra superior
        int qntBotoesInf = 3;
        for (int i = 0; i < 20*qntBotoesInf; i += 20)
        {
            // Background (cinza)
            glColor3f(0.57, 0.58, 0.58);
            glBegin(GL_TRIANGLES);
                glVertex2f(110 + i, h - 25);
                glVertex2f(130 + i, h - 25);
                glVertex2f(130 + i, h -  5);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex2f(130 + i, h -  5);
                glVertex2f(110 + i, h -  5);
                glVertex2f(110 + i, h - 25);
            glEnd();


            // Bordas
                // Cor
                if ((modo >= 7) && ((i+40) / 20 == modo-5))
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
                    glVertex2f(111 + i, h - 24);
                    glVertex2f(129 + i, h - 24);
                    glVertex2f(129 + i, h -  7);
                    glVertex2f(111 + i, h -  7);
                glEnd();
        }


        // Icones        
            // Mouse
            glColor3f(0.0, 0.0, 0.0);   // Contorno
            glBegin(GL_LINE_LOOP);
                glVertex2f(81, h - 43); // Comecando de baixo
                glVertex2f(85, h - 43); // Esquerda - direita
                glVertex2f(81, h - 38); //
                glVertex2f(86, h - 37);
                glVertex2f(74, h - 28); // Ponto mais alto
                glVertex2f(74, h - 42);
                glVertex2f(78, h - 38);
            glEnd();


            // Reta
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINES);
                    glVertex2f(91 + i, h - 43);
                    glVertex2f(107 + i, h - 27);
                glEnd();
            }


            // Retangulo
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(113 + i, h - 41 + i);
                        glVertex2f(126 + i, h - 41 + i);
                        glVertex2f(126 + i, h - 30 - i);
                        glVertex2f(113 + i, h - 30 - i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(113, h - 21);
                    glVertex2f(126, h - 21);
                    glVertex2f(126, h - 10);
                    glVertex2f(113, h - 10);
                glEnd();


            // Triangulo
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(134 + i, h - 41 + i);
                        glVertex2f(146 - i, h - 41 + i);
                        glVertex2f(140, h - 29 - 2*i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(134, h - 21);
                    glVertex2f(146, h - 21);
                    glVertex2f(140, h -  9);
                glEnd();
            

            // Poligono generico
                // Botao inferior
                glColor3f(0.0, 0.0, 0.0);
                for (int i = 0; i < 2; i++)
                {
                    glBegin(GL_LINE_LOOP);
                        glVertex2f(154 + i, h - 41 + i);
                        glVertex2f(166 - i, h - 41 + i);
                        glVertex2f(158 - i, h - 36);
                        glVertex2f(166 - i, h - 30 - i);
                        glVertex2f(154 + i, h - 30 - i);
                    glEnd();
                }

                // Botao superior
                glBegin(GL_POLYGON);
                    glVertex2f(154, h - 21);
                    glVertex2f(166, h - 21);
                    glVertex2f(158, h - 16);
                    glVertex2f(166, h - 10);
                    glVertex2f(154, h - 10);
                glEnd();


            // Circulo
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POINTS);
                for (int i = 0; i < 1; i++) {

                    int p0 = 177 + i;
                    int p1 = 181 - i;
                    int h0 = h - 42 + i;

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
                    glVertex2f(194    , h-42 + j);
                    glVertex2f(194 + j, h-42);
                    glVertex2f(204    , h-33 - j);
                    glVertex2f(204 - j, h-33);
                }

                // Alca
                for (int j = 0; j < 4; j++) {
                    glVertex2f(196 + j, h-32 + j);
                    glVertex2f(202 - j, h-32 + j);
                }

                
                // Tinta

            glEnd();

    glutPostRedisplay();
}


















































#endif