/*
 * Atividade 06 - Computacao Grafica
 * Codigo OpenGL responsavel pelo plano de fundo e outros extras
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: 22/12/2022
*/

#ifndef extras_h
#define extras_h

// include <cmath>




void desenhaGUI(int w, int h, int modo) 
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


    // Botoes

        // Espacos
        int qntBotoes = 7;

        for (int i = 0; i < 20*qntBotoes; i += 20)
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


            // Triangulo
            /*
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex2f(134 + i, h - 41 + i);
                    glVertex2f(146 - i, h - 41 + i);
                    glVertex2f(140, h - 29 - 2*i);
                glEnd();
            }
            */
            
            glColor3f(0.0, 0.0, 0.0);
            glBegin(GL_POINTS);
                for (int j = 0; j < 2; j++)
                {
                    int p0 = 134 + j;
                    int p1 = 145 - j;

                    // Base
                    for (int i = 0; i < 11; i++)
                    {
                        glVertex2f(p0 + 1*i - j, h - 41 + j);
                    }

                    // Aresta esquerda
                    for (int i = 0; i < 12; i++)
                    {
                        glVertex2f(p0 + (0.5)*i, h - 41 + 1*i);
                    }

                    // Aresta direita
                    for (int i = 0; i < 12; i++)
                    {
                        glVertex2f(p1 - (0.5)*i, h - 41 + 1*i);
                    }
                }
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