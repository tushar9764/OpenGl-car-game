#include<GL/glut.h>
#include<math.h>
#include<iostream>
#define PI 3.14159265f

using namespace std;
float carDirection = 0.0;
bool isForward = true;

float carWidth=4;
float carLength=10;

float carCenterX=0;
float carCenterY=0.5;
float carCenterZ=0;

float carSpeed = 0;
float maxSpeed = 0.6;
float maxRevSpeed = -0.1;
float retard = 0.004;

//camera attributes
float eyeX=0;
float eyeY=0;
float eyeZ=20;
float lookAtX=0;
float lookAtY=8;
float lookAtZ=-20;
float upX=0;
float upY=1;
float upZ=0;

void updateCamera(){
    float dist = (3/4.0)*carLength;
    lookAtX = carCenterX + (dist+10)*sin((carDirection+180)*PI/180);
    lookAtZ = carCenterZ + (dist+10)*cos((carDirection+180)*PI/180);
    
    eyeX = carCenterX + (dist+20)*sin((carDirection)*PI/180);
    eyeZ = carCenterZ + (dist+20)*cos((carDirection)*PI/180);
}

// Function Prototypes
void initGL();
void idle();
void display();
void reshape(GLsizei, GLsizei);
void mouse(int, int, int, int);
void keyboard(unsigned char, int, int);
void specialKeyboard(int, int, int);

// Functions
void xzGrid();
void drawRectangle(float,float,float,float);
void drawCar();
void blackLimo(float, float,float);
void drawCircle(float, float, float, float);
void building(float, float, float, float, float, float, float, float);
void roadBlock(float, float, int);
void sqJunction(float, float);
void sideJunction(float, float, int);
void turnJunction(float, float, int);


//_____END OF FUNCTION PROTOTYPE___________________

int main(int argc, char** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_RGBA | GLUT_ALPHA | GLUT_DEPTH | GLUT_DOUBLE);
    glutInitWindowSize(600,600);
    glutInitWindowPosition(100,100);
    glutCreateWindow("My Virtual World");

    glutDisplayFunc(display);
    glutReshapeFunc(reshape);
    glutKeyboardFunc(keyboard);
    glutSpecialFunc(specialKeyboard);
    glutIdleFunc(idle);
    glutMouseFunc(mouse);
    initGL();

    glutMainLoop();
    return 0;
}
//____END OF MAIN___________________________________


void display(){
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);   
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(
        lookAtX, lookAtY, lookAtZ,
        eyeX, eyeY, eyeZ,
        upX, upY, upZ
        );
        
    drawCar();
    xzGrid();

    glutSwapBuffers();

    if(isForward){
        carSpeed -= retard;
        if(carSpeed<0){
            carSpeed = 0;
        }
    }else{
        carSpeed += retard;
        if(carSpeed>0){
            carSpeed = 0;
        }
    }

    carCenterX += carSpeed*sin((carDirection)*PI/180);
    carCenterZ += carSpeed*cos((carDirection)*PI/180);
    updateCamera();
}

void drawCircle(float x, float y, float z, float rad){
    glBegin(GL_TRIANGLE_FAN);
      glVertex3f(x, y, z);    
      int numSegments = 100;
      GLfloat angle;
      for (int i = 0; i <= numSegments; i++) { 
         angle = i * 2.0f * PI / numSegments;  
         glVertex3f(x, y+cos(angle) * rad, z+sin(angle) * rad);
      }
   glEnd();
}

void drawCar(){
    float w = carWidth/2;
    float l1 = carLength*(3/4.0);
    float l2 = carLength*(1/4.0);
   
    glTranslatef(+carCenterX, 0, +carCenterZ);    
    glRotatef(carDirection,0,1,0);

    glColor3f(0.3,0.3,0.3);
    glBegin(GL_QUADS);
        glVertex3f(w, 0.5, +l1);
        glVertex3f(-w, 0.5, +l1);
        glVertex3f(-w, 0.5, -l2);
        glVertex3f(+w, 0.5, -l2);
    glEnd();

    blackLimo(w, l1, l2);

    glRotatef(-carDirection,0,1,0);
    glTranslatef(-carCenterX, 0, -carCenterZ);
}

