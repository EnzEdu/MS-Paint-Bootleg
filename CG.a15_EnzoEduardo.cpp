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
    LIN,         // Linha
    RET,         // Retangulo
    TRI,         // Triangulo
    POL,         // Poligono
    CIR,         // Circulo
    BAL
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

// Cor selecionada pro desenho
float rSelec = 0.0, gSelec = 0.0, bSelec = 0.0;




// Definicao de vertice
struct vertice{
    int x;
    int y;
};

// Definicao das formas geometricas
struct forma{
    int tipo;
    float rCor, gCor, bCor;
    forward_list<vertice> v; //lista encadeada de vertices
};

// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo){
    forma f;
    f.tipo = tipo;
    f.rCor = rSelec; f.gCor = gSelec; f.bCor = bSelec;
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

//void drawGUI(int w, int h);

// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();

// Funcao que implementa o Algoritmo de Bresenham para rasterizacao de segmentos de retas
void retaBresenham(double x1, double y1, double x2, double y2);

void verificaCliqueBotao(int mouseX, int mouseY);
//void trataCliqueBotao(int botao, int height);

void circuloBresenham(double x1, double y1, double x2, double y2);

void algoritmoFloodFill(int x1, int y1);
bool confereCor(int corPixelClick[], int corPixelArea[]);





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
//    glClearColor(1.0, 1.0, 1.0, 1.0); //Limpa a tela com a cor branca;
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

    // Atualiza as variaveis que salvam o tamanho da janela
    width = w;
    height = h;

    glOrtho (0, w, 0, h, -1 ,1);    // (0,0) no canto inferior da tela,
                                    // independente da resolucao
    //glOrtho (-(w/2), (w/2), -(h/2), (h/2), -1 ,1);
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

                            // Verifica se o clique nao foi na area de opcoes
                            if (mouseClick_y2 <= height - 50)
                            {
                                click1 = false;
                                contCoordenadas = 0;

                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                                pushVertice(mouseClick_x2, mouseClick_y2);
                            }
                            else
                            {
                                // Verifica se o clique foi em algum botao
                                verificaCliqueBotao(mouseClick_x2, mouseClick_y2);
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
                                contCoordenadas++;
                            }
                            else
                            {
                                // Verifica se o clique foi em algum botao
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);
                        }

                        ;
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
                                contCoordenadas = 0;
                                
                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y2);
                                pushVertice(mouseClick_x2, mouseClick_y2);
                                pushVertice(mouseClick_x2, mouseClick_y1);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x2, mouseClick_y2);
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
                                contCoordenadas++;
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);
                        }

                        //glutPostRedisplay();
                    }
                break;


                case TRI:
                    if (state == GLUT_UP)
                    {
                        if (click1 == true)
                        {
                            mouseClick_x2 = x;
                            mouseClick_y2 = height - y - 1;

                            if (mouseClick_y2 <= height - 50)
                            {
                                if (contCoordenadas == 1)
                                {
                                    click1 = true;
                                    contCoordenadas++;
                                    pushVertice(mouseClick_x2, mouseClick_y2);
                                    printf("Clique 2(%d, %d)\n", mouseClick_x2, mouseClick_y2);
                                }
                                else
                                {
                                    click1 = false;
                                    contCoordenadas = 0;
                                    pushVertice(mouseClick_x2, mouseClick_y2);
                                    //pushVertice(mouseClick_x1, mouseClick_y1);
                                    printf("Clique 3(%d, %d)\n", mouseClick_x2, mouseClick_y2);
                                }
                            }
                            else
                            {
                                verificaCliqueBotao(x, height - y - 1);
                            }
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                contCoordenadas++;
                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                                printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }
                        }

                        //glutPostRedisplay();
                    }
                break;


                case POL:
                    if (state == GLUT_UP)
                    {
                        if (click1 == true)
                        {
                            mouseClick_x2 = x;
                            mouseClick_y2 = height - y - 1;

                            if (mouseClick_y2 <= height - 50)
                            {
                                if (contCoordenadas > 3)
                                {
                                    if (mouseClick_x2 == mouseClick_x1 && mouseClick_y2 == mouseClick_y1)
                                    {
                                        click1 = false;
                                        contCoordenadas = 0;
                                        pushVertice(mouseClick_x2, mouseClick_y2);
                                    }
                                    else
                                    {
                                        contCoordenadas++;
                                        pushVertice(mouseClick_x2, mouseClick_y2);
                                        printf("Clique %d(%d, %d)\n", contCoordenadas, mouseClick_x2, mouseClick_y2);
                                    }
                                }
                                else
                                {
                                    contCoordenadas++;
                                    pushVertice(mouseClick_x2, mouseClick_y2);
                                    printf("Clique %d(%d, %d)\n", contCoordenadas, mouseClick_x2, mouseClick_y2);
                                }
                            }
                            else
                            {
                                verificaCliqueBotao(x, height - y - 1);
                            }
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                contCoordenadas++;
                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                                printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }
                        }
                        
                        //glutPostRedisplay();
                    }
                break;


                case CIR:
                    if (state == GLUT_UP)
                    {
                        if (click1 == true)
                        {
                            mouseClick_x2 = x;
                            mouseClick_y2 = height - y - 1;

                            if (mouseClick_y2 <= height - 50)
                            {
                                click1 = false;
                                contCoordenadas = 0;

                                pushForma(modo);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                                pushVertice(mouseClick_x2, mouseClick_y2);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x2, mouseClick_y2);
                            }

                            printf("Clique 2(%d, %d)\n", mouseClick_x2, mouseClick_y2);
                        }
                        else
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                click1 = true;
                                contCoordenadas++;
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }

                        //glutPostRedisplay();
                    }
                break;


                case BAL:
                    if (state == GLUT_UP)
                    {
                        if (click1 == false)
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                pushForma(BAL);
                                pushVertice(mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }

                        //glutPostRedisplay();
                    }
                break;
            }

        break;

