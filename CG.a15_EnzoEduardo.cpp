/*
 * Atividade 15 - Computacao Grafica
 * Editor Grafico feito com OpenGL/GLUT
 * Autor: Enzo Eduardo Cassiano Ibiapina
 * Data: 26/02/2023
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
    RETp,       // Retangulo      (Preenchido)
    TRIp,       // Triangulo      (Preenchido)
    POLp        // Poligono       (Preenchido)
};

// Enumeracao com os tipos de transformacoes geometricas
enum tipos_transformacoes
{
    TRAN = 10,  // Translacao
    ESCA,       // Escala
    CISA,       // Cisalhamento
    REFL,       // Reflexao
    ROTA        // Rotacao
};

// Indica a opcao de modo de claridade do editor
bool nightMode = false;

// Verifica se foi realizado o primeiro clique do mouse
bool click1 = false;

// Largura e altura da janela
int width = 512, height = 512;

// Indica o tipo de forma geometrica ativa para desenhar
int modoForma = MOU, modoTransf = 0;

// Contador de vertices
int contCoordenadas = 0;

// Coordenadas da posicao atual do mouse
int m_x, m_y;

// Coordenadas do primeiro clique e do segundo clique do mouse
int mouseClick_x1, mouseClick_y1, mouseClick_x2, mouseClick_y2;

// Cor selecionada pro desenho
float rSelec, gSelec, bSelec;





// Headers do arquivo
#include "estruturas.h"
#include "algoritmos.h"
#include "formas.h"
#include "transformacoes.h"
#include "glut_text.h"
#include "extras.h"





// Lista encadeada de formas geometricas
forward_list<forma> formas;

// Funcao para armazenar uma forma geometrica na lista de formas
// Armazena sempre no inicio da lista
void pushForma(int tipo, int tipoadd)
{
    forma f;
    f.tipo = tipo;
    f.rCor = rSelec; f.gCor = gSelec; f.bCor = bSelec;

    // Adiciona no inicio da lista se for uma forma
    // Adiciona no final se for um desenho pelo flood fill
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
void pushVertice(int x, int y, int tipoadd)
{
    vertice v;
    v.x = x;
    v.y = y;

    // Adiciona na forma no topo se pertencer a uma forma apenas com contorno
    // Adiciona no fim se pertencer a um desenho pelo flood fill
    if (tipoadd == 0) {
        formas.front().v.push_front(v);
    } else {
        formas.reverse();
        formas.front().v.push_front(v);
        formas.reverse();
    }
}

// Funcao para armazenar uma transformacao em cada forma da lista
void pushTransformacao(int tipo, double valores[2])
{
    transformacao tr;
    tr.tipo = tipo;

    for (int i = 0; i < 2; i++)
    {
        tr.vtf[i] = valores[i];
    }

    // Adiciona a transformacao em todas as formas desenhadas ate aquele momento
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

// Funcao que trata da mudanca de estado de desenho
void verificaCliqueBotao(int mouseX, int mouseY);

// Funcao que percorre a lista de formas geometricas, desenhando-as na tela
void drawFormas();

// Funcao que percorre a lista de transformacoes geometricas de cada forma, aplicando-as nas coordenadas
void aplicaTransformacao(double *desenhoX, double *desenhoY, forward_list<transformacao>::iterator tr);





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

    glutMainLoop();                                 // Executa o loop do OpenGL

    return EXIT_SUCCESS;                            // Retorna 0 para o tipo inteiro da funcao main()
}





/*
 * Inicializa alguns parametros do GLUT
 */
void init(void)
{
    glClearColor(1.0, 1.0, 1.0, 1.0);           // Limpa a tela com a cor branca
    rSelec = 0.0; gSelec = 0.0; bSelec = 0.0;   // Cor selecionada pra desenho inicialmente preta

    // Cria o menu reserva (outra opcao caso os botoes de desenho quebrem)
    int menuFormas = glutCreateMenu(menu_popup);
    glutAddMenuEntry("Linha"               , LIN);
    glutAddMenuEntry("Retangulo"           , RET);
    glutAddMenuEntry("Triangulo"           , TRI);
    glutAddMenuEntry("Poligono"            , POL);
    glutAddMenuEntry("Circunferencia"      , CIR);
    glutAddMenuEntry("Balde de tinta"      , BAL);
    glutAddMenuEntry("Retangulo preenchido", RETp);
    glutAddMenuEntry("Triangulo preenchido", TRIp);
    glutAddMenuEntry("Poligono preenchido" , POLp);

    int menuTransf = glutCreateMenu(menu_popup);
    glutAddMenuEntry("Translacao"          , TRAN);
    glutAddMenuEntry("Escala"              , ESCA);
    glutAddMenuEntry("Cisalhamento"        , CISA);
    glutAddMenuEntry("Reflexao"            , REFL);
    glutAddMenuEntry("Rotacao"             , ROTA);

    glutCreateMenu(menu_popup);
    glutAttachMenu(GLUT_RIGHT_BUTTON);
    glutAddSubMenu("Formas"        , menuFormas);
    glutAddSubMenu("Transformacoes", menuTransf);
    glutAddMenuEntry("Sair", -1);
}


