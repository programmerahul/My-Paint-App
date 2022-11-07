#include <GL/glut.h>
#include<iostream>
#include<cmath>
using namespace std;

GLint winW=1200,winH=700;
class scrPt {
public:
GLint x, y;
};
class myColor {
    public:
	float r,g,b;
};
myColor currentColor;
GLint thickness=1;
enum COLOR_CODE
{
	RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BLACK
};
myColor getRgb(int code)
{   
    myColor color;
	if (code == RED)
	{
		color.r = 1.0;
		color.g = 0.0;
		color.b = 0.0;
	}
	else if (code == GREEN)
	{
		color.r = 0.0;
		color.g = 1.0;
		color.b = 0.0;
	}
	else if (code == BLUE)
	{
		color.r = 0.0;
		color.g = 0.0;
		color.b = 1.0;
	}
	else if (code == CYAN)
	{
		color.r = 0.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	else if (code == MAGENTA)
	{
		color.r = 1.0;
		color.g = 0.0;
		color.b = 1.0;
	}
	else if (code == YELLOW)
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 0.0;
	}
	else if (code == WHITE)
	{
		color.r = 1.0;
		color.g = 1.0;
		color.b = 1.0;
	}
	else if (code == BLACK)
	{
		color.r = 0.0;
		color.g = 0.0;
		color.b = 0.0;
    }
    return color;
}

void init (void)
{
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
glutInitWindowPosition (100, 50);
glutInitWindowSize (winW, winH);
glutCreateWindow ("My Paint App");  
glClearColor (1.0, 1.0, 1.0, 0.0);
glMatrixMode (GL_PROJECTION);
gluOrtho2D (0.0, 1200.0, 0.0, 700.0);
glClear (GL_COLOR_BUFFER_BIT);
currentColor.r=0;
currentColor.g=1;
currentColor.b=0;
glColor3f (currentColor.r, currentColor.g, currentColor.b);
glFlush();
}
void commit(){
    glutMainLoop ();
}
void setPixel (GLint xCoord, GLint yCoord)
{
glBegin (GL_POINTS);
glVertex2i (xCoord, yCoord);
glEnd ( );
}

void plotPoint (scrPt p){
    glBegin (GL_POINTS);
    glVertex2i (p.x, p.y);
    glEnd ();
    glFlush();
}
inline int myRound (const float a) { return int (a + 0.5); }
void lineDDA (int x0, int y0, int xEnd, int yEnd)
{
int dx = xEnd - x0, dy = yEnd - y0, steps, k;
float xIncrement, yIncrement, x = x0, y = y0;
if (abs (dx) > abs (dy))
steps = abs (dx);
else
steps = abs (dy);
xIncrement = float (dx) / float (steps);
yIncrement = float (dy) / float (steps);
setPixel (myRound (x), myRound (y));
for (k = 0; k < steps; k++) {
x += xIncrement;
y += yIncrement;
setPixel (myRound (x), myRound (y));
}
glFlush();
}
void plotLine (scrPt p1, scrPt p2)
{
    lineDDA(p1.x,p1.y,p2.x,p2.y);
}
bool compareColor(myColor c1,myColor c2){
    if(int(c1.b*10)==int(c2.b*10) && int(c1.g*10)==int(c2.g*10) && int(c1.r*10) == int(c2.r*10))return true;
    return false;
}
myColor getPixelColor(int x, int y) {
	myColor color;
	glReadPixels(x, y, 1, 1, GL_RGB, GL_FLOAT, &color);
	return color;
}
void boundaryFill (GLint x, GLint y, myColor fillColor,myColor prevColor){
myColor interiorColor;
glPointSize (1);
interiorColor=getPixelColor (x, y);
if (!compareColor(interiorColor,fillColor) && compareColor(interiorColor,prevColor)) {
    setPixel (x, y);
    boundaryFill (x + 1, y , fillColor,prevColor);
    boundaryFill (x - 1, y , fillColor,prevColor);
    boundaryFill (x , y + 1, fillColor,prevColor);
    boundaryFill (x , y - 1, fillColor,prevColor);
}
glPointSize (thickness);
glFlush();
}
void plotRectangle(scrPt p1,scrPt p2){
    scrPt p3,p4;
    p3.x=p1.x;
    p3.y=p2.y;
    p4.x=p2.x;
    p4.y=p1.y;
    plotLine(p1,p3);
    plotLine(p1,p4);
    plotLine(p2,p3);
    plotLine(p2,p4);
    glFlush();
}
void plotFilledRect(scrPt p1,scrPt p2){
    plotRectangle(p1,p2);
    GLint x,y;
    if(p2.x>p1.x){
        x=p2.x-thickness;
    }else{
        x=p1.x-thickness;
    }
    if(p2.y>p1.y){
        y=p2.y-thickness;
    }else{
        y=p1.y-thickness;
    }
    myColor prevColor=getPixelColor(x,y);
    boundaryFill(x,y,currentColor,prevColor);
    glFlush();
}



