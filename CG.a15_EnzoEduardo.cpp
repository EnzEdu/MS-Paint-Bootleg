/*
 * Atividade 15 - Computacao Grafica
 * Editor grafico feito com OpenGL/GLUT
 * Autor: Enzo Eduardo
 * Data: ??/02/2023
*/






// Bibliotecas utilizadas pelo OpenGL
#ifdef __APPLE__                        // Mac OS
    #define GL_SILENCE_DEPRECATION
    #include <GLUT/glut.h>
    #include <OpenGL/gl.h>
    #include <OpenGL/glu.h>
#else                                   // Windows/Linux
    #include <GL/glut.h>
    #include <GL/gl.h>
    #include <GL/glu.h>
#endif


// Bibliotecas
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <forward_list>
#include "glut_text.h"
#include "extras.h"
using namespace std;


// Teclas do teclado e seus valores ASCII
#define ESC 27





/*
 * Declaracao de constantes e variaveis globais
*/

// Enumeracao com os tipos de formas geometricas
enum estados_mouse
{
    MOU = 0,
    LIN,    // Linha
//    BAL,
//    TRI,        // Triangulo
    RET,        // Retangulo
    TRI,
    POL,        // Poligono
    CIR         // Circulo
};

// Verifica se foi realizado o primeiro clique do mouse
bool click1 = false;

// Coordenadas da posicao atual do mouse
int m_x, m_y;

// Coordenadas do primeiro clique e do segundo clique do mouse
int mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2;

// Indica o tipo de forma geometrica ativa para desenhar
int modo = MOU;

// Largura e altura da janela
int width = 512, height = 512;

// Contador de vertices?
int contCoordenadas = 0;





// Definicao de vertice
struct vertice{
    int x;
    int y;
};

// Definicao das formas geometricas
struct forma{
    int tipo;
    forward_list<vertice> v; //lista encadeada de vertices
};

// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo){
    forma f;
    f.tipo = tipo;
    formas.push_front(f);
}

// Funcao para armazenar um vertice na forma do inicio da lista de formas geometricas
// Armazena sempre no inicio da lista
void pushVertice(int x, int y){
    vertice v;
    v.x = x;
    v.y = y;
    formas.front().v.push_front(v);
}

//Fucao para armazenar uma Linha na lista de formas geometricas
void pushLinha(int x1, int y1, int x2, int y2){
    pushForma(LIN);
    pushVertice(x1, y1);
    pushVertice(x2, y2);
}





/*
 * Declaracoes antecipadas (forward) das funcoes (assinaturas das funcoes)
 */
void init(void);
void reshape(int w, int h);
void display(void);
void menu_popup(int value);
void keyboard(unsigned char key, int x, int y);
void mouse(int button, int state, int x, int y);
void mousePassiveMotion(int x, int y);
void drawPixel(int x, int y);

// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();

// Funcao que implementa o Algoritmo de Bresenham para rasterizacao de segmentos de retas
void retaBresenham(double x1, double y1, double x2, double y2);

void verificaCliqueBotao(int mouseX, int mouseY);
//void trataCliqueBotao(int botao, int height);







/*
 * Funcao principal
 */
int main(int argc, char** argv){

    glutInit(&argc, argv);                          // Passagem de parametros C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Selecao do modo do Display e do sistema de Cor utilizado
    //glutInitWindowSize(768, 512);                   // Tamanho da janela do OpenGL
    glutInitWindowSize(width, height);              // Tamanho da janela do OpenGL
//    glutInitWindowSize(glutGet(GLUT_SCREEN_WIDTH), glutGet(GLUT_SCREEN_HEIGHT));              // Tamanho da janela do OpenGL
    glutInitWindowPosition(100, 100);               // Posicao inicial da janela do OpenGL
    glutCreateWindow("Computacao Grafica: Paint");  // Da nome para uma janela OpenGL

    init();                                         // Chama a funcao init()

    glutReshapeFunc(reshape);                       // Callback para redesenhar a tela
    glutKeyboardFunc(keyboard);                     // Callback do teclado
    glutMouseFunc(mouse);                           // Callback do mouse
    glutPassiveMotionFunc(mousePassiveMotion);      // Callback do movimento passivo do mouse
    glutDisplayFunc(display);                       // Callback de desenho

    glutCreateMenu(menu_popup);
    glutAddMenuEntry("Linha", LIN);
//    glutAddMenuEntry("Retangulo", RET);
    glutAddMenuEntry("Sair", 0);
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    glutMainLoop();                                 // Executa o loop do OpenGL

    return EXIT_SUCCESS;                            // Retorna 0 para o tipo inteiro da funcao main()
}