/*
 * Controla o menu pop-up
 */
void menu_popup(int value){
    if (value == -1) exit(EXIT_SUCCESS);
    modoForma = value;
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
}


/*
 * Controle das teclas comuns do teclado
 */
void keyboard(unsigned char key, int x, int y)
{
    switch (key)
    {
        case ESC: exit(EXIT_SUCCESS); break;
    }
}


/*
 * Controle dos botoes do mouse
 */
void mouse(int button, int state, int x, int y)
{
    switch (button)
    {
        case GLUT_LEFT_BUTTON:

            switch(modoForma)
            {
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
                                        double vetorDeslocamento[2];
                                        printf("Digite os valores de translacao para x e y: ");
                                        scanf("%lf %lf", &vetorDeslocamento[0], &vetorDeslocamento[1]);

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
                                        double vetorDeslocamento[2];
                                        printf("Digite os valores de escala para x e y: ");
                                        scanf("%lf %lf", &vetorDeslocamento[0], &vetorDeslocamento[1]);

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
                                        printf("Digite o eixo e o valor de cisalhamento (ex: \"x 3\"): ");
                                        scanf(" %c %c", &dados[0], &dados[1]);

                                        double vetorDeslocamento[2];
                                        vetorDeslocamento[0] = (double) dados[0];
                                        vetorDeslocamento[1] = (double) dados[1];

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
                                        printf("Digite o eixo de reflexao (\"x\", \"y\" ou \"0\"): ");
                                        scanf(" %c", &eixo);

                                        double vetorDeslocamento[2];
                                        vetorDeslocamento[0] = (double) eixo;
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
                                        double vetorDeslocamento[2];
                                        
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

                case RETp:
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

                case TRIp:
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

                case POLp:
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
            }

        break;      
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
    desenhaGUI(nightMode);

    // Desenha texto com as coordenadas da posicao do mouse
    desenhaTextos();

    // Desenha as formas geometricas da lista
    drawFormas();


    // Troca os buffers
    glutSwapBuffers();
}




/*
 * Funcao que desenha a lista de formas geometricas
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
        // Desenho em progresso
        if (rSelec > gSelec && rSelec > bSelec)
        {
            glColor3f(1.0, 0.0, 1.0);   // (roxo)
        }
        else
        {
            glColor3f(1.0, 0.0, 0.0);   // (vermelho)
        }
    }


    // Preview da forma a ser desenhada
    if (click1 == true)
    {
        switch(modoForma)
        {
            case LIN:
                {
                    double x[] = {(double) mouseClick_x1, (double) m_x};
                    double y[] = {(double) mouseClick_y1, (double) m_y};
                    
                    // Desenha a preview da linha
                    desenhaLinha(x, y);
                }
            break;

            case RET:
                {
                    double x[] = {(double) mouseClick_x1, (double) mouseClick_x1, (double) m_x, (double) m_x};
                    double y[] = {(double) mouseClick_y1, (double) m_y, (double) m_y, (double) mouseClick_y1};
                    
                    // Desenha a preview do retangulo
                    desenhaRetangulo(x, y);
                }
            break;

            case TRI:
                {
                    if (contCoordenadas == 1)
                    {
                        double x[] = {(double) mouseClick_x1, (double) m_x};
                        double y[] = {(double) mouseClick_y1, (double) m_y};
                        
                        desenhaLinha(x, y);
                    }
                    else if (contCoordenadas == 2)
                    {
                        double x[] = {(double) mouseClick_x1, (double) mouseClick_x2, (double) m_x};
                        double y[] = {(double) mouseClick_y1, (double) mouseClick_y2, (double) m_y};

                        // Desenha a preview do triangulo
                        desenhaTriangulo(x, y);
                    }
                }
            break;

            case POL:
                {
                    forward_list<forma>::iterator f = formas.begin();

                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    double x[numVertices+1], y[numVertices+1];
                    x[0] = (double) m_x;
                    y[0] = (double) m_y;

                    // Salva todas as coordenadas
                    int i = 1;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Muda a cor das linhas ao passar pelo vertice inicial
                    if (m_x == x[numVertices] && m_y == y[numVertices])
                    {
                        if (f->bCor > f->rCor && f->bCor > f-> gCor)
                        {
                            glColor3f(0.0, 1.0, 0.0);   // (verde)
                        }
                        else
                        {
                            glColor3f(0.0, 0.0, 1.0);   // (azul)
                        }
                    }

                    // Desenha a preview do poligono
                    desenhaPoligono(x, y, numVertices+1);
                }
            break;

            case CIR:
                {
                    double x[] = {(double) m_x, (double) mouseClick_x1};
                    double y[] = {(double) m_y, (double) mouseClick_y1};

                    // Desenha a preview da circunferencia
                    desenhaCircunferencia(x, y);
                }
            break;

            case RETp:
                {
                    double x[] = {(double) mouseClick_x1, (double) mouseClick_x1, (double) m_x, (double) m_x};
                    double y[] = {(double) mouseClick_y1, (double) m_y, (double) m_y, (double) mouseClick_y1};
                    
                    // Desenha a preview do retangulo preenchido
                    desenhaRetanguloPreenchido(x, y);
                }
            break;

            case TRIp:
                {
                    if (contCoordenadas == 1)
                    {
                        double x[] = {(double) mouseClick_x1, (double) m_x};
                        double y[] = {(double) mouseClick_y1, (double) m_y};
                        
                        desenhaLinha(x, y);
                    }
                    else if (contCoordenadas == 2)
                    {
                        double x[] = {(double) mouseClick_x1, (double) mouseClick_x2, (double) m_x};
                        double y[] = {(double) mouseClick_y1, (double) mouseClick_y2, (double) m_y};

                        // Desenha a preview do triangulo preenchido
                        desenhaTrianguloPreenchido(x, y);
                    }
                }
            break;

            case POLp:
                {
                    forward_list<forma>::iterator f = formas.begin();

                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    double x[numVertices+1], y[numVertices+1];
                    x[0] = (double) m_x;
                    y[0] = (double) m_y;

                    // Salva todas as coordenadas
                    int i = 1;
                    for (forward_list<vertice>::iterator v = f->v.begin(); v != f->v.end(); v++, i++)
                    {
                        x[i] = v->x;
                        y[i] = v->y;
                    }

                    // Muda a cor das linhas ao passar pelo vertice inicial
                    if (m_x == x[numVertices] && m_y == y[numVertices])
                    {
                        if (f->bCor > f->rCor && f->bCor > f-> gCor)
                        {
                            glColor3f(0.0, 1.0, 0.0);   // (verde)
                        }
                        else
                        {
                            glColor3f(0.0, 0.0, 1.0);   // (azul)
                        }
                    }

                    if (numVertices == 1)
                    {
                        desenhaLinha(x, y);
                    }
                    else
                    {
                        // Desenha a preview do poligono preenchido
                        desenhaPoligonoPreenchido(x, y, numVertices+1);
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
                    double x[2], y[2];

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

                    // Desenha a linha
                    desenhaLinha(x, y);
                }
            break;

            case RET:
                {
                    // Listas com os x e y de cada vertice da forma
                    double x[4], y[4];

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
                    if (i == 4)
                    {
                        desenhaRetangulo(x, y);
                    }
                }
            break;
            
            case TRI:
                {
                    // Listas com os x e y de cada vertice da forma
                    double x[3], y[3];

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
                        desenhaTriangulo(x, y);
                    }
                }
            break;
            
            case POL:
                {
                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    double x[numVertices], y[numVertices];
                    
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
                    desenhaPoligono(x, y, numVertices);
                }
            break;

            case CIR:
                {
                    // Listas com os x e y de cada vertice da forma
                    int i = 0;
                    double x[2], y[2];

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
                    desenhaCircunferencia(x, y);
                }
            break;

            case BAL:
                {
                    // Coordenadas x e y do clique inicial
                    double x, y;

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

                    // Determina a cor do pixel clicado
                    float corAntiga[3];
                    glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, corAntiga);

                    // Determina a cor nova
                    float corNova[] = {f->rCor, f->gCor, f->bCor};

                    // Colore a area
                    desenhaBalde(x, y, corAntiga, corNova);
                }
            break;
            
            case RETp:
                {
                    // Listas com os x e y de cada vertice da forma
                    double x[4], y[4];

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

                    // Desenha o retangulo preenchido
                    if (i == 4)
                    {
                        desenhaRetanguloPreenchido(x, y);
                    }
                }
            break;

            case TRIp:
                {
                    // Listas com os x e y de cada vertice da forma
                    double x[3], y[3];

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

                    // Desenha o triangulo preenchido
                    if (i == 3)
                    {
                        desenhaTrianguloPreenchido(x, y);
                    }
                }
            break;
            
            case POLp:
                {
                    // Conta o numero atual de vertices do poligono
                    int numVertices = distance(f->v.begin(), f->v.end());

                    // Listas com os x e y de cada vertice da forma
                    double x[numVertices], y[numVertices];
                    
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

                    if (numVertices == 2)
                    {
                        desenhaLinha(x, y);
                    }
                    else if (numVertices > 2)
                    {
                        // Muda a cor do poligono temporariamente ao passar pelo vertice inicial
                        if (numVertices >= 4 && m_x == x[numVertices-1] && m_y == y[numVertices-1])
                        {
                            if (f->bCor > f->rCor && f->bCor > f-> gCor)
                            {
                                glColor3f(0.0, 1.0, 0.0);   // (verde)
                            }
                            else
                            {
                                glColor3f(0.0, 0.0, 1.0);   // (azul)
                            }
                        }

                        // Desenha o poligono preenchido
                        desenhaPoligonoPreenchido(x, y, numVertices);
                    }
                }
            break;
        }
    }
}


/*
 * Funcao responsavel por tratar interacoes do mouse com os botoes da GUI
 */
void verificaCliqueBotao(int mouseX, int mouseY)
{
    /* ====== Clique em um botao de forma ====== */
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

            // Botao RETp
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != RETp)
            {
                printf("RETANGULO_RASTERIZADO!!!!!\n");
                modoForma = RETp;
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

            // Botao TRIp
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != TRIp)
            {
                printf("TRIANGULO_RASTERIZADO!!!!!\n");
                modoForma = TRIp;
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

            // Botao POLp
            else if (mouseY >= height-25 && mouseY <= height-5 && modoForma != POLp)
            {
                printf("POLIGONO_RASTERIZADO!!!!!\n");
                modoForma = POLp;
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


    /* ====== Clique em um botao de transformacao ====== */
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


    /* ====== Clique em uma das barras RGB ====== */
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


    /* ====== Clique no botao de claridade ====== */
        else if (mouseX >= 0 && mouseX <= 25)
        {
            if (mouseY >= height-25 && mouseY <= height-1)
            {
                // Alterna a variavel nightMode (muda cores de multiplos desenhos na GUI)
                nightMode = !nightMode;

                if (nightMode == false)
                {
                    // Colore as formas de cor branca para cor preta
                    for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++)
                    {
                        if (f->rCor == 1.0 && f->gCor == 1.0 && f->bCor == 1.0)
                        {
                            f->rCor = 0.0; f->gCor = 0.0; f->bCor = 0.0;
                        }
                    }

                    // Alterna a cor selecionada (se for branca)
                    if (rSelec == 1.0 && gSelec == 1.0 && bSelec == 1.0)
                    {
                        rSelec = 0.0; gSelec = 0.0; bSelec = 0.0;
                    }

                    // Limpa a tela com a cor branca
                    glClearColor(1.0, 1.0, 1.0, 1.0);
                }
                else
                {
                    // Colore as formas de cor preta para cor branca
                    for (forward_list<forma>::iterator f = formas.begin(); f != formas.end(); f++)
                    {
                        if (f->rCor == 0.0 && f->gCor == 0.0 && f->bCor == 0.0)
                        {
                            f->rCor = 1.0; f->gCor = 1.0; f->bCor = 1.0;
                        }
                    }

                    // Alterna a cor selecionada (se for preta)
                    if (rSelec == 0.0 && gSelec == 0.0 && bSelec == 0.0)
                    {
                        rSelec = 1.0; gSelec = 1.0; bSelec = 1.0;
                    }

                    // Limpa a tela com a cor preta
                    glClearColor(0.0, 0.0, 0.0, 0.0);
                }
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
 * Funcao responsavel por aplicar as transformacoes da forma em cada vertice da mesma
 */
void aplicaTransformacao(double *desenhoX, double *desenhoY, forward_list<transformacao>::iterator tr)
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

                switch ((char) tr->vtf[0])
                {
                    case 'x': translacao(desenhoX, desenhoY, 0, *desenhoY + (height - *desenhoY));
                    break;

                    case 'y': translacao(desenhoX, desenhoY, *desenhoX + (width - *desenhoX), 0);
                    break;

                    case '0': translacao(desenhoX, desenhoY, *desenhoX + (width - *desenhoX), *desenhoY + (height - *desenhoY));
                    break;
                }
            }
        break;

        case ROTA:
            {
                double centroideX = width/2, centroideY = height/2;

                translacao(desenhoX, desenhoY, -centroideX, -centroideY);
                rotacao(desenhoX, desenhoY, tr->vtf[0]);
                translacao(desenhoX, desenhoY, centroideX, centroideY);
            }
        break;
    }
}