void plotCircle(scrPt p1,scrPt p2){
    int radius=sqrt((p1.x-p2.x)*(p1.x-p2.x)+(p1.y-p2.y)*(p1.y-p2.y));
    GLint p = 1 - radius;
    void circlePlotPoints (GLint, GLint, scrPt);
    scrPt circPt;
    circPt.x=0;
    circPt.y=radius;
    circlePlotPoints (p1.x, p1.y, circPt);
    while(circPt.x<circPt.y){
        circPt.x++;
        if(p<0){
            p+=2*(circPt.x)+1;
        }else{
            circPt.y--;
            p+=2*(circPt.x-circPt.y)+1;
        }
        circlePlotPoints(p1.x,p1.y,circPt);
    }
    glFlush();
}
void circlePlotPoints (GLint xc, GLint yc, scrPt circPt)
{
setPixel (xc + circPt.x, yc + circPt.y);
setPixel (xc - circPt.x, yc + circPt.y);
setPixel (xc + circPt.x, yc - circPt.y);
setPixel (xc - circPt.x, yc - circPt.y);
setPixel (xc + circPt.y, yc + circPt.x);
setPixel (xc - circPt.y, yc + circPt.x);
setPixel (xc + circPt.y, yc - circPt.x);
setPixel (xc - circPt.y, yc - circPt.x);
}
void plotFilledCircle(scrPt p1,scrPt p2){
    plotCircle(p1,p2);
    GLint x=p1.x;
    GLint y=p1.y;
    myColor prevColor=getPixelColor(x,y);
    boundaryFill(x,y,currentColor,prevColor);
    glFlush();
}
GLint firstx=0;
GLint firsty=0;
GLint secondx=0;
GLint secondy=0;
int currentOption=-1;
int cnt=0;
void onMouseClick (GLint button, GLint action, GLint xMouse, GLint yMouse)
{
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==0){
    firstx=xMouse;
    firsty=winH-yMouse;
    scrPt p;
    p.x=firstx;
    p.y=firsty;
    plotPoint(p);
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==1){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotLine(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==2){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotRectangle(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==3){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotFilledRect(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==4){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotCircle(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==5){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotFilledCircle(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==6){
    firstx=xMouse;
    firsty=winH-yMouse;
    myColor prevColor=getPixelColor(firstx,firsty);
    boundaryFill(firstx,firsty,currentColor,prevColor);
    glFlush();
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==7){
    if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
         scrPt p1;
        p1.x=firstx;
        p1.y=firsty;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        plotLine(p1,p2);
        firstx=p2.x;
        firsty=p2.y;
    }
}
}
void menuBarAction (GLint selectedOption){
    currentOption=selectedOption;
    cnt=0;
    glutPostRedisplay ();
}

void colorSubMenu (GLint colorOption)
{
    currentColor=getRgb(colorOption);
    glColor3f (currentColor.r, currentColor.g, currentColor.b);
    glutPostRedisplay ( );
}
void thicknessSubMenu (GLint thicknessOption)
{
    thickness=thicknessOption;
    glPointSize (thickness);
    glutPostRedisplay ( );
}
void createMenu(){
    GLint sumMenu1,sumMenu2;
    sumMenu1 = glutCreateMenu (colorSubMenu);
    glutAddMenuEntry ("RED", RED);
    glutAddMenuEntry ("GREEN",GREEN);
    glutAddMenuEntry ("BLUE", BLUE);
    glutAddMenuEntry ("CYAN", CYAN);
    glutAddMenuEntry ("MAGENTA", MAGENTA);
    glutAddMenuEntry ("YELLOW", YELLOW);
    glutAddMenuEntry ("WHITE", WHITE);
    glutAddMenuEntry ("BLACK", BLACK);

    sumMenu2 = glutCreateMenu (thicknessSubMenu);
    glutAddMenuEntry ("1", 1);
    glutAddMenuEntry ("2", 2);
    glutAddMenuEntry ("3", 3);
    glutAddMenuEntry ("4", 4);
    glutAddMenuEntry ("5", 5);
    glutAddMenuEntry ("6", 6);
    glutAddMenuEntry ("7", 7);
    glutAddMenuEntry ("8", 8);

    glutCreateMenu(menuBarAction);
    glutAddMenuEntry ("Point", 0);
    glutAddMenuEntry ("Line", 1);
    glutAddMenuEntry ("Rectangle", 2);
    glutAddMenuEntry ("Filled Rectangle", 3);
    glutAddMenuEntry ("Circle", 4);
    glutAddMenuEntry ("Filled Circle", 5);
    glutAddMenuEntry ("Fill", 6);
    glutAddMenuEntry ("PolyLine", 7);
    glutAddSubMenu ("Color", sumMenu1);
    glutAddSubMenu ("Thickness", sumMenu2);
    glutAttachMenu (GLUT_RIGHT_BUTTON);
}
int main (int argc, char** argv)
{
glutInit (&argc, argv);
init ();
createMenu();
glutMouseFunc(onMouseClick);
commit();
}