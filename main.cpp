#include <GL/glut.h>
#include<cmath>
#include<iostream>
#include<vector>
 #define STB_IMAGE_WRITE_IMPLEMENTATION
#include"toPng.h"
using namespace std;
void timer(int);
const GLint winW=1200,winH=700;

void plotEverything(void);

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
enum COLOR_CODE{
	RED, GREEN, BLUE, CYAN, MAGENTA, YELLOW, WHITE, BLACK
};
myColor getRgb(int code){   
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
void searchAndTranslate();
bool isInVicinity(scrPt p,scrPt p1){
    if(abs(p.x-p1.x)<10 && abs(p.y-p1.y)<10)return true;
    return false;
}
void init (void){
glutInitDisplayMode (GLUT_SINGLE | GLUT_RGB);
glutInitWindowPosition (100, 50);
glutInitWindowSize (winW, winH);
glutCreateWindow ("My-Paint-App (Right Click Please!)");  
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
void floodFill (GLint x, GLint y, myColor fillColor){
myColor interiorColor;
glPointSize (1);
interiorColor=getPixelColor (x, y);
if (!compareColor(interiorColor,fillColor)) {
    setPixel (x, y);
    floodFill (x + 1, y , fillColor);
    floodFill (x - 1, y , fillColor);
    floodFill (x , y + 1, fillColor);
    floodFill (x , y - 1, fillColor);
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
void plotFilledRectangle(scrPt p1,scrPt p2){
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
    floodFill(x,y,currentColor);
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
    floodFill(x,y,currentColor);
    glFlush();
}
GLint firstx=0;
GLint firsty=0;
GLint secondx=0;
GLint secondy=0;

GLint animeRectx1=0;
GLint animeRecty1=0;
GLint animeRectx2=0;
GLint animeRecty2=0;

GLint startAnimex=0;
GLint startAnimey=0;
GLint endAnimex=0;
GLint endAnimey=0;
int currentOption=-1;
int cnt=0;

//clipping
int x_max;
int y_max;
int x_min;
int y_min;
const int INSIDE = 0; 
const int LEFT = 1;
const int RIGHT = 2;
const int BOTTOM = 4; 
const int TOP = 8;
int computeCode(double x, double y)
{
    int code = INSIDE;
    if (x < x_min) 
        code |= LEFT;
    else if (x > x_max)
        code |= RIGHT;
    if (y < y_min) 
        code |= BOTTOM;
    else if (y > y_max) 
        code |= TOP;
    return code;
}

void cohenSutherlandClip(double x1, double y1,double x2, double y2,vector<pair<double,double>>&myLines){
    int code1 = computeCode(x1, y1);
    int code2 = computeCode(x2, y2);
    bool accept = false;
    while (true) {
        if ((code1 == 0) && (code2 == 0)) {
            accept = true;
            break;
        }
        else if (code1 & code2) {
            break;
        }
        else {
            int code_out;
            double x, y;
            if (code1 != 0)
                code_out = code1;
            else
                code_out = code2;
            if (code_out & TOP) {
                x = x1 + (x2 - x1) * (y_max - y1) / (y2 - y1);
                y = y_max;
            }
            else if (code_out & BOTTOM) {
                x = x1 + (x2 - x1) * (y_min - y1) / (y2 - y1);
                y = y_min;
            }
            else if (code_out & RIGHT) {
                y = y1 + (y2 - y1) * (x_max - x1) / (x2 - x1);
                x = x_max;
            }
            else if (code_out & LEFT) {
                y = y1 + (y2 - y1) * (x_min - x1) / (x2 - x1);
                x = x_min;
            }
            if (code_out == code1) {
                x1 = x;
                y1 = y;
                code1 = computeCode(x1, y1);
            }
            else {
                x2 = x;
                y2 = y;
                code2 = computeCode(x2, y2);
            }
        }
    }
    if (accept) {
             myLines.push_back({x1,y1});
             myLines.push_back({x2,y2});
    }
}

int dh=35;//top bar width
vector<vector<bool>> topBar(winW+1,vector<bool>(dh+1,0));
vector<pair<double,double>> inputPoints;
vector<pair<double,double>> inputLines;
vector<pair<double,double>> inputRect;
vector<pair<double,double>> inputFilledRect;
vector<pair<double,double>> inputCircle;
vector<pair<double,double>> inputFilledCircle;

bool isCreated=0;
void createBar(){
    int objectNo=0;
    glColor3f (0.0,1.0,0.0);
     if(isCreated){
        glPointSize(1);
       for(int i=0;i<winW;i++){
        for(int j=winH-dh;j<winH;j++){
            if(topBar[i][j-(winH-dh)])
                setPixel(i,j);
        }
       }
    glFlush();
    glPointSize(thickness);
    glColor3f (currentColor.r, currentColor.g, currentColor.b);
    return;
    }
     thickness=1;
    glPointSize(thickness);
    scrPt p1,p2;
    p1.x=0;
    p2.x=winW;
    
    p1.y=winH-dh;
    p2.y=winH-dh;
    plotLine(p1,p2);
    int wid=winW/9;
    for(int i=wid;i<=winW;i+=wid){
        if(objectNo==0){
            scrPt p;
            p.x=i-(wid/2);
            p.y=winH-dh/2;
            thickness=4;
            glPointSize(thickness);
            plotPoint(p);
            thickness=1;
            glPointSize(thickness);
            glFlush();
        }else if(objectNo==1){
            scrPt p1,p2;
            p1.x=i-(2*wid/3);
            p1.y=winH-dh/3;
            p2.x=i-(wid/3);
            p2.y=winH-(2*dh/3);
            plotLine(p1,p2);
             glFlush();
        }
        else if(objectNo==2){
            scrPt p1,p2;
            p1.x=i-(2*wid/3);
            p1.y=winH-dh/3;
            p2.x=i-(wid/3);
            p2.y=winH-(2*dh/3);
            plotRectangle(p1,p2);
             glFlush();
        }
        else if(objectNo==3){
            scrPt p1,p2;
            p1.x=i-(2*wid/3);
            p1.y=winH-dh/3;
            p2.x=i-(wid/3);
            p2.y=winH-(2*dh/3);
            plotFilledRectangle(p1,p2);
            glFlush();
        }
        else if(objectNo==4){
            scrPt p;
            p.x=i-(wid/2);
            p.y=winH-dh/2;
            scrPt p1;
            p1.x=i-(wid/2);;
            p1.y=winH-dh/4;
            plotCircle(p,p1);
            glFlush();
        }
        else if(objectNo==5){
             scrPt p;
            p.x=i-(wid/2);
            p.y=winH-dh/2;
            scrPt p1;
            p1.x=i-(wid/2);;
            p1.y=winH-dh/4;
            plotFilledCircle(p,p1);
            glFlush();
        }
        else if(objectNo==6){
            scrPt p1,p2;
              p2.y=winH-(dh/4);
              p2.x=i-(2*wid/3);
              p1.y=winH-(dh/2);
              p1.x=i-(wid/2);
              plotLine(p1,p2);
              glFlush();
             p2.y=winH-(3*dh/4);
            plotLine(p1,p2);
             glFlush();
        }
        else if(objectNo==7){
              scrPt p1,p2;
            p1.x=i-(2*wid/3);
            p1.y=winH-dh/2;
            p2.x=i-(wid/2);
            p2.y=winH-(dh/4);
            plotRectangle(p1,p2);
            p1.y=winH-3*dh/4;
            plotRectangle(p1,p2);
             glFlush();
        }
        else if(objectNo==8){
              scrPt p1,p2;
              p2.y=winH-(dh/4);
              p2.x=i-(2*wid/3);
              p1.y=winH-(dh/4);
              p1.x=i-(wid/2);
              plotLine(p1,p2);
              glFlush();
             p1.y=winH-dh/2;
             p2.y=winH-dh/2;
             plotLine(p1,p2);
             glFlush();
             p2.y=winH-(dh/4);
             p1.y=winH-3*dh/4;
            p1.x=i-(2*wid/3);
            plotLine(p1,p2);
             glFlush();
        }else{
            objectNo=0;
        }
        p1.x=p2.x=i;
        p1.y=winH;
        p2.y=winH-dh;
        plotLine(p1,p2);
        objectNo++;
    }
    
    glFlush();
    thickness=1;
    glPointSize(thickness);
    glColor3f (currentColor.r, currentColor.g, currentColor.b);
     for(int i=0;i<winW;i++){
        for(int j=winH-dh;j<winH;j++){
            myColor color=getPixelColor(i,j);
            if(color.r==0){
                topBar[i][j-(winH-dh)]=1;
            }else{
                topBar[i][j-(winH-dh)]=0;
            }
        }
       }
    isCreated=1;
}
int findObject(GLint x,GLint y){
    int wid=winW/9;
    x/=wid;
    int dh=35;
    if(y>winH-dh){
        return x;
    }
    return -1;
}
bool directionOfObject=1;
void animate(){
    glutTimerFunc(0,timer,0);
}
void saveImage(char* filepath) {
 int width=winW, height=winH-dh;
 GLsizei nrChannels = 3;
 GLsizei stride = nrChannels * width;
 stride += (stride % 4) ? (4 - stride % 4) : 0;
 GLsizei bufferSize = stride * height;
 std::vector<char> buffer(bufferSize);
 glPixelStorei(GL_PACK_ALIGNMENT, 4);
 glReadBuffer(GL_FRONT);
 glReadPixels(0, 0, width, height, GL_RGB, GL_UNSIGNED_BYTE, buffer.data());
 stbi_flip_vertically_on_write(true);
 stbi_write_png(filepath, width, height, nrChannels, buffer.data(), stride);
}
int DrawingNumber=0;
void onMouseClick (GLint button, GLint action, GLint xMouse, GLint yMouse){
    if(button == GLUT_LEFT_BUTTON && action == GLUT_DOWN){
        int object=findObject(xMouse,winH-yMouse);
        if(object!=-1){
        currentOption=object;
        cnt=0;
        object=-1;
        return;
        }
    }
   
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==0){
    firstx=xMouse;
    firsty=winH-yMouse;
    scrPt p;
    p.x=firstx;
    p.y=firsty;
    inputPoints.push_back({p.x,p.y});
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
        inputLines.push_back({firstx,firsty});
        inputLines.push_back({p2.x,p2.y});
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
        inputRect.push_back({p1.x,p1.y});
        inputRect.push_back({p2.x,p2.y});
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
        inputFilledRect.push_back({p1.x,p1.y});
        inputFilledRect.push_back({p2.x,p2.y});
        plotFilledRectangle(p1,p2);
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
        inputCircle.push_back({p1.x,p1.y});
        inputCircle.push_back({p2.x,p2.y});
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
        inputFilledCircle.push_back({p1.x,p1.y});
        inputFilledCircle.push_back({p2.x,p2.y});
        plotFilledCircle(p1,p2);
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==6){
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
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==7){
    firstx=xMouse;
    firsty=winH-yMouse;
    myColor prevColor=getPixelColor(firstx,firsty);
    boundaryFill(firstx,firsty,currentColor,prevColor);
    glFlush();
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==8){
    firstx=xMouse;
    firsty=winH-yMouse;
    floodFill(firstx,firsty,currentColor);
    glFlush();
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==9){
   if(cnt==0){
         firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
       
        int secondx=xMouse;
        int secondy=winH-yMouse;
        if(firstx<secondx){
            x_min=firstx;
            x_max=secondx;
        }else{
            x_min=secondx;
            x_max=firstx;
        }
        if(firsty<secondy){
            y_min=firsty;
            y_max=secondy;
        }else{
            y_min=secondy;
            y_max=firsty;
        }
        vector<pair<double,double>> linesAfterClip,temp;
       
        for(int i=0;i<inputLines.size();i+=2){
            cohenSutherlandClip(inputLines[i].first,inputLines[i].second,
            inputLines[i+1].first,inputLines[i+1].second,linesAfterClip);
        }
        glClear (GL_COLOR_BUFFER_BIT);
        glFlush();
        createBar();
        glFlush();
         glColor3f (currentColor.r, currentColor.g, currentColor.b);
    glutPostRedisplay ( );
    
        for(int i=0;i<linesAfterClip.size();i+=2){
            scrPt p1,p2;
            p1.x=linesAfterClip[i].first;
            p1.y=linesAfterClip[i].second;
            p2.x=linesAfterClip[i+1].first;
            p2.y=linesAfterClip[i+1].second;
            plotLine(p1,p2);
        }
        inputLines=linesAfterClip;
        linesAfterClip=temp;
        cnt=0;
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==10){
     if(cnt==0){
         animeRectx1=xMouse;
        animeRecty1=winH-yMouse;
        startAnimex=animeRectx1;
        startAnimey=animeRecty1;
        cnt++;
    }else if(cnt==1){
        scrPt p1;
        p1.x=animeRectx1;
        p1.y=animeRecty1;
        scrPt p2;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        animeRectx2=p2.x;
        animeRecty2=p2.y;
        plotRectangle(p1,p2);
        cnt++;
    }else{
        endAnimex=xMouse;
        endAnimey=winH-yMouse;
        animate();
    }
}
if (button == GLUT_LEFT_BUTTON && action == GLUT_DOWN && currentOption==11){
    if(cnt==0){
        firstx=xMouse;
        firsty=winH-yMouse;
        cnt++;
    }else{
        secondx=xMouse;
        secondy=winH-yMouse;
        searchAndTranslate();
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        cnt=0;
    }
}
}
void menuBarAction (GLint selectedOption){
    if(selectedOption==12){
        char filePath[]="MyDrawing.png";
        saveImage(filePath);
        return;
    }
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
    GLint sumMenu1,sumMenu2,subMenu3;
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
    glutAddMenuEntry ("PolyLine", 6);
    glutAddMenuEntry ("Boundary Fill", 7);
    glutAddMenuEntry ("Flood Fill", 8);
    glutAddMenuEntry ("Clipping", 9);
    glutAddMenuEntry ("Animation",10);
    glutAddMenuEntry ("Translation",11);
    glutAddMenuEntry ("Save",12);
    glutAddSubMenu ("Color", sumMenu1);
    glutAddSubMenu ("Thickness", sumMenu2);

    glutAttachMenu (GLUT_RIGHT_BUTTON);
}


void onMouseMove(GLint xMouse, GLint yMouse){
    if(currentOption==1 && cnt>0){
        scrPt p1,p2;
        p1.x=firstx;
        p1.y=firsty;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        plotLine(p1,p2);
    }
    if(currentOption==2 && cnt>0){
        scrPt p1,p2;
        p1.x=firstx;
        p1.y=firsty;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        plotRectangle(p1,p2);
    }
    if(currentOption==3 && cnt>0){
        scrPt p1,p2;
        p1.x=firstx;
        p1.y=firsty;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        plotFilledRectangle(p1,p2);
    }
    if(currentOption==4 && cnt>0){
        scrPt p1,p2;
        p1.x=firstx;
        p1.y=firsty;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        plotCircle(p1,p2);
    }
    if(currentOption==5 && cnt>0){
        scrPt p1,p2;
        p1.x=firstx;
        p1.y=firsty;
        p2.x=xMouse;
        p2.y=winH-yMouse;
        glClear (GL_COLOR_BUFFER_BIT);
        plotEverything();
        plotFilledCircle(p1,p2);
    }
}

int main (int argc, char** argv)
{
glutInit (&argc, argv);
init ();
createBar();
createMenu();
glutMouseFunc(onMouseClick);
glutPassiveMotionFunc(onMouseMove);
commit();
}

void timer(int){
    glClear (GL_COLOR_BUFFER_BIT);
    plotEverything();
    if(directionOfObject && animeRectx1<endAnimex){
        animeRectx1+=5;animeRectx2+=5;
    }else if(directionOfObject)directionOfObject=!directionOfObject;
    else if(!directionOfObject && animeRectx1>startAnimex){
        animeRectx1-=5;animeRectx2-=5;
    }else directionOfObject=!directionOfObject;
    scrPt p1;
    p1.x=animeRectx1;
    p1.y=animeRecty1;
    scrPt p2;
    p2.x=animeRectx2;
    p2.y=animeRecty2;
    plotRectangle(p1,p2);
    glutTimerFunc(1000/60,timer,0);
}

void plotEverything(){
    createBar();
    for(int i=0;i<inputPoints.size();i++){
        setPixel(inputPoints[i].first,inputPoints[i].second);
    }
    for(int i=0;i<inputLines.size();i+=2){
        scrPt p1,p2;
        p1.x=inputLines[i].first;
        p1.y=inputLines[i].second;
        p2.x=inputLines[i+1].first;
        p2.y=inputLines[i+1].second;
        plotLine(p1,p2);
    }
    for(int i=0;i<inputRect.size();i+=2){
        scrPt p1,p2;
        p1.x=inputRect[i].first;
        p1.y=inputRect[i].second;
        p2.x=inputRect[i+1].first;
        p2.y=inputRect[i+1].second;
        plotRectangle(p1,p2);
    }
    for(int i=0;i<inputFilledRect.size();i+=2){
        scrPt p1,p2;
        p1.x=inputFilledRect[i].first;
        p1.y=inputFilledRect[i].second;
        p2.x=inputFilledRect[i+1].first;
        p2.y=inputFilledRect[i+1].second;
        plotFilledRectangle(p1,p2);
    }
    for(int i=0;i<inputCircle.size();i+=2){
        scrPt p1,p2;
        p1.x=inputCircle[i].first;
        p1.y=inputCircle[i].second;
        p2.x=inputCircle[i+1].first;
        p2.y=inputCircle[i+1].second;
        plotCircle(p1,p2);
    }
    for(int i=0;i<inputFilledCircle.size();i+=2){
        scrPt p1,p2;
        p1.x=inputFilledCircle[i].first;
        p1.y=inputFilledCircle[i].second;
        p2.x=inputFilledCircle[i+1].first;
        p2.y=inputFilledCircle[i+1].second;
        plotFilledCircle(p1,p2);
    }
    glFlush();
}

void searchAndTranslate(){
    scrPt cursorPoint,lastPoint,tempPoint;
    cursorPoint.x=firstx;
    cursorPoint.y=firsty;
    lastPoint.x=secondx;
    lastPoint.y=secondy;
    for(int i=0;i<inputPoints.size();i++){
            tempPoint.x=inputPoints[i].first;
            tempPoint.y=inputPoints[i].second;
            if(isInVicinity(cursorPoint,tempPoint)){
                inputPoints[i].first=lastPoint.x;
                inputPoints[i].second=lastPoint.y;
                return;
            }
    }
    for(int i=0;i<inputLines.size();i+=2){
        tempPoint.x=inputLines[i].first;
        tempPoint.y=inputLines[i].second;
        if(isInVicinity(cursorPoint,tempPoint)){
                inputLines[i+1].first+=(lastPoint.x-inputLines[i].first);
                inputLines[i+1].second+=(lastPoint.y-inputLines[i].second);
                inputLines[i].first=lastPoint.x;
                inputLines[i].second=lastPoint.y;
                return;
        }
         tempPoint.x=inputLines[i+1].first;
        tempPoint.y=inputLines[i+1].second;
          if(isInVicinity(cursorPoint,tempPoint)){
                inputLines[i].first+=(lastPoint.x-inputLines[i+1].first);
                inputLines[i].second+=(lastPoint.y-inputLines[i+1].second);
                inputLines[i+1].first=lastPoint.x;
                inputLines[i+1].second=lastPoint.y;
                return;
        }
    }
    for(int i=0;i<inputRect.size();i+=2){
        tempPoint.x=inputRect[i].first;
        tempPoint.y=inputRect[i].second;
        if(isInVicinity(cursorPoint,tempPoint)){
                inputRect[i+1].first+=(lastPoint.x-inputRect[i].first);
                inputRect[i+1].second+=(lastPoint.y-inputRect[i].second);
                inputRect[i].first=lastPoint.x;
                inputRect[i].second=lastPoint.y;
                return;
        }
        tempPoint.x=inputRect[i+1].first;
        tempPoint.y=inputRect[i+1].second;
          if(isInVicinity(cursorPoint,tempPoint)){
                inputRect[i].first+=(lastPoint.x-inputRect[i+1].first);
                inputRect[i].second+=(lastPoint.y-inputRect[i+1].second);
                inputRect[i+1].first=lastPoint.x;
                inputRect[i+1].second=lastPoint.y;
                return;
        }
    }
    for(int i=0;i<inputFilledRect.size();i+=2){
        tempPoint.x=inputFilledRect[i].first;
        tempPoint.y=inputFilledRect[i].second;
        if(isInVicinity(cursorPoint,tempPoint)){
                inputFilledRect[i+1].first+=(lastPoint.x-inputFilledRect[i].first);
                inputFilledRect[i+1].second+=(lastPoint.y-inputFilledRect[i].second);
                inputFilledRect[i].first=lastPoint.x;
                inputFilledRect[i].second=lastPoint.y;
                return;
        }
         tempPoint.x=inputFilledRect[i+1].first;
        tempPoint.y=inputFilledRect[i+1].second;
          if(isInVicinity(cursorPoint,tempPoint)){
                inputFilledRect[i].first+=(lastPoint.x-inputFilledRect[i+1].first);
                inputFilledRect[i].second+=(lastPoint.y-inputFilledRect[i+1].second);
                inputFilledRect[i+1].first=lastPoint.x;
                inputFilledRect[i+1].second=lastPoint.y;
                return;
        }
    }
    for(int i=0;i<inputCircle.size();i+=2){
        tempPoint.x=inputCircle[i].first;
        tempPoint.y=inputCircle[i].second;
        if(isInVicinity(cursorPoint,tempPoint)){
                inputCircle[i+1].first+=(lastPoint.x-inputCircle[i].first);
                inputCircle[i+1].second+=(lastPoint.y-inputCircle[i].second);
                inputCircle[i].first=lastPoint.x;
                inputCircle[i].second=lastPoint.y;
                return;
        }
         tempPoint.x=inputCircle[i+1].first;
        tempPoint.y=inputCircle[i+1].second;
          if(isInVicinity(cursorPoint,tempPoint)){
                inputCircle[i].first+=(lastPoint.x-inputCircle[i+1].first);
                inputCircle[i].second+=(lastPoint.y-inputCircle[i+1].second);
                inputCircle[i+1].first=lastPoint.x;
                inputCircle[i+1].second=lastPoint.y;
                return;
        }
    }
    for(int i=0;i<inputFilledCircle.size();i+=2){
        tempPoint.x=inputFilledCircle[i].first;
        tempPoint.y=inputFilledCircle[i].second;
        if(isInVicinity(cursorPoint,tempPoint)){
                inputFilledCircle[i+1].first+=(lastPoint.x-inputFilledCircle[i].first);
                inputFilledCircle[i+1].second+=(lastPoint.y-inputFilledCircle[i].second);
                inputFilledCircle[i].first=lastPoint.x;
                inputFilledCircle[i].second=lastPoint.y;
                return;
        }
         tempPoint.x=inputFilledCircle[i+1].first;
        tempPoint.y=inputFilledCircle[i+1].second;
          if(isInVicinity(cursorPoint,tempPoint)){
                inputFilledCircle[i].first+=(lastPoint.x-inputFilledCircle[i+1].first);
                inputFilledCircle[i].second+=(lastPoint.y-inputFilledCircle[i+1].second);
                inputFilledCircle[i+1].first=lastPoint.x;
                inputFilledCircle[i+1].second=lastPoint.y;
                return;
        }
    }

}
    