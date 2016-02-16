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
bool luz1Ligada = true, luz2Ligada = true, luz3Ligada = true, texturaLigada = true;
GLuint textura1, textura2, textura3;
GLfloat aspect, olhoCameraX = 0, posicaoCameraZ = 0, olhoCameraZ = 0, luz1PosicaoX = 0, luz1PosicaoY = 0,
                luz1PosicaoZ = 0, luz2PosicaoX = 0, luz2PosicaoY = 0, luz2PosicaoZ = 0, luz3PosicaoX = 0,
                luz3PosicaoY = 0, luz3PosicaoZ = 0, lightPos, spotAngle = 10, spotExponent = 2;

void desenhaCuboComTextura(GLfloat x, GLfloat y, GLfloat z)
{
    glEnable(GL_TEXTURE_2D);
    if(n == 1)
    {


        glBindTexture(GL_TEXTURE_2D,textura1);
    }
    else if(n == 2)
    {
        glBindTexture(GL_TEXTURE_2D,textura2);
    }
    else if(n == 3)
    {
        glBindTexture(GL_TEXTURE_2D,textura3);
    }
    glBegin(GL_QUADS);
    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x,y,z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-x,y,z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x,-y,z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x,-y,z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x,y,-z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-x,y,-z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x,-y,-z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x,-y,-z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x,y,-z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-x,y,z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x,-y,z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x,-y,-z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x,-y,-z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x,-y,z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x,y,z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x,y,-z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(x,-y,z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(-x,-y,z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(-x,-y,-z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(x,-y,-z);

    glTexCoord2f(0.0f, 1.0f);
    glVertex3f(-x,y,z);
    glTexCoord2f(1.0f, 1.0f);
    glVertex3f(x,y,z);
    glTexCoord2f(1.0f, 0.0f);
    glVertex3f(x,y,-z);
    glTexCoord2f(0.0f, 0.0f);
    glVertex3f(-x,y,-z);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

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

void gerarTextura()
{
    AUX_RGBImageRec *imagemTextura;

    glGenTextures(1, &textura1);
    if(imagemTextura = LoadBMP("textura1.bmp"))
    {
        glBindTexture(GL_TEXTURE_2D, textura1);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, imagemTextura->sizeX,imagemTextura->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,imagemTextura->data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    glGenTextures(1, &textura2);
    if(imagemTextura = LoadBMP("textura2.bmp"))
    {
        glBindTexture(GL_TEXTURE_2D, textura2);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, imagemTextura->sizeX,imagemTextura->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,imagemTextura->data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
    }
    glGenTextures(1, &textura3);
    if(imagemTextura = LoadBMP("textura3.bmp"))
    {
        glBindTexture(GL_TEXTURE_2D, textura3);
        glTexImage2D(GL_TEXTURE_2D, 0, 3, imagemTextura->sizeX,imagemTextura->sizeY, 0, GL_RGB, GL_UNSIGNED_BYTE,imagemTextura->data);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);
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
                        desenhaCuboComTextura(i,j,k);
                    }
                }
            }
        }
    }
}


void atualizarCamera()
{
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    gluLookAt(0+olhoCameraX, 0, 0+olhoCameraZ, 0, 0, -30+posicaoCameraZ, 0,1,0);

}