void blackLimo(float w, float l1, float l2){
    //left Side
    glColor3f(0.3,0.2,0.2);
    glBegin(GL_POLYGON);
        glVertex3f(w, 1.0, l1);
        glVertex3f(w, 3.0-0.2, l1);
        glVertex3f(w, 3.0, l1-0.2);
        glVertex3f(w, 3.0, l1-2.5);
        glVertex3f(w, 4.0, l1-4.0);
        glVertex3f(w, 4.0, -l2+0.8);
        glVertex3f(w, 3.0, -l2);
        glVertex3f(w, 1.0, -l2);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(w+0.01, 1.0+0.1, l1-2.7);
        glVertex3f(w+0.01, 3.0-0.1, l1-2.7);
        glVertex3f(w+0.01, 4.0-0.1, l1-4.0);
        glVertex3f(w+0.01, 4.0-0.1, l1-5.1);
        glVertex3f(w+0.01, 1.0+0.1, l1-5.1);
        glVertex3f(w+0.01, 1.0+0.1, l1-2.7);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex3f(w+0.01, 4.0-0.1, l1-5.1);
        glVertex3f(w+0.01, 4.0-0.1, l1-7.1);
        glVertex3f(w+0.01, 3.0-0.1, l1-7.5);
        glVertex3f(w+0.01, 1.0+0.1, l1-7.5);
        glVertex3f(w+0.01, 1.0+0.1, l1-5.1);
    glEnd();

    //first window
    glBegin(GL_POLYGON);
        glVertex3f(w+0.01, 3.0-0.2, l1-2.8);
        glVertex3f(w+0.01, 4.0-0.2, l1-4.0);
        glVertex3f(w+0.01, 4.0-0.2, l1-5.0);
        glVertex3f(w+0.01, 3.0-0.2, l1-5.0);
        glVertex3f(w+0.01, 3.0-0.2, l1-2.8);
    glEnd();

    //second window
    glBegin(GL_POLYGON);
        glVertex3f(w+0.01, 3.0-0.2, l1-5.2);
        glVertex3f(w+0.01, 4.0-0.2, l1-5.2);
        glVertex3f(w+0.01, 4.0-0.2, l1-7.0);
        glVertex3f(w+0.01, 3.0-0.2, l1-7.4);
        glVertex3f(w+0.01, 3.0-0.2, l1-5.2);
    glEnd();

    //third window;
    glBegin(GL_POLYGON);
        glVertex3f(w+0.01, 4.0-0.2, l1-7.3);
        glVertex3f(w+0.01, 4.0-0.2, -l2+0.2+0.8);
        glVertex3f(w+0.01, 3.0-0.2, -l2+0.2);
        glVertex3f(w+0.01, 3.0-0.2, l1-7.9);
    glEnd();


    //front wheel
    drawCircle(w+0.01, 1.0, (l1-1.5), 1.0);
    //rear wheel
    drawCircle(w+0.01, 1.0, (0), 1.0);


    //right Side
    glColor3f(0.3,0.2,0.2);
    glBegin(GL_POLYGON);
        glVertex3f(-w, 1.0, l1);
        glVertex3f(-w, 3.0-0.2, l1);
        glVertex3f(-w, 3.0, l1-0.2);
        glVertex3f(-w, 3.0, l1-2.5);
        glVertex3f(-w, 4.0, l1-4.0);
        glVertex3f(-w, 4.0, -l2+0.8);
        glVertex3f(-w, 3.0, -l2);
        glVertex3f(-w, 1.0, -l2);
    glEnd();

    glColor3f(0,0,0);
    glBegin(GL_LINE_STRIP);
        glVertex3f(-w-0.01, 1.0+0.1, l1-2.7);
        glVertex3f(-w-0.01, 3.0-0.1, l1-2.7);
        glVertex3f(-w-0.01, 4.0-0.1, l1-4.0);
        glVertex3f(-w-0.01, 4.0-0.1, l1-5.1);
        glVertex3f(-w-0.01, 1.0+0.1, l1-5.1);
        glVertex3f(-w-0.01, 1.0+0.1, l1-2.7);
    glEnd();

    glBegin(GL_LINE_STRIP);
        glVertex3f(-w-0.01, 4.0-0.1, l1-5.1);
        glVertex3f(-w-0.01, 4.0-0.1, l1-7.1);
        glVertex3f(-w-0.01, 3.0-0.1, l1-7.5);
        glVertex3f(-w-0.01, 1.0+0.1, l1-7.5);
        glVertex3f(-w-0.01, 1.0+0.1, l1-5.1);
    glEnd();

    //first window
    glBegin(GL_POLYGON);
        glVertex3f(-w-0.01, 3.0-0.2, l1-2.8);
        glVertex3f(-w-0.01, 4.0-0.2, l1-4.0);
        glVertex3f(-w-0.01, 4.0-0.2, l1-5.0);
        glVertex3f(-w-0.01, 3.0-0.2, l1-5.0);
        glVertex3f(-w-0.01, 3.0-0.2, l1-2.8);
    glEnd();

    //second window
    glBegin(GL_POLYGON);
        glVertex3f(-w-0.01, 3.0-0.2, l1-5.2);
        glVertex3f(-w-0.01, 4.0-0.2, l1-5.2);
        glVertex3f(-w-0.01, 4.0-0.2, l1-7.0);
        glVertex3f(-w-0.01, 3.0-0.2, l1-7.4);
        glVertex3f(-w-0.01, 3.0-0.2, l1-5.2);
    glEnd();

    //third window;
    glBegin(GL_POLYGON);
        glVertex3f(-w-0.01, 4.0-0.2, l1-7.3);
        glVertex3f(-w-0.01, 4.0-0.2, -l2+0.2+0.8);
        glVertex3f(-w-0.01, 3.0-0.2, -l2+0.2);
        glVertex3f(-w-0.01, 3.0-0.2, l1-7.9);
    glEnd();


    //front wheel
    drawCircle(-w-0.01, 1.0, (l1-1.5), 1.0);
    //rear wheel
    drawCircle(-w-0.01, 1.0, (0), 1.0);


    // Back
    glColor3f(0.288,0.2,0.2);
    glBegin(GL_POLYGON);
        glVertex3f(w, 1.0, -l2);
        glVertex3f(w, 3.0, -l2);
        glVertex3f(w, 4.0, -l2+0.8);
        glVertex3f(-w, 4.0, -l2+0.8);
        glVertex3f(-w, 3.0, -l2);
        glVertex3f(-w, 1.0, -l2);
    glEnd();

    //roof
    glBegin(GL_POLYGON);
        glVertex3f(w, 4.0, -l2+0.7);
        glVertex3f(w, 4.0, l1-4.0);
        glVertex3f(-w, 4.0, l1-4.0);
        glVertex3f(-w, 4.0, -l2+0.7);
    glEnd();

    // back window
    glColor3f(0,0,0);
    glBegin(GL_POLYGON);
        glVertex3f(w-0.2, 3.0-0.1, -l2-0.01);
        glVertex3f(w-0.2, 4.0-0.2, -l2+0.7-0.01);
        glVertex3f(-w+0.2, 4.0-0.2, -l2+0.7-0.01);
        glVertex3f(-w+0.2, 3.0-0.1, -l2-0.01);
    glEnd();

    // tail lamp and indicator
    //left side
    glBegin(GL_QUADS);
    //brake light
        glColor3f(0.4,0,0);
        glVertex3f(w-0.1, 2.5, -l2-0.01);
        glVertex3f(w-0.1-0.5, 2.5, -l2-0.01);
        glVertex3f(w-0.1-0.5, 2.5-1, -l2-0.01);
        glVertex3f(w-0.1, 2.5-1, -l2-0.01);

    //turn indicator
        glColor3f(0.6,0.3,0);
        glVertex3f(w-0.2, 2.2, -l2-0.015);
        glVertex3f(w-0.2-0.3, 2.2, -l2-0.015);
        glVertex3f(w-0.2-0.3, 2.2-0.2, -l2-0.015);
        glVertex3f(w-0.2, 2.2-0.2, -l2-0.015);

    //reverse light
        glColor3f(0.6,0.6,0.6);
        glVertex3f(w-0.2, 1.8, -l2-0.015);
        glVertex3f(w-0.2-0.3, 1.8, -l2-0.015);
        glVertex3f(w-0.2-0.3, 1.8-0.2, -l2-0.015);
        glVertex3f(w-0.2, 1.8-0.2, -l2-0.015);
    glEnd();

    //right side
    glBegin(GL_QUADS);
    //brake light
        glColor3f(0.4,0,0);
        glVertex3f(-(w-0.1), 2.5, -l2-0.01);
        glVertex3f(-(w-0.1-0.5), 2.5, -l2-0.01);
        glVertex3f(-(w-0.1-0.5), 2.5-1, -l2-0.01);
        glVertex3f(-(w-0.1), 2.5-1, -l2-0.01);

    //turn indicator
        glColor3f(0.6,0.3,0);
        glVertex3f(-(w-0.2), 2.2, -l2-0.015);
        glVertex3f(-(w-0.2-0.3), 2.2, -l2-0.015);
        glVertex3f(-(w-0.2-0.3), 2.2-0.2, -l2-0.015);
        glVertex3f(-(w-0.2), 2.2-0.2, -l2-0.015);

    //reverse light
        glColor3f(0.6,0.6,0.6);
        glVertex3f(-(w-0.2), 1.8, -l2-0.015);
        glVertex3f(-(w-0.2-0.3), 1.8, -l2-0.015);
        glVertex3f(-(w-0.2-0.3), 1.8-0.2, -l2-0.015);
        glVertex3f(-(w-0.2), 1.8-0.2, -l2-0.015);
    glEnd();


}

