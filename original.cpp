#include <windows.h>
#include <GL/glut.h>
#include <cstdio>
#include <cstdlib>
#include <cmath>
#include "glaux.h"

/* Global variables */
char title[] = "Esponga de Menger.";
int n = 0;
int tamanhoBase;
bool luz1Ligada, luz2Ligada, luz3Ligada;
GLuint vetorTexturas;
GLfloat aspect, olhoCameraX = 0, olhoCameraY = 0, olhoCameraZ = 0, luz1PosicaoX = 0, luz1PosicaoY = 0,
                luz1PosicaoZ = 0, luz2PosicaoX = 0, luz2PosicaoY = 0, luz2PosicaoZ = 0, luz3PosicaoX = 0,
                luz3PosicaoY = 0, luz3PosicaoZ = 0;

//Função para Carregar uma imagem .BMP
AUX_RGBImageRec *LoadBMP(char *Filename)
{
    FILE *File=NULL;
    if (!Filename)
    {
        return NULL;
    }
    File=fopen(Filename,"r");
    if (File)	// Se o arquivo existe
    {
        fclose(File);
        return auxDIBImageLoad(Filename);//Retorna a imagem
    }
    return NULL;
}

void gerarTextura(int iter)
{
    AUX_RGBImageRec *textura;
    char *nomeArquivo;
    switch(iter)
    {
    case 1:
        nomeArquivo = "textura1.bmp";
        break;
    case 2:
        nomeArquivo = "textura2.bmp";
        break;
    case 3:
        nomeArquivo = "textura3.bmp";
        break;
    default:
        break;
    }
    if(textura = LoadBMP(nomeArquivo))
    {
        glGenTextures(1, &vetorTexturas);
        glEnable(GL_TEXTURE_GEN_S); //enable texture coordinate generation
        glEnable(GL_TEXTURE_GEN_T);
        glBindTexture(GL_TEXTURE_2D, vetorTexturas);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
        glTexImage2D(GL_TEXTURE_2D, 3, GL_RGB, textura->sizeX, textura->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE, textura->data);
    }
}

int exponencial(int base, int expoente)
{
    int retorno = 1;
    for(int i = 0; i < expoente; i++)
    {
        retorno *= base;
    }
    return retorno;
}

void desenharCubo(GLfloat translacaoX, GLfloat translacaoY, GLfloat translacaoZ, GLfloat tamanho)
{
    glPushMatrix();
    glColor3f(1,0,0);
    glTranslatef(translacaoX, translacaoY, translacaoZ);
    glutSolidCube(tamanho);
    glPopMatrix();

}

void cortarEsponja(int iter, GLfloat tamanhoCubo)
{
    for(int i = 0; i < tamanhoBase; i++)
    {
        for(int j = 0; j < tamanhoBase; j++)
        {
            for(int k = 0; k < tamanhoBase; k++)
            {
                if(!(i%3==j%3 && i%3==1 || i%3==k%3 && i%3==1 || k%3==j%3 && k%3==1))
                {
                    {
                        desenharCubo(i, j, k, tamanhoCubo);
                    }
                }
            }
        }
    }
}

void carregarLuz()
{
    if(luz1Ligada)
    {
        GLfloat luz_0_posicao[] = { -3+luz1PosicaoX, -5+luz1PosicaoY, 1+luz1PosicaoZ, 1 };
        GLfloat luz_0_difusa[] = { 0, 0, 1 };
        GLfloat luz_0_ambiente[] = { 1, 1, 1 };

        glLightfv(GL_LIGHT0, GL_POSITION, luz_0_posicao);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_0_difusa);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luz_0_ambiente);

        glEnable(GL_LIGHT0);

    }
    else{
        glDisable(GL_LIGHT0);
    }
    if(luz2Ligada)
    {
        GLfloat luz_1_posicao[] = { 10+luz2PosicaoX, 4+luz2PosicaoY, 1+luz2PosicaoZ, 0.5 };
        GLfloat luz_1_difusa[] = { 0.4, 0, 1 };
        GLfloat luz_1_ambiente[] = { 0.2, 0.2, 0.2 };

        glLightfv(GL_LIGHT1, GL_POSITION, luz_1_posicao);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_1_difusa);
        glLightfv(GL_LIGHT1, GL_AMBIENT, luz_1_ambiente);

        glEnable(GL_LIGHT1);
    }
    else{
        glDisable(GL_LIGHT1);
    }
    if(luz3Ligada)
    {
        GLfloat luz_2_posicao[] = { 0+luz3PosicaoX, -1.5+luz3PosicaoY, 0+luz3PosicaoZ, 0.7 };
        GLfloat luz_2_difusa[] = { 0.5, 0.5, 0.7 };
        GLfloat luz_2_ambiente[] = { 0.1, 0.1, 0.1 };

        glLightfv(GL_LIGHT2, GL_POSITION, luz_2_posicao);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_2_difusa);
        glLightfv(GL_LIGHT2, GL_AMBIENT, luz_2_ambiente);

        glEnable(GL_LIGHT2);
    }
    else{
        glDisable(GL_LIGHT2);
    }
    glEnable(GL_LIGHTING);
}