/*
 * Inicializa alguns parametros do GLUT
 */
void init(void){
    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
}


/*
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);    // Muda pro modo de projecao
    glLoadIdentity();               // Carrega a matriz identidade

    // Definindo o Viewport para o tamanho da janela
    glViewport(0, 0, w, h);

//  width = w;
// height = h;

    glOrtho (0, w, 0, h, -1 ,1);    // (0,0) no canto inferior da tela,
                                    // independente da resolucao
}


/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case ESC: exit(EXIT_SUCCESS); break;
    }
}


/*
 * Controle dos botoes do mouse
 */
void mouse(int button, int state, int x, int y){
    switch (button) {

        case GLUT_LEFT_BUTTON:

            switch(modo){

                case MOU:

                    // Clique
                    if (state == GLUT_UP)
                    {
                        mouseClick_x1 = x;
                        mouseClick_y1 = height - y - 1;

                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                        printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);
                    } 

                    // Segurou
                    else
                    {
                        // Modo de selecionar
                    }

                break;


                case LIN:
                    if (state == GLUT_UP) {
                        if (click1 == true)
                        {
                            mouseClick_x2 = x;
                            mouseClick_y2 = height - y - 1;

                            // Se o clique nao foi na area de opcoes
                            if (mouseClick_y2 <= height - 50)
                            {
                                click1 = false;
                                pushLinha(mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2);
                                glutPostRedisplay();
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x2, mouseClick_y2);
                                glutPostRedisplay();
                            }

                            printf("Clique 2(%d, %d)\n",mouseClick_x2,mouseClick_y2);
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;
                            
                            // Verifica se o clique nao foi na area de opcoes
                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                glutPostRedisplay();
                            }
                            else
                            {
                                // Verifica se clicou em algum botao
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                glutPostRedisplay();
                            }

                            printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);
                        }
                    }
                break;


                case RET:
                    if (state == GLUT_UP)
                    {
                        if (click1 == true)
                        {
                            mouseClick_x2 = x;
                            mouseClick_y2 = height - y - 1;

                            if (mouseClick_y2 <= height - 50)
                            {
                                click1 = false;
                                
                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y2);
                                pushVertice(mouseClick_x2, mouseClick_y2);
                                pushVertice(mouseClick_x2, mouseClick_y1);
                                pushVertice(mouseClick_x1, mouseClick_y1);

                                glutPostRedisplay();   
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x2, mouseClick_y2);
                                glutPostRedisplay();
                            }

                            printf("Clique 2(%d, %d)\n",mouseClick_x2,mouseClick_y2);
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                glutPostRedisplay();
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                glutPostRedisplay();
                            }

                            printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);
                        }
                    }
                break;


                case TRI:
                    if (state == GLUT_UP)
                    {
                        if (click1 == true)
                        {
                            if (height - y - 1 <= height - 50)
                            {
                                contCoordenadas--;

                                if (contCoordenadas == 1)
                                {
                                    mouseClick_x2 = x;
                                    mouseClick_y2 = height - y - 1;
                                    click1 = true;
                                    printf("Clique 2(%d, %d)\n", mouseClick_x2, mouseClick_y2);
                                }
                                else
                                {
                                    pushForma(modo);
                                    pushVertice(mouseClick_x1, mouseClick_y1);
                                    pushVertice(mouseClick_x2, mouseClick_y2);
                                    pushVertice(x, height - y - 1);

                                    click1 = false;
                                    printf("Clique 3(%d, %d)\n", x, height - y - 1);
                                    contCoordenadas = 3;
                                }
                            }
                            else
                            {
                                verificaCliqueBotao(x, height - y - 1);
                                glutPostRedisplay();
                            }
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                contCoordenadas--;
                                click1 = true;

                                printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                glutPostRedisplay();
                            }
                        }
                    }
                break;



            }

        break;

//        case GLUT_MIDDLE_BUTTON:
//            if (state == GLUT_DOWN) {
//                glutPostRedisplay();
//            }
//        break;
//
//        case GLUT_RIGHT_BUTTON:
//            if (state == GLUT_DOWN) {
//                glutPostRedisplay();
//            }
//        break;
            
    }
}


/*
 * Controle da posicao do cursor do mouse
 */
void mousePassiveMotion(int x, int y){
    m_x = x; m_y = height - y - 1;
    glutPostRedisplay();
}


/*
 * Controla os desenhos na tela
 */