void building(float width, float length, float height, float centerX, float centerZ, float r, float g, float b){
    glTranslatef(-centerX, 0, centerZ);
    glColor3f(r,g,b);
    glBegin(GL_QUADS);
        //left
        glVertex3f(-width/2, 0, -length/2);
        glVertex3f(-width/2, 0, +length/2);
        glVertex3f(-width/2, height, +length/2);
        glVertex3f(-width/2, height, -length/2);

        //right
        glVertex3f(width/2, 0, -length/2);
        glVertex3f(width/2, 0, +length/2);
        glVertex3f(width/2, height, +length/2);
        glVertex3f(width/2, height, -length/2);

        //front
        glVertex3f(-width/2, 0, -length/2);
        glVertex3f(+width/2, 0, -length/2);
        glVertex3f(+width/2, height, -length/2);
        glVertex3f(-width/2, height, -length/2);

        //front
        glVertex3f(-width/2, 0, length/2);
        glVertex3f(+width/2, 0, length/2);
        glVertex3f(+width/2, height, length/2);
        glVertex3f(-width/2, height, length/2);

        //top
        glVertex3f(-width/2, height, -length/2);
        glVertex3f(-width/2, height, +length/2);
        glVertex3f(+width/2, height, +length/2);
        glVertex3f(+width/2, height, -length/2);


    glEnd();
    glTranslatef(centerX, 0, -centerZ);
}

