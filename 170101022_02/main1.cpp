#include<iostream>
#include<bits/stdc++.h>
#include<vector>
#include<sstream>
#include<string.h>
#include<string>
#include <fstream>
#include <GL/glut.h>

using namespace std ; 

float Angle = 0.0;
float CamAngle = 30.0;
float sdepth = -30.0;
float sphi=0.0, stheta=0.0;

int downX, downY;

bool leftButton = false, rightButton = false;


class mesh3D{
   struct vertex{
        double xCordinate;
        double yCordinate;
        double zCordinate;
   };
   struct surface{
       unsigned int v1,v2,v3;
   };
   vector<surface> surfaces;
   vector<vertex> vertices;
   
public:
    void extractInfo(const char* filename){
        string str;
        ifstream fin(filename);
        if (!fin)
            return;
        while (fin >> str){
            switch (*str.c_str()){
            case 'f':{
                        surface f;
                        fin >> f.v1 >> f.v2 >> f.v3;
                        surfaces.push_back(f);
                    }
                    break;
            case 'v':{
                        vertex v;
                        fin >> v.xCordinate >> v.yCordinate >> v.zCordinate;
                        this->vertices.push_back(v);
                    }
                    break;
            }
        }
    }
    void createModel(){
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        glTranslatef(0.0f, 0.0f, sdepth);
        glRotatef(-stheta,1.0f,0.0f,0.0f);
        glRotatef(sphi,0.0f,1.0f,0.0f);
        glPushMatrix();
        glTranslatef(1.0f, -3.0f, 0.0f);
        glScalef(20.0f,20.0f,20.0f);
        glBegin(GL_TRIANGLES);
            glColor3d(1, 1, 1);
            for(int i=0;i<int(surfaces.size());i++){
                vertex v1 = vertices[surfaces[i].v1-1];
                vertex v2 = vertices[surfaces[i].v2-1];
                vertex v3 = vertices[surfaces[i].v3-1];
                glVertex3d(v1.xCordinate,v1.yCordinate,v1.zCordinate);
                glVertex3d(v2.xCordinate,v2.yCordinate,v2.zCordinate);
                glVertex3d(v3.xCordinate,v3.yCordinate,v3.zCordinate);
            }
        glEnd();
        glFlush();
        glPopMatrix();
        glutSwapBuffers();
    }  
};

mesh3D bunny;
void display3DModel(){
    bunny.createModel();
}

void MouseCallback(int button, int state, int x, int y) 
{

    cout << x << " " << y << endl;
    downX = x; downY = y;

    leftButton = ((button == GLUT_LEFT_BUTTON) && (state == GLUT_DOWN));

    rightButton = ((button == GLUT_RIGHT_BUTTON) &&  (state == GLUT_DOWN));

    glutPostRedisplay();
} 

void MotionCallback(int x, int y) 
{

    if (leftButton) //rotate
    {
        sphi -= (float)(x-downX)/4.0;
        stheta -= (float)(downY-y)/4.0;
    }

    if (rightButton) //zooming
    {
        sdepth += (float)(downY - y) / 10.0;  
    }

    downX = x;   downY = y;

    glutPostRedisplay();
}

//Keep bunny position in centre when we resize screen
void fix(int width, int hieght){
    glViewport(0, 0, width, hieght);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(double)width/(double)hieght, 1.0, 100.0);
}



int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(80,80);
    glutInitWindowSize(600,600); 
    glutCreateWindow("3DModel"); 
    glEnable(GL_DEPTH_TEST); 
    bunny.extractInfo("lowpolybunny.obj"); 
    glutDisplayFunc(display3DModel);
    glutMouseFunc(MouseCallback);
    glutMotionFunc(MotionCallback); 
    glutReshapeFunc(fix);
    glutMainLoop(); 
    return 0;
}