//        case GLUT_MIDDLE_BUTTON:
//            if (state == GLUT_DOWN) {
//                //glutPostRedisplay();
//            }
//        break;
//
//        case GLUT_RIGHT_BUTTON:
//            if (state == GLUT_DOWN) {
//                //glutPostRedisplay();
//            }
//        break;
//    //glutPostRedisplay();        
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
    //drawGUI(wi, he);



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





void verificaCliqueBotao(int mouseX, int mouseY)
{
    if (mouseY >= height-45 && mouseY <= height-25)
    {
        // Botao MOU
        if (mouseX > 70 && mouseX < 90)
        {
            printf("CLIQUE!!!!!\n");
            modo = MOU;
            contCoordenadas = 0;
        }

        // Botao LIN
        else if (mouseX > 90 && mouseX < 110)
        {
            printf("LINHA!!!!!\n");
            modo = LIN;
            contCoordenadas = 0;
        }

        // Botao RET
        else if (mouseX > 110 && mouseX < 130)
        {
            printf("RETANGULO!!!!!\n");
            modo = RET;
            contCoordenadas = 0;
        }

        // Botao TRI
        else if (mouseX > 130 && mouseX < 150)
        {
            printf("TRIANGULO!!!!!\n");
            modo = TRI;
            contCoordenadas = 0;
        }

        // Botao POL
        else if (mouseX > 150 && mouseX < 170)
        {
            printf("POLIGONO!!!!!\n");
            modo = POL;
            contCoordenadas = 0;
        }

        // Botao CIR
        else if (mouseX > 170 && mouseX < 190)
        {
            printf("CIRCULO!!!!!\n");
            modo = CIR;
            contCoordenadas = 0;
        }

        // Botao BAL
        else if (mouseX > 190 && mouseX < 210)
        {
            printf("BALDE!!!!!\n");
            modo = BAL;
            contCoordenadas = 0;
        }
    }
}