void roadBlock(float centerX, float centerZ, int direction){
    float segmentLength = 60;
    float segmentWidth = 15;
    float dividerWidth = 0.3;
    float margin = 0.5;
    float angle = 0;
    if(direction==1){
        angle = 90;
    }

    glTranslatef(-centerX, 0, centerZ);
    glRotatef(angle,0,1,0);
    glBegin(GL_QUADS);
        //base road;
        glColor3f(0.2176, 0.2078,0.2392);
        glVertex3f(-segmentWidth/2, 0.01, -segmentLength/2);
        glVertex3f(-segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, -segmentLength/2);

        //marking right
        glColor3f(1.0,1.0,1.0);
        glVertex3f(-segmentWidth/2 + margin, 0.03, -segmentLength/2);
        glVertex3f(-segmentWidth/2 + margin, 0.03, +segmentLength/2);
        glVertex3f(-segmentWidth/2 + margin + dividerWidth, 0.03, +segmentLength/2);
        glVertex3f(-segmentWidth/2 + margin + dividerWidth, 0.03, -segmentLength/2);
        //marking left
        glColor3f(1.0,1.0,1.0);
        glVertex3f(-(-segmentWidth/2 + margin), 0.03, -segmentLength/2);
        glVertex3f(-(-segmentWidth/2 + margin), 0.03, +segmentLength/2);
        glVertex3f(-(-segmentWidth/2 + margin + dividerWidth), 0.03, +segmentLength/2);
        glVertex3f(-(-segmentWidth/2 + margin + dividerWidth), 0.03, -segmentLength/2);

        //middle marking
        glColor3f(1.0,1.0,1.0);
        float start = -segmentLength/2+5;
        for(int i=0;i<5;i++){
            if(i%2==0){
                glVertex3f(-dividerWidth/2,0.03,start + i*10);
                glVertex3f(-dividerWidth/2,0.03,start + i*10 + 10);
                glVertex3f(+dividerWidth/2,0.03,start + i*10 + 10);
                glVertex3f(+dividerWidth/2,0.03,start + i*10);
            }
        }
    glEnd();
    glRotatef(-angle,0,1,0);
    glTranslatef(centerX, 0, -centerZ);
}

void sqJunction(float centerX, float centerZ){

    float segmentLength = 15;
    float segmentWidth = 15;
    float dividerWidth = 0.3;
    float margin = 0.5;

    glTranslatef(-centerX, 0, centerZ);
    glBegin(GL_QUADS);
        //base road;
        glColor3f(0.2176, 0.2078,0.2392);
        glVertex3f(-segmentWidth/2, 0.01, -segmentLength/2);
        glVertex3f(-segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, -segmentLength/2);

        float a = (margin+dividerWidth)/2;
        glColor3f(1,1,1);
        float x=0,y=0;

        x = (-segmentWidth/2)+a;
        y = (-segmentLength/2)+a;
        glVertex3f(-a+x, 0.02, -a+y);
        glVertex3f(-a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, -a+y);

        x = (-segmentWidth/2)+a;
        y = (segmentLength/2)-a;
        glVertex3f(-a+x, 0.02, -a+y);
        glVertex3f(-a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, -a+y);

        x = (segmentWidth/2)-a;
        y = (-segmentLength/2)+a;
        glVertex3f(-a+x, 0.02, -a+y);
        glVertex3f(-a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, -a+y);

        x = (segmentWidth/2)-a;
        y = (segmentLength/2)-a;
        glVertex3f(-a+x, 0.02, -a+y);
        glVertex3f(-a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, +a+y);
        glVertex3f(+a+x, 0.02, -a+y);

        //corner cutout
        a = (margin)/2;
        glColor3f(0.2176, 0.2078,0.2392);
        float tempy = 0.021;

        x = (-segmentWidth/2)+a;
        y = (-segmentLength/2)+a;
        glVertex3f(-a+x, tempy, -a+y);
        glVertex3f(-a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, -a+y);

        x = (-segmentWidth/2)+a;
        y = (segmentLength/2)-a;
        glVertex3f(-a+x, tempy, -a+y);
        glVertex3f(-a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, -a+y);

        x = (segmentWidth/2)-a;
        y = (-segmentLength/2)+a;
        glVertex3f(-a+x, tempy, -a+y);
        glVertex3f(-a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, -a+y);

        x = (segmentWidth/2)-a;
        y = (segmentLength/2)-a;
        glVertex3f(-a+x, tempy, -a+y);
        glVertex3f(-a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, +a+y);
        glVertex3f(+a+x, tempy, -a+y);

    glEnd();

    glTranslatef(centerX, 0, -centerZ);
}

