/*
 * Atividade 06 - Computacao Grafica
 * Codigo OpenGL responsavel pelo plano de fundo e outros extras
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: 22/12/2022
*/

#ifndef extras_h
#define extras_h

// include <cmath>


void desenhaExtras(int w, int h) 
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
        int qntBotoes = 3;
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
        }
            

        // Icones

            // Mouse
            /*
            glColor3f(1.0, 1.0, 1.0);
            glBegin(GL_LINE_LOOP);
            //glBegin(GL_POLYGON);
                glVertex2f(61, h - 42); // 
                glVertex2f(65, h - 42); // 
                glVertex2f(62, h - 37); //
                glVertex2f(65, h - 37);
                glVertex2f(55, h - 29); // 
                glVertex2f(55, h - 41);
                glVertex2f(57, h - 36);
            glEnd();
            */
            glColor3f(0.0, 0.0, 0.0);
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
            

}



void atualizaBotao(int botao, int h)
{
    /*
    if (botao == 'L')
    {
        glColor3f(1.0, 0.0, 0.0);
        glBegin(GL_TRIANGLES);
            glVertex2f(55, h - 45);
            glVertex2f(70, h - 45);
            glVertex2f(70, h - 30);
        glEnd();

        glColor3f(0.0, 0.0, 1.0);
        glBegin(GL_TRIANGLES);
            glVertex2f(70, h - 30);
            glVertex2f(55, h - 30);
            glVertex2f(55, h - 45);
        glEnd();
    }
    */
    switch (botao)
    {
        case 0:
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
                glVertex2f(50, h - 45);
                glVertex2f(70, h - 45);
                glVertex2f(70, h - 26);
                glVertex2f(50, h - 26);
            glEnd();
        break;
        
        case 1:     // LIN
            //printf("%d ", h-26);
            glColor3f(1.0, 0.0, 0.0);
            glBegin(GL_LINE_LOOP);
                
                glVertex2f(70, h - 45);
                glVertex2f(90, h - 45);
                glVertex2f(90, h - 26);
                glVertex2f(70, h - 26);
                
            glEnd();
        break;

        default:

        break;
    }
}




















#endif