/*
 * Funcao para desenhar apenas um pixel na tela
 */
void drawPixel(int x, int y)
{
    // Restringe o desenho de pontos para a area de desenho
    if (y <= height - 50)
    {
        glBegin(GL_POINTS); // Seleciona a primitiva GL_POINTS para desenhar
            glVertex2i(x, y);
        glEnd();  // indica o fim do ponto
    }
}


/*
 *Funcao que desenha a lista de formas geometricas
 */
void drawFormas(){

    if (contCoordenadas == 0)       // =========TEMPORARIO========= //
    {
        glColor3f(rSelec, gSelec, bSelec);
    }
    else
    {
        glColor3f(1.0, 0.0, 0.0);
    }

    // Preview da forma a ser desenhada
    if (click1 == true)
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
                if (contCoordenadas == 1)
                {
                    retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                }
                else if (contCoordenadas == 2)
                {
                    retaBresenham(mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2);
                    retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                    retaBresenham(m_x, m_y, mouseClick_x2, mouseClick_y2);
                }
            break;

            case POL:
                {
                    forward_list<forma>::iterator f = formas.begin();

                    int numVertices = 0;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++) {
                        numVertices++;
                    }

                    int i = 0, x[numVertices], y[numVertices];
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Muda a cor do traco ao passar pelo primeiro vertice
                    if (m_x == x[numVertices-1] && m_y == y[numVertices-1])
                    {
                        glColor3f(0.0, 0.0, 1.0);
                    }

                    for (int j = 0; j < numVertices; j++)
                    {
                        if (j != numVertices-1)
                        {
                            retaBresenham(x[j], y[j], x[j+1], y[j+1]);
                        }
                        
                        if (j == 0)
                        {
                            retaBresenham(x[j], y[j], m_x, m_y);
                        }
                    }
                }
            break;

            case CIR:
                circuloBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
            break;
        }
    }

    // Percorre a lista de formas geometricas para desenhar
    for(forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++){
        glColor3f(f->rCor, f->gCor, f->bCor);

        switch (f->tipo) {
            case LIN:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[2], y[2];

                    // Percorre a lista de vertices da forma linha para desenhar
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }
                    // Desenha o segmento de reta apos dois cliques
                    retaBresenham(x[0], y[0], x[1], y[1]);
                }
            break;

            case RET:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[4], y[4];

                    // Salva as coordenadas de cada vertice
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha o retangulo
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

                    // Salva as coordenadas de cada vertice
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha o triangulo
                    for (int j = 0; j < i-1; j++)
                    {
                        retaBresenham(x[j], y[j], x[j+1], y[j+1]);

                        if (j == 1 && i == 3)
                        {
                            retaBresenham(x[2], y[2], x[0], y[0]);
                        }
                    }
                }
            break;
            
            case POL:
                {
                    int numVertices = 0;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++)
                    {
                        numVertices++;
                    }

                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[numVertices], y[numVertices];
                    
                    // Salva as coordenadas de cada vertice
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha o poligono
                    for (int j = 0; j < i-1; j++)
                    {
                        retaBresenham(x[j], y[j], x[j+1], y[j+1]);
                    }
                }
            break;

            case CIR:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0, x[2], y[2];

                    // Salva as coordenadas de cada vertice da forma
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Desenha o circulo
                    circuloBresenham(x[1], y[1], x[0], y[0]);
                }
            break;

            case BAL:
                {
                    // Coordenadas x e y do clique inicial
                    int x, y;

                    // Salva as coordenadas do vertice do pixel clicado
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                        x = v->x;
                        y = v->y;
                    }

                    // Colore a area
                    algoritmoFloodFill(x, y);
                }
            break;

            default:
            break;
        }
    }
}