void sideJunction(float centerX, float centerZ, int direction){

    float segmentLength = 15;
    float segmentWidth = 15;
    float dividerWidth = 0.3;
    float margin = 0.5;

    glTranslatef(-centerX, 0, centerZ);
    glBegin(GL_QUADS);
        //base road;
        glColor3f(0.2176, 0.2078,0.2392);
        glVertex3f(-segmentWidth/2, 0.01, -segmentLength/2);
        glVertex3f(-segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, -segmentLength/2);

        float a = (margin+dividerWidth)/2;
        glColor3f(1,1,1);
        float x=0,y=0;

        if(direction==0){
            //left
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);
            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = -(segmentWidth/2 - margin - dividerWidth);
            float xMax = -(segmentWidth/2 - margin);
            float zMin = -(segmentWidth/2);
            float zMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else if(direction==1){
            //right
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);
            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = (segmentWidth/2 - margin - dividerWidth);
            float xMax = (segmentWidth/2 - margin);
            float zMin = -(segmentWidth/2);
            float zMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else if(direction==2){
            //front
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);
            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float zMin = -(segmentWidth/2 - margin - dividerWidth);
            float zMax = -(segmentWidth/2 - margin);
            float xMin = -(segmentWidth/2);
            float xMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else{
            //back
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);
            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float zMin = (segmentWidth/2 - margin - dividerWidth);
            float zMax = (segmentWidth/2 - margin);
            float xMin = -(segmentWidth/2);
            float xMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);
        }

    glEnd();

    glTranslatef(centerX, 0, -centerZ);
}

