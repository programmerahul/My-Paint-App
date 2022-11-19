#include <GL/glut.h>
void init (void)
{
glClearColor (1.0, 1.0, 1.0, 0.0);
glMatrixMode (GL_PROJECTION);
gluOrtho2D (0.0, 400.0, 0.0, 300.0);
}
GLint x=0;
bool state=1;
void timer(int){
    glutPostRedisplay();
    glutTimerFunc(1000/60,timer,0);
    if(state && x<350){
        x++;
    }else if(state)state=!state;
    else if(!state && x>0){
        x--;
    }else state=!state;

}
void display(void){
    glClear (GL_COLOR_BUFFER_BIT);
    glColor3f (0.0, 0.4, 0.2);
    glClearColor (1.0, 1.0, 1.0, 0.0);
    glRecti (x, 200, x+50, 150);
    glFlush();
}
int main (int argc, char** argv)
{
glutInit (&argc, argv);

glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);

glutInitWindowPosition (50, 100);

glutInitWindowSize (400, 300);

glutCreateWindow ("An Example OpenGL Program"); 
init ( );
glutDisplayFunc (display);
glutTimerFunc(0,timer,0);
glutMainLoop ( );

}