/*
 * Funcao que implementa o Algoritmo de Bresenham para rasterizacao de segmentos de reta
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

        // Desenha o ponto
        drawPixel(bresenhamX, bresenhamY);
    }
}




/*
 * Funcao que implementa o Algoritmo de Bresenham para rasterizacao de circunferencias
*/
void circuloBresenham(double x1, double y1, double x2, double y2)
{
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
        if (Xi == 0)
        {
            drawPixel(xCentro + bresenhamX, yCentro + bresenhamY);
            drawPixel(xCentro + bresenhamX, yCentro - bresenhamY);
            drawPixel(xCentro + bresenhamY, yCentro + bresenhamX);
            drawPixel(xCentro - bresenhamY, yCentro - bresenhamX);
        }

        else
        {
            drawPixel(xCentro + bresenhamX, yCentro + bresenhamY);
            drawPixel(xCentro + bresenhamY, yCentro + bresenhamX);
            drawPixel(xCentro + bresenhamY, yCentro - bresenhamX);
            drawPixel(xCentro + bresenhamX, yCentro - bresenhamY);

            drawPixel(xCentro - bresenhamX, yCentro - bresenhamY);
            drawPixel(xCentro - bresenhamY, yCentro - bresenhamX);
            drawPixel(xCentro - bresenhamY, yCentro + bresenhamX);
            drawPixel(xCentro - bresenhamX, yCentro + bresenhamY);
        }
    }
}




