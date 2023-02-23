/*
 * Atividade 15 - Computacao Grafica
 * Editor Grafico feito com OpenGL/GLUT
 * Autor: Enzo Eduardo Cassiano Ibiapina
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
using namespace std;





/*
 * Declaracao de constantes e variaveis globais
*/

// Teclas do teclado e seus valores ASCII
#define ESC 27

// Enumeracao com os tipos de formas geometricas
enum tipos_formas
{
    MOU = 0,
    LIN,        // Linha
    RET,        // Retangulo
    TRI,        // Triangulo
    POL,        // Poligono
    CIR,        // Circulo
    BAL,        // Balde de tinta (Flood Fill)
    RETc,       // Retangulo      (Rasterizado)
    TRIc,       // Triangulo      (Rasterizado)
    POLc        // Poligono       (Rasterizado)
};

// Enumeracao com os tipos de transformacoes geometricas
enum tipos_transformacoes
{
    TRAN = 10,
    ESCA,
    CISA,
    REFL,
    ROTA
};

// Verifica se foi realizado o primeiro clique do mouse
bool click1 = false;

// Coordenadas da posicao atual do mouse
int m_x, m_y;

// Coordenadas do primeiro clique e do segundo clique do mouse
int mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2;

// Largura e altura da janela (variaveis globais de todo o projeto)
int width = 512, height = 512;

// Indica o tipo de forma geometrica ativa para desenhar
int modoForma = MOU, modoTransf = 0;

// Contador de vertices?
int contCoordenadas = 0;

// Cor selecionada pro desenho
float rSelec = 0.0, gSelec = 0.0, bSelec = 0.0;





// Headers do arquivo
#include "estruturas.h"
#include "algoritmos.h"
#include "transformacoes.h"
#include "extras.h"
#include "glut_text.h"





// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo, int tipoadd){
    forma f;
    f.tipo = tipo;
    f.rCor = rSelec; f.gCor = gSelec; f.bCor = bSelec;
    if (tipoadd == 0) {
        formas.push_front(f);
    } else {
        formas.reverse();
        formas.push_front(f);
        formas.reverse();
    }
}

// Funcao para armazenar um vertice na forma do inicio da lista de formas geometricas
// Armazena sempre no inicio da lista
void pushVertice(int x, int y, int tipoadd){
    vertice v;
    v.x = x;
    v.y = y;
    if (tipoadd == 0) {
        formas.front().v.push_front(v);
    } else {
        formas.reverse();
        formas.front().v.push_front(v);
        formas.reverse();
    }
}

// Funcao para armazenar uma transformacao na forma
void pushTransformacao(int tipo, float valores[2])
{
    transformacao tr;
    tr.tipo = tipo;

    for (int i = 0; i < 2; i++)
    {
        tr.vtf[i] = valores[i];
    }

    // Adiciona a transformacao em todas as formas desenhadas ate aquele momento (?)
    // Testa se novas formas sao afetadas ou nao
    for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++)
    {
        f->t.push_front(tr);
    }
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

// Funcao que trata da mudanca de estado de desenho
void verificaCliqueBotao(int mouseX, int mouseY);

// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();

// Funcao que percorre a lista de transformacoes geometricas de cada forma, aplicando-as nas coordenadas
void aplicaTransformacao(float *desenhoX, float *desenhoY, forward_list<transformacao>::iterator tr);





