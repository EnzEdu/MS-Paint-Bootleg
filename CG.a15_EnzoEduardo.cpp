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
    CLICK = 0,
    LIN,    // Linha
    TRI,        // Triangulo
    RET,        // Retangulo
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
//int modo = LIN;
int modo = CLICK;

// Largura e altura da janela
int width = 512, height = 512;





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
void trataCliqueBotao(int botao, int height);





/*
 * Funcao principal
 */
int main(int argc, char** argv){

    glutInit(&argc, argv);                          // Passagem de parametros C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Selecao do modo do Display e do sistema de Cor utilizado
    //glutInitWindowSize(768, 512);                   // Tamanho da janela do OpenGL
    glutInitWindowSize(width, height);              // Tamanho da janela do OpenGL
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
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y){
    switch (key) {
        case ESC: exit(EXIT_SUCCESS); break;
    }
}


/*
 * Controla o menu pop-up
 */
void menu_popup(int value){
    if (value == 0) exit(EXIT_SUCCESS);
    modo = value;
}


/*
 * Controle dos botoes do mouse
 */
void mouse(int button, int state, int x, int y){
    switch (button) {

        case GLUT_LEFT_BUTTON:

            switch(modo){

                case CLICK:

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

//                    if (state == GLUT_DOWN) {
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
                            
                            // Se o clique nao foi na area de opcoes
                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
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
 * Ajusta a projecao para o redesenho da janela
 */
void reshape(int w, int h)
{
    glMatrixMode(GL_PROJECTION);    // Muda pro modo de projecao
    glLoadIdentity();               // Carrega a matriz identidade

	// Definindo o Viewport para o tamanho da janela
	glViewport(0, 0, w, h);

//	width = w;
// height = h;

    glOrtho (0, w, 0, h, -1 ,1);    // (0,0) no canto inferior da tela,
                                    // independente da resolucao
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
    desenhaExtras(wi, he);



    /*
    glColor3f (1.0, 0.0, 0.0); // Seleciona a cor default como preto
    drawFormas(); // Desenha as formas geometricas da lista

    //Desenha texto com as coordenadas da posicao do mouse
    draw_text_stroke(0, 0, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.2);
    */

    drawFormas(); // Desenha as formas geometricas da lista

    trataCliqueBotao(modo, he);



    glutSwapBuffers(); // manda o OpenGl renderizar as primitivas
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
    //Apos o primeiro clique, desenha a reta com a posicao atual do mouse
    if(click1) retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
    
    //Percorre a lista de formas geometricas para desenhar
    for(forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++){
        switch (f->tipo) {
            case LIN:
                int i = 0, x[2], y[2];
                //Percorre a lista de vertices da forma linha para desenhar
                for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                    x[i] = v->x;
                    y[i] = v->y;
                }
                //Desenha o segmento de reta apos dois cliques
                retaBresenham(x[0], y[0], x[1], y[1]);
            break;
//            case RET:
//            break;
        }
    }
}


/*
 * Funcao que implementa o Algoritmo de Bresenham de rasterizacao de segmentos de reta
*/
void retaBresenham(double x1, double y1, double x2, double y2)
{
    // Coordenadas
    int xInicio = (int) x1;
    int yInicio = (int) y1;
    int xFim = (int) x2;
    int yFim = (int) y2;

    int trueX;
    int trueY;


    // Verifica se a reta esta no primeiro octante

    // Calcula as variaveis usadas na conversao
    int varX  = xFim - xInicio;
    int varY  = yFim - yInicio;
    int simetrico, declive = 0;


    // Testa se o declive da reta eh negativo
    int coeficienteAngular = varX * varY;
    if (coeficienteAngular < 0)
    {
        simetrico = 1;

        yInicio = (-1)*yInicio;
        yFim = (-1)*yFim;

        varY = (-1)*varY;
    }


    // Testa se o declive eh maior que 1
    if (abs(varX) < abs(varY))
    {
        declive = 1;

        int tmp;
        tmp = xInicio;
        xInicio = yInicio;
        yInicio = tmp;

        tmp = xFim;
        xFim = yFim;
        yFim = tmp;
        
        tmp = varX;
        varX = varY;
        varY = tmp;
    }


    // Testa se xInicio eh maior que xFim
    if (xInicio > xFim)
    {
        int tmp;

        tmp = xInicio;
        xInicio = xFim;
        xFim = tmp;

        tmp = yInicio;
        yInicio = yFim;
        yFim = tmp;

        varX = (-1)*varX;
        varY = (-1)*varY;
    }





    // Desenha o primeiro extremo
    trueX = xInicio, trueY = yInicio;

    // Transformacao Inversa do mesmo
    if (declive == 1)
    {
        int tmp;
        tmp = trueX;
        trueX = trueY;
        trueY = tmp;
    }

    if (simetrico == 1)
    {
        trueY = (-1)*trueY;
    }

    drawPixel(trueX, trueY);



    // Algoritmo de Bresenham
    int Xi = xInicio, Yi = yInicio;
    int Di = 0;
    int Dii = (2*varY) - varX;
    int incE  = 2*varY;
    int incNE = 2*(varY - varX);

    while (Xi < xFim)
    {
        // Incrementa o X(i)
        Xi++;

        // Identifica D(i)  --- D(i+1) da linha passada ---
        Di = Dii;


        // Acha o avanco
        if (Di > 0)
        {
            // Avanco NE (acima do ponto medio)
            Dii = Di + incNE;
            Yi++;               // Incrementa Y(i)

        } else
        {
            // Avanco E (em cima e abaixo do ponto medio)
            Dii = Di + incE;
        }



        // Transformacao Inversa

        // Coordenadas reais
        int newX = Xi, newY = Yi;

        // Conserta o declive
        if (declive == 1)
        {
            int tmp;
            tmp = newX;
            newX = newY;
            newY = tmp;
        }

        // Conserta a simetria
        if (simetrico == 1)
        {
            newY = (-1)*newY;
        }

        // Desenha o ponto com as coordenadas transformadas
        drawPixel(newX, newY);
    }



    // Desenha o segundo extremo
    trueX = xFim, trueY = yFim;

    // Transformacao inversa do mesmo
    if (declive == 1)
    {
        int tmp;
        tmp = trueX;
        trueX = trueY;
        trueY = tmp;
    }

    if (simetrico == 1)
    {
        trueY = (-1)*trueY;
    }

    drawPixel(trueX, trueY);
}

void verificaCliqueBotao(int mouseX, int mouseY)
{
    if (mouseClick_y1 >= height-45 && mouseClick_y1 <= height-25)
    {

        // Clicou no primeiro botao vago
        if (mouseClick_x1 > 50 && mouseClick_x1 < 70)
        {
            printf("CLIQUE!!!!!\n");
            modo = CLICK;
        }

        // Botao LINHA
        else if (mouseClick_x1 > 70 && mouseClick_x1 < 90)
        {
            printf("LINHA!!!!!\n");
            modo = LIN;
        }

    }
}

void trataCliqueBotao(int botao, int height)
{
    // Pinta a borda dos botoes
    int qntBotoes = 3;
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