void movimentaCamera(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        olhoCameraY += 1;
        break;
    case GLUT_KEY_DOWN:
        olhoCameraY -= 1;
        break;
    case GLUT_KEY_LEFT:
        olhoCameraX -= 1;
        break;
    case GLUT_KEY_RIGHT:
        olhoCameraX += 1;
        break;
    }
    glutPostRedisplay();
}


void teclado(unsigned char tecla, int x, int y)
{
    switch(tecla)
    {
    case '+':
        if(n < 4)
        {

            n++;
            tamanhoBase = exponencial(3,n);
            cortarEsponja(n, 1);
        }
        break;
    case '-':
        if(n > 0)
        {
            n--;
            tamanhoBase = exponencial(3,n);
            cortarEsponja(n, 1);
        }
        break;
    case 'a':
    case 'A':
        if(!luz1Ligada)
        {
            luz1Ligada = true;
        }
        else
        {
            luz1Ligada = false;
        }
        break;
    case 's':
    case 'S':
        if(!luz2Ligada)
        {
            luz2Ligada = true;
        }
        else
        {
            luz2Ligada = false;
        }
        break;
    case 'd':
    case 'D':
        if(!luz3Ligada)
        {
            luz3Ligada = true;
        }
        else
        {
            luz3Ligada = false;
        }
        break;
    case 'q':
    case 'Q':
        if(luz1PosicaoX > 50)
        {
            luz1PosicaoX -= 2;
        }
        else if(luz1PosicaoX > -50)
        {
            luz1PosicaoX += 2;
        }
        break;
    case 'w':
    case 'W':
        if(luz2PosicaoX > 50)
        {
            luz2PosicaoX -= 2;
        }
        else if(luz2PosicaoX > -50)
        {
            luz2PosicaoX += 2;
        }
        break;
    case 'e':
    case 'E':
        if(luz3PosicaoX > 50)
        {
            luz3PosicaoX -= 2;
        }
        else if(luz3PosicaoX > -50)
        {
            luz3PosicaoX += 2;
        }
        break;
        break;
    case 27:
        exit(0);
        break;
    default:
        break;
    }
    glutPostRedisplay();
}


/* Handler for window-repaint event. Called back when the window first appears and
   whenever the window needs to be re-painted. */
void display()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // Clear color and depth buffers
    carregarLuz();
    gerarTextura(n);
    cortarEsponja(n, 1);
    glutSwapBuffers();  // Swap the front and back frame buffers (double buffering)

}

void reshape(GLsizei width, GLsizei height)    // GLsizei for non-negative integer
{
    // Compute aspect ratio of the new window
    if (height == 0) height = 1;                // To prevent divide by 0
    aspect = (GLfloat)width / (GLfloat)height;

    // Set the viewport to cover the new window
    glViewport(0, 0, width, height);

    // Set the aspect ratio of the clipping volume to match the viewport
    glMatrixMode(GL_PROJECTION);  // To operate on the Projection matrix
    glLoadIdentity();             // Reset
    // Enable perspective projection with fovy, aspect, zNear and zFar
    gluPerspective(90.0f, aspect, 0.001f, 1000.0f);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0+olhoCameraX, 0+olhoCameraY, -15+olhoCameraZ, 0, 0, 0, 1,0,0);
}

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

/* Main function: GLUT runs as a console application starting at main() */
int main(int argc, char** argv)
{
    glutInit(&argc, argv);            // Initialize GLUT
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_DEPTH); // Enable double buffered mode
    glutInitWindowSize(640, 480);   // Set the window's initial width & height
    glutInitWindowPosition(50, 50); // Position the window's initial top-left corner
    glutCreateWindow(title);          // Create window with the given title
    glutDisplayFunc(display);       // Register callback handler for window re-paint event
    glutReshapeFunc(reshape);
    glutKeyboardFunc(teclado);
    glutSpecialFunc(movimentaCamera);
    initGL();
    glutMainLoop();                 // Enter the infinite event-processing loop
    return 0;
}
