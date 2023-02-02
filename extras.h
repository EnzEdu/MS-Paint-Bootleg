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
    // Parte superior da GUI (azul)
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
        int qntBotoes = 4;
        for (int i = 0; i < 20*qntBotoes; i += 20)
        {
            // Background (cinza)
            glColor3f(0.57, 0.58, 0.58);
            glBegin(GL_TRIANGLES);
                glVertex2f(50 + i, h - 45);
                glVertex2f(70 + i, h - 45);
                glVertex2f(70 + i, h - 25);
            glEnd();

            glBegin(GL_TRIANGLES);
                glVertex2f(70 + i, h - 25);
                glVertex2f(50 + i, h - 25);
                glVertex2f(50 + i, h - 45);
            glEnd();


            // Bordas
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
        
            glBegin(GL_LINE_LOOP);
                glVertex2f(51 + i, h - 44);
                glVertex2f(69 + i, h - 44);
                glVertex2f(69 + i, h - 27);
                glVertex2f(51 + i, h - 27);
            glEnd();
        }
            

        // Icones

            // Mouse
            glColor3f(0.0, 0.0, 0.0);   // Contorno
            glBegin(GL_LINE_LOOP);
                glVertex2f(61, h - 43); // Comecando de baixo
                glVertex2f(65, h - 43); // Esquerda - direita
                glVertex2f(61, h - 38); //
                glVertex2f(66, h - 37);
                glVertex2f(54, h - 28); // Ponto mais alto
                glVertex2f(54, h - 42);
                glVertex2f(58, h - 38);
            glEnd();

            // Reta
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINES);
                    glVertex2f(71 + i, h - 43);
                    glVertex2f(87 + i, h - 27);
                glEnd();
            }

            // Retangulo
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex2f( 93 + i, h - 41 + i);
                    glVertex2f(106 + i, h - 41 + i);
                    glVertex2f(106 + i, h - 30 - i);
                    glVertex2f( 93 + i, h - 30 - i);
                glEnd();
            }

            // Triangulo
            glColor3f(0.0, 0.0, 0.0);
            for (int i = 0; i < 2; i++)
            {
                glBegin(GL_LINE_LOOP);
                    glVertex2f(114 + i, h - 41 + i);
                    glVertex2f(126 - i, h - 41 + i);
                    glVertex2f(120, h - 29 - 2*i);
                glEnd();
            }
}








void trataCliqueBotao(int botao, int height)
{
    // Pinta a borda dos botoes
    int qntBotoes = 4;
    for (int i = 0; i < 20*qntBotoes; i += 20)
    {
        if (i / 20 == botao)
        {
            // Botao selecionado (borda vermelha)
            glColor3f(1.0, 0.0, 0.0);
        }
        else
        {
            // Botoes nao selecionados (borda branca)
            glColor3f(1.0, 1.0, 1.0);
        }
        
        glBegin(GL_LINE_LOOP);
            glVertex2f(51 + i, height - 44);
            glVertex2f(69 + i, height - 44);
            glVertex2f(69 + i, height - 27);
            glVertex2f(51 + i, height - 27);
        glEnd();
    }
}


















































#endif