/*
 * Funcao principal
*/
int main(int argc, char** argv){

    glutInit(&argc, argv);                          // Passagem de parametros C para o glut
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);    // Selecao do modo do Display e do sistema de Cor utilizado
    glutInitWindowSize(width, height);              // Tamanho da janela do OpenGL
    glutInitWindowPosition(700, 100);               // Posicao inicial da janela do OpenGL
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

            switch(modoForma){

                case MOU:
                    // Clique
                    if (state == GLUT_UP)
                    {
                        mouseClick_x1 = x;
                        mouseClick_y1 = height - y - 1;
                        printf("Clique 1(%d, %d)\n",mouseClick_x1,mouseClick_y1);

                        switch (modoTransf)
                        {
                            case 0:
                                {
                                    verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                }
                            break;

                            case TRAN:
                                {
                                    if (mouseClick_y1 <= height - 50)
                                    {
                                        float vetorDeslocamento[2];
                                        printf("Digite os valores de x e y: ");
                                        scanf("%f %f", &vetorDeslocamento[0], &vetorDeslocamento[1]);

                                        pushTransformacao(TRAN, vetorDeslocamento);
                                        printf("ADICIONOU!!!!!!\n");
                                    }
                                    else
                                    {
                                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                    }
                                }
                            break;
                            
                            case ESCA:
                                {
                                    if (mouseClick_y1 <= height - 50)
                                    {
                                        float vetorDeslocamento[2];
                                        printf("Digite os valores de x e y: ");
                                        scanf("%f %f", &vetorDeslocamento[0], &vetorDeslocamento[1]);

                                        pushTransformacao(ESCA, vetorDeslocamento);
                                        printf("ADICIONOU!!!!!!\n");
                                    }
                                    else
                                    {
                                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                    }
                                }
                            break;
                            
                            case CISA:
                                {
                                    if (mouseClick_y1 <= height - 50)
                                    {
                                        char dados[2];
                                        printf("Digite o eixo e o valor de deslocamento (ex: \"x 3\"): ");
                                        scanf(" %c %c", &dados[0], &dados[1]);

                                        float vetorDeslocamento[2];
                                        vetorDeslocamento[0] = (float) dados[0];
                                        vetorDeslocamento[1] = (float) dados[1];

                                        pushTransformacao(CISA, vetorDeslocamento);
                                        printf("ADICIONOU!!!!!!\n");
                                    }
                                    else
                                    {
                                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                    }
                                }
                            break;
                            
                            case REFL:
                                {
                                    if (mouseClick_y1 <= height - 50)
                                    {
                                        char eixo;
                                        printf("Digite o eixo (\"x\", \"y\" ou \"0\"): ");
                                        scanf(" %c", &eixo);

                                        float vetorDeslocamento[2];
                                        vetorDeslocamento[0] = (float) eixo;
                                        vetorDeslocamento[1] = 0;

                                        pushTransformacao(REFL, vetorDeslocamento);
                                        printf("ADICIONOU!!!!!!\n");
                                    }
                                    else
                                    {
                                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                    }
                                }
                            break;
                            
                            case ROTA:
                                {
                                    if (mouseClick_y1 <= height - 50)
                                    {
                                        int grauRotacao;
                                        printf("Digite o grau de rotacao: ");
                                        scanf("%d", &grauRotacao);

                                        // Converte graus em radianos
                                        // (funcoes trig da math.h trabalham com radianos)
                                        float vetorDeslocamento[2];
                                        
                                        vetorDeslocamento[0] = grauRotacao * M_PI / 180.0;
                                        vetorDeslocamento[1] = 0;

                                        pushTransformacao(ROTA, vetorDeslocamento);
                                        printf("ADICIONOU!!!!!!\n");
                                    }
                                    else
                                    {
                                        verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                                    }
                                }
                            break;
                        }
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

                                pushForma(modoForma, 0);
                                pushVertice(mouseClick_x1, mouseClick_y1, 0);
                                pushVertice(mouseClick_x2, mouseClick_y2, 0);
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
                                
                                pushForma(modoForma, 0);
                                pushVertice(mouseClick_x1, mouseClick_y2, 0);
                                pushVertice(mouseClick_x2, mouseClick_y2, 0);
                                pushVertice(mouseClick_x2, mouseClick_y1, 0);
                                pushVertice(mouseClick_x1, mouseClick_y1, 0);
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
                                    pushVertice(mouseClick_x2, mouseClick_y2, 0);
                                    printf("Clique 2(%d, %d)\n", mouseClick_x2, mouseClick_y2);
                                }
                                else
                                {
                                    click1 = false;
                                    contCoordenadas = 0;
                                    pushVertice(mouseClick_x2, mouseClick_y2, 0);
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
                                pushForma(modoForma, 0);
                                pushVertice(mouseClick_x1, mouseClick_y1, 0);
                                printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }
                        }
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
                                        pushVertice(mouseClick_x2, mouseClick_y2, 0);
                                    }
                                    else
                                    {
                                        contCoordenadas++;
                                        pushVertice(mouseClick_x2, mouseClick_y2, 0);
                                        printf("Clique %d(%d, %d)\n", contCoordenadas, mouseClick_x2, mouseClick_y2);
                                    }
                                }
                                else
                                {
                                    contCoordenadas++;
                                    pushVertice(mouseClick_x2, mouseClick_y2, 0);
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
                                pushForma(modoForma, 0);
                                pushVertice(mouseClick_x1, mouseClick_y1, 0);
                                printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }
                        }
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

                                pushForma(modoForma, 0);
                                pushVertice(mouseClick_x1, mouseClick_y1, 0);
                                pushVertice(mouseClick_x2, mouseClick_y2, 0);
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
                                pushForma(BAL, 1);
                                pushVertice(mouseClick_x1, mouseClick_y1, 1);
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }
                    }
                break;

                case RETc:
                    if (state == GLUT_UP)
                    {
                        if (click1 == false)
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                //
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }
                    }
                break;

                case TRIc:
                    if (state == GLUT_UP)
                    {
                        if (click1 == false)
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                //
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }
                    }
                break;

                case POLc:
                    if (state == GLUT_UP)
                    {
                        if (click1 == false)
                        {
                            mouseClick_x1 = x;
                            mouseClick_y1 = height - y - 1;

                            if (mouseClick_y1 <= height - 50)
                            {
                                //
                            }
                            else
                            {
                                verificaCliqueBotao(mouseClick_x1, mouseClick_y1);
                            }

                            printf("Clique 1(%d, %d)\n", mouseClick_x1, mouseClick_y1);
                        }
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


    // Carrega a GUI do app
    desenhaGUI();

    // Desenha as formas geometricas da lista
    drawFormas();


    /*
    //Desenha texto com as coordenadas da posicao do mouse
    draw_text_stroke(0, 0, "(" + to_string(m_x) + "," + to_string(m_y) + ")", 0.2);
    */

    // Troca os buffers
    glutSwapBuffers();
}