void turnJunction(float centerX, float centerZ, int direction){

    float segmentLength = 15;
    float segmentWidth = 15;
    float dividerWidth = 0.3;
    float margin = 0.5;

    glTranslatef(-centerX, 0, centerZ);
    glBegin(GL_QUADS);
        //base road;
        glColor3f(0.2176, 0.2078,0.2392);
        glVertex3f(-segmentWidth/2, 0.01, -segmentLength/2);
        glVertex3f(-segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, +segmentLength/2);
        glVertex3f(+segmentWidth/2, 0.01, -segmentLength/2);

        float a = (margin+dividerWidth)/2;
        glColor3f(1,1,1);
        float x=0,y=0;

        if(direction==0){
            //TL
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);

            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = -((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = -(segmentWidth/2 - margin - dividerWidth);
            float xMax = -(segmentWidth/2 - margin);
            float zMin = -(segmentWidth/2)+margin;
            float zMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

            zMin = -(segmentWidth/2 - margin - dividerWidth);
            zMax = -(segmentWidth/2 - margin);
            xMin = -(segmentWidth/2)+margin;
            xMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else if(direction==1){
            //TR
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = ((-segmentWidth/2)+a);
            y = -((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = (segmentWidth/2 - margin - dividerWidth);
            float xMax = (segmentWidth/2 - margin);
            float zMin = -(segmentWidth/2)+margin;
            float zMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

            zMin = -(segmentWidth/2 - margin - dividerWidth);
            zMax = -(segmentWidth/2 - margin);
            xMin = -(segmentWidth/2);
            xMax = (segmentWidth/2)-margin;
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else if(direction==2){
            //BL
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);

            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = -((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = -(segmentWidth/2 - margin - dividerWidth);
            float xMax = -(segmentWidth/2 - margin);
            float zMin = (segmentWidth/2)-margin;
            float zMax = -(segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

            zMin = (segmentWidth/2 - margin - dividerWidth);
            zMax = (segmentWidth/2 - margin);
            xMin = -(segmentWidth/2)+margin;
            xMax = (segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

        }else{
            //BR
            a = (margin+dividerWidth)/2;
            glColor3f(1,1,1);

            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, 0.02, -a+y);
            glVertex3f(-a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, +a+y);
            glVertex3f(+a+x, 0.02, -a+y);

            a = (margin)/2;
            glColor3f(0.2176, 0.2078,0.2392);
            float tempy = 0.021;

            x = ((-segmentWidth/2)+a);
            y = ((-segmentLength/2)+a);
            glVertex3f(-a+x, tempy, -a+y);
            glVertex3f(-a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, +a+y);
            glVertex3f(+a+x, tempy, -a+y);

            //solid line
            glColor3f(1,1,1);
            float xMin = (segmentWidth/2 - margin - dividerWidth);
            float xMax = (segmentWidth/2 - margin);
            float zMin = (segmentWidth/2)-margin;
            float zMax = -(segmentWidth/2);
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);

            zMin = (segmentWidth/2 - margin - dividerWidth);
            zMax = (segmentWidth/2 - margin);
            xMin = -(segmentWidth/2);
            xMax = (segmentWidth/2)-margin;
            glVertex3f(xMin,0.02,zMin);
            glVertex3f(xMin,0.02,zMax);
            glVertex3f(xMax,0.02,zMax);
            glVertex3f(xMax,0.02,zMin);
        }
    glEnd();

    glTranslatef(centerX, 0, -centerZ);
}


void xzGrid(){

    //City Road Map
    roadBlock(0,0,0);
    sideJunction(0, 30+7.5, 0);

    //A
    roadBlock(-30-7.5, 30+7.5, 1);
    roadBlock(-30-7.5-60, 30+7.5, 1);
    roadBlock(-30-7.5-120, 30+7.5, 1);
    roadBlock(-30-7.5-120 - 15, 30+7.5, 1);
    sideJunction(-30-7.5-120 - 15 - 7.5 - 30, 30+7.5, 2);

    //B
    float shiftB = 210;
    roadBlock(-30-7.5 - shiftB, 30+7.5, 1);
    roadBlock(-30-7.5-60 - shiftB, 30+7.5, 1);
    roadBlock(-30-7.5-120 - shiftB, 30+7.5, 1);
    roadBlock(-30-7.5-120 - 15 - shiftB, 30+7.5, 1);
    sideJunction(-30-7.5-120 - 15 - 7.5 - 30 - shiftB, 30+7.5, 2);

    //C
    roadBlock(-420-7.5-30, 30+7.5, 1);
    turnJunction(-420-60-15, 30+7.5, 1);

    {
        //c1
        roadBlock(-420, 30+7.5+7.5+30, 0);
        sideJunction(-420, 30+7.5+7.5+60+7.5, 1);
        roadBlock(-420, 30+7.5+7.5+30+15+60, 0);
        turnJunction(-420, 30+7.5+7.5+30+15+60+7.5+30, 2);

        //c2
        roadBlock(-420-30-7.5, 30+7.5+7.5+30+15+60+7.5+30, 1);
    }

    //D
    roadBlock(0,30+15+30,0);
    sqJunction(0, 30+15+30+30+7.5);
    {
        //d1
        float shiftD = 120;
        roadBlock(0, shiftD+30,0);
        turnJunction(0,shiftD+60+7.5, 3);

        //d2
        roadBlock(30+7.5, shiftD+60+7.5, 1);
        turnJunction(60+15, shiftD+60+7.5, 2);

        //d3
        roadBlock(60+15, shiftD+30,0);
        turnJunction(60+15, shiftD-7.5, 0);

        //d4
        roadBlock(30+7.5, shiftD-7.5,1);
    }

    //E
    roadBlock(-30-7.5, 112.5, 1);
    roadBlock(-30-7.5-60, 112.5, 1);
    sideJunction(-30-7.5-60-7.5-30, 112.5, 2);
    roadBlock(-30-7.5-60-60-15, 112.5, 1);
    sideJunction(-30-7.5-60-60-15-30-7.5, 112.5, 3);
    roadBlock(-210-7.5-30, 112.5,1);
    sideJunction(-210-7.5-60-7.5, 112.5, 2);
    roadBlock(-210-7.5-60-15-30, 112.5, 1);
    roadBlock(-210-7.5-60-15-60-30, 112.5, 1);

    //F
    roadBlock(-30-7.5-60-60-15-30-7.5, 112.5-7.5-30, 0);

    //G
    roadBlock(-420-60-15, 30+7.5+7.5+30, 0);
    roadBlock(-420-60-15, 30+7.5+7.5+60+30, 0);
    roadBlock(-420-60-15, 30+7.5+7.5+60+15+30, 0);
    sideJunction(-420-60-15, 30+7.5+7.5+60+15+30+7.5+30, 1);
    roadBlock(-495, 157.5+7.5+30+30, 0);
    sideJunction(-495, 262.5, 1);
    roadBlock(-495, 262.5+7.5+30, 0);
    roadBlock(-495, 262.5+7.5+30+60, 0);
    sideJunction(-495, 262.5+7.5+60+60+7.5, 1);
    roadBlock(-495, 262.5+7.5+60+60+15+30, 0);
    turnJunction(-495, 262.5+7.5+60+60+15+7.5+60, 3);
    roadBlock(-495+30+7.5, 262.5+7.5+60+60+15+7.5+60, 1);

    //H
    roadBlock(-495+7.5+30, 262.5, 1);
    roadBlock(-495+7.5+30+60, 262.5, 1);
    roadBlock(-495+7.5+30+60+15, 262.5, 1);
    roadBlock(-495+7.5+30+60+15+60, 262.5, 1);
    sideJunction(-495+7.5+30+60+15+7.5+30+60,262.5, 0);

    //I1
    roadBlock(-210-7.5-60-7.5, 112.5+7.5+30,0);
    roadBlock(-210-7.5-60-7.5, 112.5+7.5+30+60,0);
    roadBlock(-210-7.5-60-7.5, 112.5+7.5+30+60+15,0);
    //junction build by H
    roadBlock(-210-7.5-60-7.5, 112.5+7.5+30+60+15+15+60,0);
    roadBlock(-210-7.5-60-7.5, 112.5+7.5+30+60+15+15+60+60,0);
    turnJunction(-210-7.5-60-7.5, 112.5+7.5+30+60+15+15+60+60+30+7.5,2);
    roadBlock(-495+7.5+30, 262.5 + 135, 1);
    sideJunction(-495+7.5+30+30+7.5, 262.5 + 135, 2);
    roadBlock(-495+7.5+30+60+15, 262.5 + 135, 1);
    roadBlock(-495+7.5+30+60+15+60, 262.5 + 135, 1);

    //I2
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30,0);
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30+60,0);
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30+60+15,0);
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30+60+15+30+15,0);
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30+60+15+15+60,0);
    roadBlock(-210-7.5-60-7.5+150, 112.5+7.5+30+60+15+15+60+60,0);
    turnJunction(-210-7.5-60-7.5+150, 112.5+7.5+30+60+15+15+60+60+30+7.5,3);

    //J -135, 397.5
    roadBlock(-135+7.5+30, 397.5, 1);
    sideJunction(-135+15+60, 397.5, 2);
    roadBlock(-135+15+60+7.5+30, 397.5, 1);
    turnJunction(-135+15+60+15+60, 397.5, 0);

    //K1
    roadBlock(-135+15+60, 397.5+7.5+30, 0);
    sideJunction(-135+15+60, 397.5+7.5+30+30+7.5, 1);
    roadBlock(-135+15+60, 397.5+7.5+30+30+7.5+7.5+30, 0);
    sideJunction(-135+15+60, 397.5+7.5+30+30+7.5+75, 0);
    roadBlock(-135+15+60, 397.5+7.5+30+30+7.5+7.5+30+75, 0);
    turnJunction(-135+15+60, 397.5+7.5+30+30+7.5+7.5+30+75+30+7.5, 2);

    //K2
    roadBlock(-135+15+60+75, 397.5+7.5+30, 0);
    sideJunction(-135+15+60+75, 397.5+7.5+30+30+7.5, 0);
    roadBlock(-135+15+60+75, 397.5+7.5+30+30+7.5+7.5+30, 0);
    roadBlock(-135+15+60+75, 397.5+7.5+30+30+7.5+7.5+30+60, 0);
    roadBlock(-135+15+60+75, 397.5+7.5+30+30+7.5+7.5+30+120, 0);

    //K3
    roadBlock(-135+15+60+7.5+30, 397.5+75, 1);

    //L
    roadBlock(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5, 0);
    sideJunction(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+30+7.5, 0);
    roadBlock(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+75, 0);
    sideJunction(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+30+7.5+75, 1);
    roadBlock(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+150, 0);
    sideJunction(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+30+7.5+150, 1);
    roadBlock(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+150+75, 0);
    roadBlock(-495+7.5+30+30+7.5, 262.5 + 135+30+7.5+150+75+60, 0);

    //M1 -420, 547.5
    roadBlock(-420+30+7.5, 547.5, 1);
    roadBlock(-420+30+7.5+60, 547.5, 1);
    roadBlock(-420+30+7.5+120, 547.5, 1);
    sideJunction(-420+30+7.5+120+7.5+30, 547.5, 2);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30, 547.5, 1);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30+60, 547.5, 1);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30+60+30, 547.5, 1);

    //M2 -420, 547.5
    roadBlock(-420+30+7.5, 547.5+75, 1);
    roadBlock(-420+30+7.5+60, 547.5+75, 1);
    roadBlock(-420+30+7.5+120, 547.5+75, 1);
    sideJunction(-420+30+7.5+120+7.5+30, 547.5+75, 3);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30, 547.5+75, 1);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30+60, 547.5+75, 1);
    roadBlock(-420+30+7.5+120+7.5+30+7.5+30+60+30, 547.5+75, 1);

    //M3
    roadBlock(-420+30+7.5+120+7.5+30, 547.5+7.5+30, 0);

    // City Building
    //1
    {
        building(25, 15+20, 10, -35, 55+10, 0.7, 0.7, 0.7);
        building(20, 15, 25, -80, 55, 0.4, 0.4, 0.4);
        building(20, 25, 13, -120, 60, 0.7, 0.4,0.1);
        building(50, 25, 20, -175, 60, 0.7, 0.3,0.7);
    }

    //2
    {
        building(50, 50, 20, 37.5, 150, 0,0,0.8);
    }
    
    //3
    {
        int heights3[] = {9,6,5,10,5,20,13,7,10,8,12,6,9,12,17};
        for(int i=0;i<13;i++){
            building(5, 10, heights3[i], -(i*15)-225, 55, (heights3[i]/20.0), (heights3[i]/20.0), (heights3[i]/20.0));
        }
        // int heights[] = {9,6,5,10,5,20,13,7,10,8,12,6,9,12,17};

        for(int i=0;i<13;i++){
            building(5, 10, heights3[12-i], -(i*15)-225, 55+40, (heights3[i]/20.0), (heights3[i]/20.0), (heights3[i]/20.0));
        }
    }

    //4
    {
        int heights4[] = {5,10,5,20,13,7,10,8,12,6,9,12,17};
        for(int i=0;i<13;i++){
            building(35, 8, heights4[i], -450, (i*10)+53, (heights4[i]/10.0), (heights4[12-i]/20.0), (heights4[i]/20.0));
        }
    }

    //5
    {
        int heights5[] = {13,7,10,8,12,6,9,12,17};
        for(int i=0;i<9;i++){
            building(50, 8, heights5[i], -450+10, (i*10)+53+240, (heights5[8-i]/17.0), (heights5[i]/20.0), (heights5[8-i]/20.0));
        }

        int height5[] = {5,5,9,7,12,8,10,6,15};
        for(int i=0;i<9;i++){
            building(60, 8, height5[i], -450+60+20, (i*10)+53+240, (height5[8-i]/17.0), (height5[i]/10.0), (height5[8-i]/17.0));
        }
    }

    //6
    {
        building(50, 50, 80, -20, 435, 0.4,4.0,0.0);
    }

    //7
    {
        building(50, 50, 30, -20, 435+90, 0.4,0.4,0.4);
    }

    //8
    {
        int heights8[] = {7,23,15,10,5,19,10,9,18};
        for(int i=0;i<6;i++){
            building(40, 8, heights8[i], -450, (i*10)+53+240+117, (heights8[i]/17.0), (heights8[i]/20.0), (heights8[8-i]/20.0));
        }
    }

    //9
    {
        building(90, 40, 15, -450+110, 580, 0.4,0.4,0.4);
        building(100, 40, 15, -450+110+180, 580, 0.5,0.2,0.2);
    }

    //Green Feilds
    float d = 1000;
    glColor3f(0,0.4,0);
    glBegin(GL_QUADS);
        glVertex3f(carCenterX-d, 0, carCenterZ-d);
        glVertex3f(carCenterX-d, 0, carCenterZ+d);
        glVertex3f(carCenterX+d, 0, carCenterZ+d);
        glVertex3f(carCenterX+d, 0, carCenterZ-d);
    glEnd();


}