void carregarLuz()
{
    if(luz1Ligada)
    {
        GLfloat luz_0_posicao[] = { -100+luz1PosicaoX, -5+luz1PosicaoY, 1+luz1PosicaoZ, 1 };
        GLfloat luz_0_difusa[] = { 1, 1, 0 };
        GLfloat luz_0_ambiente[] = { 1, 1, 1 };

        glLightfv(GL_LIGHT0, GL_POSITION, luz_0_posicao);
        glLightfv(GL_LIGHT0, GL_DIFFUSE, luz_0_difusa);
        glLightfv(GL_LIGHT0, GL_AMBIENT, luz_0_ambiente);

        glEnable(GL_LIGHT0);

    }
    else
    {
        glDisable(GL_LIGHT0);
    }
    if(luz2Ligada)
    {
        GLfloat luz_1_posicao[] = { 100+luz2PosicaoX, 4+luz2PosicaoY, 1+luz2PosicaoZ, 0.5 };
        GLfloat luz_1_difusa[] = { 0.4, 0, 1 };
        GLfloat luz_1_ambiente[] = { 1, 0, 0 };

        glLightfv(GL_LIGHT1, GL_POSITION, luz_1_posicao);
        glLightfv(GL_LIGHT1, GL_DIFFUSE, luz_1_difusa);
        glLightfv(GL_LIGHT1, GL_AMBIENT, luz_1_ambiente);

        glEnable(GL_LIGHT1);
    }
    else
    {
        glDisable(GL_LIGHT1);
    }
    if(luz3Ligada)
    {
        GLfloat luz_2_posicao[] = { 75+luz3PosicaoX, -1.5+luz3PosicaoY, 0+luz3PosicaoZ, 0.7 };
        GLfloat luz_2_difusa[] = { 0.5, 0.5, 0.7 };
        GLfloat luz_2_ambiente[] = { 0, 1, 0 };

        glLightfv(GL_LIGHT2, GL_POSITION, luz_2_posicao);
        glLightfv(GL_LIGHT2, GL_DIFFUSE, luz_2_difusa);
        glLightfv(GL_LIGHT2, GL_AMBIENT, luz_2_ambiente);

        glEnable(GL_LIGHT2);
    }
    else
    {
        glDisable(GL_LIGHT2);
    }
    float lightPos[] = { 0, 100, 1 };
    float spotDirection[] = {0.0, -1.0, 0.0};
    glLightfv(GL_LIGHT3, GL_POSITION, lightPos);
    glLightf(GL_LIGHT3, GL_SPOT_CUTOFF, spotAngle);
    glLightfv(GL_LIGHT3, GL_SPOT_DIRECTION, spotDirection);
    glLightf(GL_LIGHT3, GL_SPOT_EXPONENT, spotExponent);
    glEnable(GL_LIGHT3);
    glEnable(GL_LIGHTING);
}


void movimentaCamera(int key, int x, int y)
{
    switch(key)
    {
    case GLUT_KEY_UP:
        olhoCameraZ += 5;
        break;
    case GLUT_KEY_DOWN:
        olhoCameraZ -= 5;
        break;
    case GLUT_KEY_LEFT:
        olhoCameraX -= 5;
        break;
    case GLUT_KEY_RIGHT:
        olhoCameraX += 5;
        break;
    case GLUT_KEY_PAGE_UP:
        posicaoCameraZ += 5;
        break;
    case GLUT_KEY_PAGE_DOWN:
        posicaoCameraZ -= 5;
        break;
    }
    atualizarCamera();
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
        luz1Ligada = !luz1Ligada;
        break;
    case 's':
    case 'S':
        luz2Ligada = !luz2Ligada;
        break;
    case 'd':
    case 'D':
        luz3Ligada = !luz3Ligada;
        break;
    case 'q':
    case 'Q':
        if(luz1PosicaoX >= 100)
        {
            luz1PosicaoX -= 10;
        }
        else if(luz1PosicaoX <= 0)
        {
            luz1PosicaoX += 10;
        }
        break;
    case 'w':
    case 'W':
        if(luz2PosicaoX >= 100)
        {
            luz2PosicaoX -= 10;
        }
        else if(luz2PosicaoX <= 0)
        {
            luz2PosicaoX += 10;
        }
        break;
    case 'e':
    case 'E':
        if(luz3PosicaoX >= 100)
        {
            luz3PosicaoX -= 10;
        }
        else if(luz3PosicaoX <= 0)
        {
            luz3PosicaoX += 10;
        }
        break;
    case 't':
    case 'T':
        texturaLigada = !texturaLigada;
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


}

void initGL()
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClearDepth(1.0f);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    gerarTextura();
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