void algoritmoFloodFill(int x1, int y1)
{
    /*
    glReadBuffer(GL_FRONT);

    // Identifica a cor do pixel clicado
    int corPixelClick[4];
    glReadPixels(x1, y1, 1, 1, GL_RGB, GL_INT, corPixelClick);


    // Identifica a cor do pixel atual
    int corPixelArea[4];



    glColor3f(1.0, 0.0, 0.0);
    //printf("a %d %d\n", x1, y1);

    // Itera sob os pixels acima do clicado
    int y2 = y1 + 1;
    while (y2 < height - 50)
    {
        
        // Carrega a cor do pixel atual
        glReadPixels(x1, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);
        
        if ((corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]))
        {
            //glutSwapBuffers();
            //drawPixel(x1, y2);
            

            //glDrawPixels(GL_FRONT_AND_BACK);
            //drawPixel(x1, y2);
            //glFlush();
            y2++;
        }
        else
        {
            //printf("b\n");
            //printf("c %d %d\n", x1, y2);
            break;
        }
        

        //y2++;

        //printf("%d %d %d\n", corPixelArea[0], corPixelArea[1], corPixelArea[2]);
    }

    retaBresenham(x1, y1, x1, y2);
    */




    /*
    // Altera o buffer de leitura para o buffer da frente
    glReadBuffer(GL_FRONT);


    // Guarda a cor do pixel clicado
    int corPixelClick[4];
    glReadPixels(x1, y1, 1, 1, GL_RGB, GL_INT, corPixelClick);

    // Guarda a cor do pixel atual
    int corPixelArea[4];


    // Define a cor interna
//  glColor3f(rSelec, gSelec, bSelec);
    glColor3f(1.0, 0.0, 0.0);


    // Itera sob os pixels acima do clicado
    int limPixelSuperior = y1 + 1;
    while (limPixelSuperior <= height - 50)
    {
        // Carrega a cor do pixel atual
        glReadPixels(x1, limPixelSuperior, 1, 1, GL_RGB, GL_INT, corPixelArea);
        
        if ((corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]))
        {
            // Encontra o pixel limite da esquerda
            int leftPixelSuperior = x1;
            while (leftPixelSuperior >= 0)
            {
                glReadPixels(leftPixelSuperior, limPixelSuperior, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if ((corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]))
                {
                    leftPixelSuperior--;
                    continue;
                }
                else
                {

                    // Encontra o pixel limite da direita
                    int rightPixelSuperior = x1;
                    while (rightPixelSuperior <= width)
                    {
                        glReadPixels(rightPixelSuperior, limPixelSuperior, 1, 1, GL_RGB, GL_INT, corPixelArea);

                        if ((corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]))
                        {
                            rightPixelSuperior++;
                            continue;
                        }
                        else
                        {
                            retaBresenham(leftPixelSuperior, limPixelSuperior, rightPixelSuperior, limPixelSuperior);
                        }

                        break;
                    }

                }

                break;
            }

            limPixelSuperior++;
        }
        else
        {
            retaBresenham(x1, y1, x1, limPixelSuperior);
            break;
        }
        

        //y2++;

        //printf("%d %d %d\n", corPixelArea[0], corPixelArea[1], corPixelArea[2]);
    }
    */






    // Altera o buffer de leitura para o buffer da frente
    glReadBuffer(GL_FRONT);


    // Guarda a cor do pixel clicado
    int corPixelClick[4];
    glReadPixels(x1, y1, 1, 1, GL_RGB, GL_INT, corPixelClick);

    // Guarda a cor do pixel atual
    int corPixelArea[4];


    // Define a cor interna
//  glColor3f(rSelec, gSelec, bSelec);
    glColor3f(1.0, 0.0, 0.0);



    // Itera sob a parte acima do pixel clicado
    int y2 = y1 + 1;
    while (y2 <= height - 50)
    {
        // Coordenadas limites das linhas superiores
        int leftLimSup = x1, rightLimSup = x1;

        glReadPixels(x1, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);
        if (confereCor(corPixelArea, corPixelClick) == true)
        {

            // Procura pelo limite na esquerda
            while (leftLimSup >= 0)
            {
                glReadPixels(leftLimSup, y2, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    leftLimSup--;
                    continue;
                }

                else
                {

                    // Procura pelo limite na direita
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
                }

                break;
            }

            // Desenha retas acima
            for (int i = leftLimSup; i <= rightLimSup; i++)
            {
                drawPixel(i, y2);
            }

            y2++;
            continue;
        }

        break;
    }


    
    // Itera sob a linha do pixel clicado
    int leftLimClick = x1, rightLimClick = x1;
    while (leftLimClick >= 0)
    {
        glReadPixels(leftLimClick, y1, 1, 1, GL_RGB, GL_INT, corPixelArea);
        if (confereCor(corPixelArea, corPixelClick) == true)
        {
            leftLimClick--;
            continue;
        }
        else
        {
            while (rightLimClick <= width)
            {
                glReadPixels(rightLimClick, y1, 1, 1, GL_RGB, GL_INT, corPixelArea);
                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    rightLimClick++;
                    continue;
                }

                retaBresenham(leftLimClick, y1, rightLimClick, y1);
                break;
            }
        }

        for (int i = leftLimClick; i <= rightLimClick; i++)
        {
            drawPixel(i, y1);
        }

        break;
    }


    
    // Itera sob a parte acima do pixel clicado
    int y0 = y1 - 1;
    while (y0 >= 0)
    {
        // Coordenadas limites das linhas inferiores
        int leftLimInf = x1, rightLimInf = x1;

        glReadPixels(x1, y0, 1, 1, GL_RGB, GL_INT, corPixelArea);
        if (confereCor(corPixelArea, corPixelClick) == true)
        {

            while (leftLimInf >= 0)
            {
                glReadPixels(leftLimInf, y0, 1, 1, GL_RGB, GL_INT, corPixelArea);

                if (confereCor(corPixelArea, corPixelClick) == true)
                {
                    leftLimInf--;
                    continue;
                }

                else
                {

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
                }

                break;
            }

            // Desenha retas abaixo
            for (int i = leftLimInf; i <= rightLimInf; i++)
            {
                drawPixel(i, y0);
            }

            y0--;
            continue;
        }

        break;
    }
    
}


bool confereCor(int corPixelArea[], int corPixelClick[])
{
    return (corPixelArea[0] == corPixelClick[0] && corPixelArea[1] == corPixelClick[1] && corPixelArea[2] == corPixelClick[2]);
}