/*
 * Controla o menu pop-up
 */
void menu_popup(int value){
    if (value == 0) exit(EXIT_SUCCESS);
    modoForma = value;
}





void verificaCliqueBotao(int mouseX, int mouseY)
{
    /* ====== Tratamento dos botoes de formas ====== */
    bool clicouForma = false;

        // Botao MOU
        if (mouseX > 70 && mouseX < 90)
        {
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != MOU)
            {
                printf("CLIQUE!!!!!\n");
                modoForma = MOU;
                clicouForma = true;
            }
        }

        // Botao LIN
        else if (mouseX > 90 && mouseX < 110)
        {
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != LIN)
            {
                printf("LINHA!!!!!\n");
                modoForma = LIN;
                clicouForma = true;
            }
        }

        else if (mouseX > 110 && mouseX < 130)
        {
            // Botao RET
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != RET)
            {
                printf("RETANGULO!!!!!\n");
                modoForma = RET;
                clicouForma = true;
            }

            // Botao RETc
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != RETc)
            {
                printf("RETANGULO_RASTERIZADO!!!!!\n");
                modoForma = RETc;
                clicouForma = true;
            }
        }

        else if (mouseX > 130 && mouseX < 150)
        {
            // Botao TRI
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != TRI)
            {
                printf("TRIANGULO!!!!!\n");
                modoForma = TRI;
                clicouForma = true;
            }

            // Botao TRIc
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != TRIc)
            {
                printf("TRIANGULO_RASTERIZADO!!!!!\n");
                modoForma = TRIc;
                clicouForma = true;
            }
        }

        else if (mouseX > 150 && mouseX < 170)
        {
            // Botao POL
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != POL)
            {
                printf("POLIGONO!!!!!\n");
                modoForma = POL;
                clicouForma = true;
            }

            // Botao POLc
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != POLc)
            {
                printf("POLIGONO_RASTERIZADO!!!!!\n");
                modoForma = POLc;
                clicouForma = true;
            }
        }

        // Botao CIR
        else if (mouseX > 170 && mouseX < 190)
        {
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != CIR)
            {
                printf("CIRCULO!!!!!\n");
                modoForma = CIR;
                clicouForma = true;
            }
        }

        // Botao BAL
        else if (mouseX > 190 && mouseX < 210)
        {
            if (mouseY >= height-45 && mouseY <= height-25 && modoForma != BAL)
            {
                printf("BALDE!!!!!\n");
                modoForma = BAL;
                clicouForma = true;
            }
        }


    /* ====== Tratamento das colunas de transformacao ====== */
        else if (mouseX > 250 && mouseX < 265)
        {
            if (modoForma == MOU)
            {
                // Botao de translacao
                if (mouseY >= height-17 && mouseY <= height-2 && modoTransf != TRAN)
                {
                    printf("TRANSLACAO!!!!!\n");
                    modoTransf = TRAN;
                }

                // Botao de escala
                else if (mouseY >= height-32 && mouseY <= height-17 && modoTransf != ESCA)
                {
                    printf("ESCALA!!!!!\n");
                    modoTransf = ESCA;
                }

                // Botao de cisalhamento
                else if (mouseY >= height-47 && mouseY <= height-32 && modoTransf != CISA)
                {
                    printf("CISALHAMENTO!!!!!\n");
                    modoTransf = CISA;
                }
            }
        }

        else if (mouseX > 270 && mouseX < 285)
        {
            if (modoForma == MOU)
            {
                // Botao de reflexao
                if (mouseY >= height-17 && mouseY <= height-2 && modoTransf != REFL)
                {
                    printf("REFLEXAO!!!!!\n");
                    modoTransf = REFL;
                }

                // Botao de rotacao
                else if (mouseY >= height-32 && mouseY <= height-17 && modoTransf != ROTA)
                {
                    printf("ROTACAO!!!!!\n");
                    modoTransf = ROTA;
                }
            }
        }


    // Tratamento das barras RGB
        else if (mouseX >= width-90 && mouseX <= width-10)
        {
            // Barra de cor R
            if (mouseY > height-15 && mouseY < height-5)
            {
                float corEscolhida[3];
                glReadPixels(mouseX, mouseY, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);

                rSelec = corEscolhida[0];
            }

            // Barra de cor G
            else if (mouseY > height-30 && mouseY < height-20)
            {
                float corEscolhida[3];
                glReadPixels(mouseX, mouseY, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);

                gSelec = corEscolhida[1];
            }

            // Barra de cor B
            else if (mouseY >= height-45 && mouseY <= height-35)
            {
                float corEscolhida[3];
                glReadPixels(mouseX, mouseY, 1, 1, GL_RGB, GL_FLOAT, corEscolhida);

                bSelec = corEscolhida[2];
            }
        }


    // Reinicia variaveis em caso de clique de forma
    if (clicouForma == true)
    {
        contCoordenadas = 0;
        modoTransf = 0;
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
void drawFormas()
{
    // Altera a cor das linhas conforme durante o processo de desenho
    if (contCoordenadas == 0)
    {
        // Desenho concluido = usa a cor escolhida
        glColor3f(rSelec, gSelec, bSelec);
    }
    else
    {
        // Desenho em progresso = cor vermelha
        glColor3f(1.0, 0.0, 0.0);
    }


    // Preview da forma a ser desenhada
    if (click1 == true)
    {
        switch(modoForma)
        {
            case LIN:
                {
                    forward_list<vertice> v = retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                    for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                        drawPixel(it_v->x, it_v->y);
                    }
                }
            break;

            case RET:
                {
                    forward_list<vertice> v;
                    for (int k = 0; k < 4; k++)
                    {
                        switch (k)
                        {
                            case 0: v = retaBresenham(mouseClick_x1, mouseClick_y1, mouseClick_x1, m_y);
                            break;
                            case 1: v = retaBresenham(mouseClick_x1, mouseClick_y1, m_x, mouseClick_y1);
                            break;
                            case 2: v = retaBresenham(mouseClick_x1, m_y, m_x, m_y);
                            break;
                            case 3: v = retaBresenham(m_x, mouseClick_y1, m_x, m_y);
                            break;
                        }

                        for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                            drawPixel(it_v->x, it_v->y);
                        }
                    }
                }
            break;

            case TRI:
                {
                    forward_list<vertice> v;
                    if (contCoordenadas == 1)
                    {
                        v = retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                        for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                            drawPixel(it_v->x, it_v->y);
                        }
                    }
                    else if (contCoordenadas == 2)
                    {
                        for (int k = 0; k < 3; k++)
                        {
                            switch (k)
                            {
                                case 0: v = retaBresenham(mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2);
                                break;
                                case 1: v = retaBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                                break;
                                case 2: v = retaBresenham(m_x, m_y, mouseClick_x2, mouseClick_y2);
                                break;
                            }

                            for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                                drawPixel(it_v->x, it_v->y);
                            }
                        }
                    }
                }
            break;

            case POL:
                {
                    forward_list<forma>::iterator f = formas.begin();

                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    float x[numVertices], y[numVertices];

                    // Salva todas as coordenadas
                    int i = 0;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Muda a cor das linhas ao passar pelo vertice inicial
                    if (m_x == x[numVertices-1] && m_y == y[numVertices-1])
                    {
                        glColor3f(0.0, 0.0, 1.0);
                    }

                    // Preview do poligono atual
                    forward_list<vertice> v;
                    for (int j = 0; j < numVertices; j++)
                    {
                        if (j != numVertices-1)
                        {
                            v = retaBresenham(x[j], y[j], x[j+1], y[j+1]);
                        }
                        
                        if (j == 0)
                        {
                            v = retaBresenham(x[j], y[j], m_x, m_y);
                        }

                        for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                            drawPixel(it_v->x, it_v->y);
                        }
                    }
                }
            break;

            case CIR:
                {
                    forward_list<vertice> v = circuloBresenham(mouseClick_x1, mouseClick_y1, m_x, m_y);
                    for (forward_list<vertice>::iterator it_v = v.begin(); it_v != v.end(); it_v++) {
                        drawPixel(it_v->x, it_v->y);
                    }
                }
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
                    float x[2], y[2];

                    // Salva as coordenadas de cada vertice
                    int i = 0;
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                    {
                        aplicaTransformacao(&x[0], &y[0], tr);
                        aplicaTransformacao(&x[1], &y[1], tr);
                    }

                    // Desenha o segmento de reta
                    forward_list<vertice> vertices = retaBresenham(x[0], y[0], x[1], y[1]);
                    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                    {
                        float desenhoX = v->x;
                        float desenhoY = v->y;
                        drawPixel(desenhoX, desenhoY);
                    }
                }
            break;

            case RET:
                {
                    // Listas com os x e y de cada vertice da forma
                    float x[4], y[4];

                    // Salva as coordenadas de cada vertice
                    int i = 0;
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (int k = 0; k < 4; k++)
                    {
                        for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                        {
                            aplicaTransformacao(&x[k], &y[k], tr);
                        }
                    }

                    // Desenha o retangulo
                    forward_list<vertice> vertices;
                    for (int k = 0; k < 4; k++)
                    {
                        if (k != 3)
                        {
                            vertices = retaBresenham(x[k], y[k], x[k+1], y[k+1]);
                        }
                        else
                        {
                            vertices = retaBresenham(x[k], y[k], x[0], y[0]);
                        }

                        for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                        {
                            float desenhoX = v->x;
                            float desenhoY = v->y;
                            drawPixel(desenhoX, desenhoY);
                        }
                    }
                }
            break;
            
            case TRI:
                {
                    // Listas com os x e y de cada vertice da forma
                    float x[3], y[3];

                    // Salva as coordenadas de cada vertice
                    int i = 0;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++) {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (int k = 0; k < 3; k++)
                    {
                        for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                        {
                            aplicaTransformacao(&x[k], &y[k], tr);
                        }
                    }

                    // Desenha o triangulo
                    if (i == 3)
                    {
                        forward_list<vertice> vertices;
                        for (int j = 0; j < i; j++)
                        {
                            if (j != 2)
                            {
                                vertices = retaBresenham(x[j], y[j], x[j+1], y[j+1]);
                            }
                            else
                            {
                                vertices = retaBresenham(x[j], y[j], x[0], y[0]);
                            }

                            for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                            {
                                float desenhoX = v->x;
                                float desenhoY = v->y;
                                drawPixel(desenhoX, desenhoY);
                            }
                        }
                    }
                }
            break;
            
            case POL:
                {
                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    float x[numVertices], y[numVertices];
                    
                    // Salva as coordenadas de cada vertice
                    int i = 0;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (int k = 0; k < numVertices; k++)
                    {
                        for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                        {
                            aplicaTransformacao(&x[k], &y[k], tr);
                        }
                    }

                    // Desenha o poligono
                    forward_list<vertice> vertices;
                    for (int j = 0; j < numVertices-1; j++)
                    {
                        vertices = retaBresenham(x[j], y[j], x[j+1], y[j+1]);

                        for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                        {
                            float desenhoX = v->x;
                            float desenhoY = v->y;
                            drawPixel(desenhoX, desenhoY);
                        }
                    }
                }
            break;

            case CIR:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0;
                    float x[2], y[2];

                    // Salva as coordenadas de cada vertice da forma
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (int k = 0; k < 2; k++)
                    {
                        for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                        {
                            aplicaTransformacao(&x[k], &y[k], tr);
                        }
                    }

                    // Desenha a circunferencia
                    forward_list<vertice> vertices = circuloBresenham(x[1], y[1], x[0], y[0]);
                    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                    {
                        float desenhoX = v->x;
                        float desenhoY = v->y;
                        drawPixel(desenhoX, desenhoY);
                    }
                }
            break;

            case BAL:
                {
                    // Coordenadas x e y do clique inicial
                    float x, y;

                    // Salva as coordenadas do vertice do pixel clicado
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++){
                        x = v->x;
                        y = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                    {
                        aplicaTransformacao(&x, &y, tr);
                        aplicaTransformacao(&x, &y, tr);
                    }

                    // Colore a area
                    forward_list<vertice> vertices = algoritmoFloodFill(x, y);
                    for (forward_list<vertice>::iterator v = vertices.begin(); v != vertices.end(); v++)
                    {
                        float desenhoX = v->x;
                        float desenhoY = v->y;
                        drawPixel(desenhoX, desenhoY);
                    }
                }
            break;
            /*
            case RETc:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0
                    float x[4], y[4];

                    // Salva as coordenadas de cada vertice
                    for(forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++){
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Aplica as transformacoes armazenadas na forma
                    for (int k = 0; k < 4; k++)
                    {
                        for (forward_list<transformacao>::iterator tr = f->t.begin(); tr != f->t.end(); tr++)
                        {
                            aplicaTransformacao(&x[k], &y[k], tr);
                        }
                    }

                    // Desenha o retangulo
                    //
                }
            break;
            */
            default:
            break;
        }
    }
}


void aplicaTransformacao(float *desenhoX, float *desenhoY, forward_list<transformacao>::iterator tr)
{
    // Aplica a transformacao geometrica com base no tipo
    switch (tr->tipo)
    {
        case TRAN:
            {
                translacao(desenhoX, desenhoY, tr->vtf[0], tr->vtf[1]);
            }
        break;

        case ESCA:
            {
                escala(desenhoX, desenhoY, tr->vtf[0], tr->vtf[1]);
            }
        break;

        case CISA:
            {
                cisalhamento(desenhoX, desenhoY, tr->vtf[0], tr->vtf[1]);
            }
        break;

        case REFL:
            {
                reflexao(desenhoX, desenhoY, tr->vtf[0]);
            }
        break;

        case ROTA:
            {
                float centroideX = width/2, centroideY = height/2;

                translacao(desenhoX, desenhoY, -centroideX, -centroideY);
                rotacao(desenhoX, desenhoY, tr->vtf[0]);
                translacao(desenhoX, desenhoY, centroideX, centroideY);
            }
        break;
    }
}