void display(void)
{
    glMatrixMode(GL_MODELVIEW);     // Muda pro modo de desenho
    glClear(GL_COLOR_BUFFER_BIT);   // Limpa o Buffer de Cores
    glLoadIdentity();               // Carrega a matriz identidade


    // Variaveis que pegam medidas atuais da janela
    float wi = glutGet(GLUT_WINDOW_WIDTH);
    float he = glutGet(GLUT_WINDOW_HEIGHT);


    // Carrega o layout do app
    desenhaGUI(wi, he, modo);



    /*
    glColor3f (1.0, 0.0, 0.0); // Seleciona a cor default como preto
    drawFormas(); // Desenha as formas geometricas da lista

    //Desenha texto com as coordenadas da posicao do mouse
    draw_text_stroke(0, 0, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.2);
    */

    drawFormas(); // Desenha as formas geometricas da lista
    //printf("contCoord = %d\n", contCoordenadas);

    // Atualiza a borda dos botoes
    // de acordo com o modo selecionado
    //trataCliqueBotao(modo, he);



    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
}


/*
 * Controla o menu pop-up
 */
void menu_popup(int value){
    if (value == 0) exit(EXIT_SUCCESS);
    modo = value;
}





/*
 * Funcao para desenhar apenas um pixel na tela
 */
void drawPixel(int x, int y){
    glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
        glVertex2i(x, y);
    glEnd();  // indica o fim do ponto
}




/*
 *Funcao que desenha a lista de formas geometricas
 */
void drawFormas(){

    // Preview da forma a ser desenhada
    if (!(click1 == true) ^ (contCoordenadas <= 2))
    {
        switch(modo)
        {
            case LIN:
                retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
            break;

            case RET:
                retaBresenham(mouseClick_x1, mouseClick_y1, mouseClick_x1, m_y);
                retaBresenham(mouseClick_x1, mouseClick_y1, m_x, mouseClick_y1);
                retaBresenham(mouseClick_x1, m_y, m_x, m_y);
                retaBresenham(m_x, mouseClick_y1, m_x, m_y);
            break;

            case TRI:
                if (contCoordenadas == 2)
                {
                    retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                }
                else if (contCoordenadas == 1)
                {
                    retaBresenham(mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2);
                    retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                    retaBresenham(m_x, m_y, mouseClick_x2, mouseClick_y2);
                }
            break;
        }
    }

    //Percorre a lista de formas geometricas para desenhar
    for(forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++){
        switch (f->tipo) {
            case LIN:
                {
                    int i = 0, x[2], y[2];
                    //Percorre a lista de vertices da forma linha para desenhar
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }
                    //Desenha o segmento de reta apos dois cliques
                    retaBresenham(x[0], y[0], x[1], y[1]);
                }
            break;

            case RET:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[4], y[4];

                    // Itera sob cada forma da lista
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha os quatro lados
                    retaBresenham(x[0], y[0], x[1], y[1]);
                    retaBresenham(x[1], y[1], x[2], y[2]);
                    retaBresenham(x[2], y[2], x[3], y[3]);
                    retaBresenham(x[3], y[3], x[0], y[0]);
                }
            break;

            
            case TRI:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[3], y[3];
    
                    // Itera sob cada forma da lista
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha os tres lados
                    retaBresenham(x[0], y[0], x[1], y[1]);
                    retaBresenham(x[1], y[1], x[2], y[2]);
                    retaBresenham(x[2], y[2], x[0], y[0]);
                }
            break;
            

            default:
            break;
        }
    }
}




/*
 * Funcao que implementa o Algoritmo de Bresenham de rasterizacao de segmentos de reta
*/
void retaBresenham(double x1, double y1, double x2, double y2)
{
    // Coordenadas originais
    int xInicio = (int) x1;
    int yInicio = (int) y1;
    int xFim    = (int) x2;
    int yFim    = (int) y2;
 


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

        drawPixel(bresenhamX, bresenhamY);
    }
}




void verificaCliqueBotao(int mouseX, int mouseY)
{
    if (mouseY >= height-45 && mouseY <= height-25)
    {
        // Botao MOU
        if (mouseX > 50 && mouseX < 70)
        {
            printf("CLIQUE!!!!!\n");
            modo = MOU;
            contCoordenadas = 0;
        }

        // Botao LIN
        else if (mouseX > 70 && mouseX < 90)
        {
            printf("LINHA!!!!!\n");
            modo = LIN;
            contCoordenadas = 0;
        }

        // Botao RET
        else if (mouseX > 90 && mouseX < 110)
        {
            printf("RETANGULO!!!!!\n");
            modo = RET;
            contCoordenadas = 0;
        }

        // Botao TRI
        else if (mouseX > 110 && mouseX < 130)
        {
            printf("TRIANGULO!!!!!\n");
            modo = TRI;
            contCoordenadas = 3;
        }

    }
}