void mouse(int button, int state, int x, int y){

}

void keyboard(unsigned char key, int x, int y){

}

void specialKeyboard(int key, int X, int y){
    float acc=0.009;
    if(key == GLUT_KEY_LEFT){
        //left turn
        if(isForward){
            carSpeed += acc;
            if(carSpeed>maxSpeed){
                carSpeed = maxSpeed;
            }
        }else{
            carSpeed -= acc;
            if(carSpeed<maxRevSpeed){
                carSpeed = maxRevSpeed;
            }
        }

        carDirection+=(carSpeed)*(5);
        if(carDirection>360)
            carDirection-=360;
    }
     
    if(key == GLUT_KEY_RIGHT){
        //right turn
        if(isForward){
            carSpeed += acc;
            if(carSpeed>maxSpeed){
                carSpeed = maxSpeed;
            }
        }else{
            carSpeed -= acc;
            if(carSpeed<maxRevSpeed){
                carSpeed = maxRevSpeed;
            }
        }

        carDirection-=(carSpeed)*(5);
        if(carDirection>360)
            carDirection-=360;
    }

    if(key == GLUT_KEY_UP){
        //forward
        if(carSpeed>=0){
            isForward = true;
        }
        carSpeed += acc;
        if(carSpeed>maxSpeed){
            carSpeed = maxSpeed;
        }
    }

    if(key == GLUT_KEY_DOWN){
        //reverse

        if(carSpeed<=0){
            isForward = false;
            carSpeed -= acc;
        }else{
            carSpeed -= 0.2;
        }

        if(carSpeed<maxRevSpeed){
            carSpeed = maxRevSpeed;
        }
    }

}

void initGL(){
    glClearColor(0.4,0.4,0.7,1.0);
    glClearDepth(1);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glShadeModel(GL_SMOOTH);
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
}

void idle(){
    glutPostRedisplay();
}

void reshape(GLsizei width, GLsizei height){
    if(!height)
        height = 1;
    GLfloat aspectRatio = (GLfloat)width/(GLfloat)height;

    glViewport(0,0,width,height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0f, aspectRatio, 0.1f, 1000.0f); // fovy; aspect; znear; zfar;
}
