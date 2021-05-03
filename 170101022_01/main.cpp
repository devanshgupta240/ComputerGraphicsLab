#include<iostream>
#include<vector>
#include<sstream>
#include<string.h>
#include<string>
#include <fstream>
#include <GL/glut.h>

using namespace std ; 

float Angle = 0.0;
float CamAngle = 45.0;

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
        glRotatef(CamAngle, 0.0f, 0.5f, 0.5f);
        glTranslatef(0.0f, 0.0f, -10.0f);
        glPushMatrix();
        glTranslatef(5.0f, -1.0f, 0.0f);
        glScalef(7.0f, 7.0f, 7.0f);
        glRotatef(Angle, 1.0f, 3.0f, 2.0f);
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

//Keep bunny position in centre when we resize screen
void fix(int width, int hieght){
    glViewport(0, 0, width, hieght);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45.0,(double)width/(double)hieght, 1.0, 100.0);
}

//Changes angle which results in Rotation
void rotateModel(int n){
	Angle += 1.0f;        //Decides Speed of Rotation
	if(Angle>360.f){
	    Angle-=360;
	}
	glutPostRedisplay();
	glutTimerFunc(50,rotateModel,0);
}

int main(int argc, char ** argv){
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
    glutInitWindowPosition(0,0);
    glutInitWindowSize(1000,1000); 
    glutCreateWindow("Rotating3DModel"); 
    glEnable(GL_DEPTH_TEST); 
    bunny.extractInfo("lowpolybunny.obj"); 
    glutDisplayFunc(display3DModel);
    glutTimerFunc(50,rotateModel,0);
    glutReshapeFunc(fix);
    glutMainLoop(); 
    return 0;
}