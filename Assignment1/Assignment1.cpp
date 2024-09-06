//  ========================================================================
//  COSC363: Computer Graphics (2023);  University of Canterbury.
//
//  FILE NAME: Assignment1
//
//  Program displays the model of Ameswindow, Atom , Moire Pattern.
//
//  Use left/right arrow keys to turn left/right on the scene
//  Use up/down arrow keys to move camera forwaer
//  ========================================================================

#include <iostream>
#include <fstream>
#include <GL/freeglut.h>
#include <climits>
#include "loadTGA.h"
#include <cmath>
#include "loadBMP.h"

using namespace std;
int rotnWindow = 20;
float rotnAtom = 0;
float rotnElectronAxis = 0;


float theta = 0;
float shift = 0;
bool state = false;
float floor_height = -2;
GLuint txId[4];   //Texture ids
float light[4] = {0, 100, 0, 1}; // light pos xyz
float angle=0, look_x=0, look_z=0., eye_x=0, eye_z=700;  //View parameters


//-------------------Load glass texture---------------------------------
void loadTexture()
{
    glGenTextures(4, txId);     // Create 4 texture ids

    glBindTexture(GL_TEXTURE_2D, txId[0]);
    loadTGA("wall_1.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[1]);
    loadTGA("scan.tga");
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[2]);
    loadTGA("glass_texture.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    glBindTexture(GL_TEXTURE_2D, txId[3]);
    loadTGA("glass_texture.tga");
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);


}

void glass() {
        //Glass
    glColor4f(1.0, 1.0, 1.0, 0.9);   //The fourth component is the opacity term
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glBindTexture(GL_TEXTURE_2D, txId[2]);
    glBegin(GL_QUADS);
        glTexCoord2f(0, 0);  glVertex3f(-20, 0, 50);
        glTexCoord2f(1, 0);  glVertex3f(20, 0, 50);
        glTexCoord2f(1, 1);  glVertex3f(20, 5, -0);
        glTexCoord2f(0, 1);  glVertex3f(-20, 5, -0);
    glEnd();
    glDisable(GL_TEXTURE_2D);
}

//--Draws a grid of lines on the floor plane -------------------------------

void drawFloor() {
    {

    glColor4f(1, 1, 1,0.1);         //Floor colour

    bool flag = false;

    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);
    for(int x = -2000; x <= 2000; x += 20)
    {
        for(int z = -2000; z <= 2000; z += 20)
        {
            if(flag) glColor3f(0., .0, 0.);
            else glColor3f(1, 1.0, 1);
            glVertex3f(x, floor_height, z);
            glVertex3f(x, floor_height, z+20);
            glVertex3f(x+20, floor_height, z+20);
            glVertex3f(x+20, floor_height, z);
            flag = !flag;
        }
    }
    glEnd();

    glPopMatrix();
}
}
void moire() {


    for (int i=0; i<30; i++) {
        glPushMatrix();
            glBegin(GL_QUADS);
                glColor3f(0,0,0);
                glVertex2f(0,0.5+i*1);
                glVertex2f(0,0+i*1);
                glVertex2f(30,6.5+i*1);
                glVertex2f(30,7+i*1);
            glEnd();

            glTranslatef(3,4,30);
            glPushMatrix();
                glTranslatef(15,0,0);
                glRotatef(shift,0,1,0);
                glTranslatef(-15,0,0);
                glBegin(GL_QUADS);
                    glColor3f(0,0,0);
                    glVertex2f(0,0.5+i*1);
                    glVertex2f(0,0+i*1);
                    glVertex2f(30,2.5+i*1);
                    glVertex2f(30,3+i*1);
                glEnd();
            glPopMatrix();

        glPopMatrix();
    }



    glFlush();
}
void ameswindow() {

    glPushMatrix();
        glTranslatef(12.5,0.0,0.0); // translate the point to the middle.
        glRotatef(rotnWindow,0.0,1.0,0.0); //Rotate the ameswindow.
        glTranslatef(-12.5,0.0,0.0);


        glBegin(GL_QUADS);
            glColor3f(1.0f,1.0f,1.0f); //1
            glVertex2f(0.0f,3.9f);
            glVertex2f(0.0f,-3.9f);
            glVertex2f(0.8f,-4.0f);
            glVertex2f(0.8f,4.0f);

            glColor3f(1.0f,1.0f,1.0f); //2
            glVertex2f(0.8f,4.0f);
            glVertex2f(0.8f,3.1f);
            glVertex2f(21.3f,6.05f);
            glVertex2f(21.3f,7.7f);

            glColor3f(1.0f,1.0f,1.0f); //3
            glVertex2f(0.8f,0.4f);
            glVertex2f(0.8f,-0.4f);
            glVertex2f(21.3f,-0.8f);
            glVertex2f(21.3f,0.8f);

            glColor3f(1.0f,1.0f,1.0f); //4
            glVertex2f(0.8f,-3.1f);
            glVertex2f(0.8f,-4.0f);
            glVertex2f(21.3f,-7.7f);
            glVertex2f(21.3f,-6.05f);

            glColor3f(1.0f,1.0f,1.0f); //5
            glVertex2f(5.1f,4.0f);
            glVertex2f(5.1f,-4.0f);
            glVertex2f(6.2f,-4.0f);
            glVertex2f(6.2f,4.0f);

            glColor3f(1.0f,1.0f,1.0f); //6
            glVertex2f(11.8f,5.0f);
            glVertex2f(11.8f,-5.0f);
            glVertex2f(13.0f,-5.0f);
            glVertex2f(13.0f,5.0f);

            glColor3f(1.0f,1.0f,1.0f); //7
            glVertex2f(19.9f,7.0f);
            glVertex2f(19.9f,-7.0f);
            glVertex2f(21.3f,-7.0f);
            glVertex2f(21.3f,7.0f);

            glColor3f(0.0f,0.0f,1.0f); //8bluee

            glVertex2f(21.3f,7.7f);
            glVertex2f(21.3f,-7.7f);
            glVertex2f(23.0f,-7.1f);
            glVertex2f(23.0f,7.1f);


        glEnd();
        //bluu
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.0f,0.0f,1.0f); //1
            glVertex2f(0.8f,3.1f);
            glVertex2f(0.8f,0.4f);
            glVertex2f(1.7f,0.4f);
            glVertex2f(1.7f,2.9f);
            glVertex2f(5.1f,3.3f);
            glVertex2f(5.1f,3.8f);


        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,1.0f); //2
            glVertex2f(6.2f,4.0f);
            glVertex2f(6.2f,0.5f);
            glVertex2f(7.2f,0.5f);
            glVertex2f(7.2f,3.6f);
            glVertex2f(11.8f,4.1f);
            glVertex2f(11.8f,4.7f);
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,1.0f); //3
            glVertex2f(13.0f,4.9f);
            glVertex2f(13.0f,0.6f);
            glVertex2f(14.2f,0.6f);
            glVertex2f(14.2f,4.4f);
            glVertex2f(19.9f,5.1f);
            glVertex2f(19.9f,5.9f);
        glEnd();

        //bottom tri
        glBegin(GL_TRIANGLE_FAN);
            glColor3f(0.0f,0.0f,1.0f); //1
            glVertex2f(0.8f,-3.1f);
            glVertex2f(0.8f,-0.4f);
            glVertex2f(1.7f,-0.4f);
            glVertex2f(1.7f,-2.9f);
            glVertex2f(5.1f,-3.3f);
            glVertex2f(5.1f,-3.8f);


        glEnd();
        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,1.0f); //2
            glVertex2f(6.2f,-4.0f);
            glVertex2f(6.2f,-0.5f);
            glVertex2f(7.2f,-0.5f);
            glVertex2f(7.2f,-3.6f);
            glVertex2f(11.8f,-4.1f);
            glVertex2f(11.8f,-4.7f);
        glEnd();

        glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,1.0f); //3
            glVertex2f(13.0f,-4.9f);
            glVertex2f(13.0f,-0.6f);
            glVertex2f(14.2f,-0.6f);
            glVertex2f(14.2f,-4.4f);
            glVertex2f(19.9f,-5.1f);
            glVertex2f(19.9f,-5.9f);
        glEnd();

    glPopMatrix();
}
void ameswindowshadow() {
            glTranslatef(12.5,0.0,0.0); // translate the point to the middle.
        glRotatef(rotnWindow,0.0,1.0,0.0); //Rotate the ameswindow.
        glTranslatef(-12.5,0.0,0.0);


        glBegin(GL_QUADS);
            glColor3f(0.0f,0.0f,0.0f); //1
            glVertex2f(0.0f,3.9f);
            glVertex2f(0.0f,-3.9f);
            glVertex2f(0.8f,-4.0f);
            glVertex2f(0.8f,4.0f);

            glColor3f(0.0f,0.0f,0.0f); //2
            glVertex2f(0.8f,4.0f);
            glVertex2f(0.8f,3.1f);
            glVertex2f(21.3f,6.05f);
            glVertex2f(21.3f,7.7f);

            glColor3f(0.0f,0.0f,0.0f); //3
            glVertex2f(0.8f,0.4f);
            glVertex2f(0.8f,-0.4f);
            glVertex2f(21.3f,-0.8f);
            glVertex2f(21.3f,0.8f);

            glColor3f(0.0f,0.0f,0.0f); //4
            glVertex2f(0.8f,-3.1f);
            glVertex2f(0.8f,-4.0f);
            glVertex2f(21.3f,-7.7f);
            glVertex2f(21.3f,-6.05f);

            glColor3f(0.0f,0.0f,0.0f); //5
            glVertex2f(5.1f,4.0f);
            glVertex2f(5.1f,-4.0f);
            glVertex2f(6.2f,-4.0f);
            glVertex2f(6.2f,4.0f);

            glColor3f(0.0f,0.0f,0.0f); //6
            glVertex2f(11.8f,5.0f);
            glVertex2f(11.8f,-5.0f);
            glVertex2f(13.0f,-5.0f);
            glVertex2f(13.0f,5.0f);

            glColor3f(0.0f,0.0f,0.0f); //7
            glVertex2f(19.9f,7.0f);
            glVertex2f(19.9f,-7.0f);
            glVertex2f(21.3f,-7.0f);
            glVertex2f(21.3f,7.0f);

            glColor3f(0.0f,0.0f,0.0f); //8 black


            glVertex2f(21.3f,7.7f);
            glVertex2f(21.3f,-7.7f);
            glVertex2f(23.0f,-7.1f);
            glVertex2f(23.0f,7.1f);

                glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,0.0f); //1
        glVertex2f(0.8f,3.1f);
        glVertex2f(0.8f,0.4f);
        glVertex2f(1.7f,0.4f);
        glVertex2f(1.7f,2.9f);
        glVertex2f(5.1f,3.3f);
        glVertex2f(5.1f,3.8f);


    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f,0.0f,0.0f); //2
        glVertex2f(6.2f,4.0f);
        glVertex2f(6.2f,0.5f);
        glVertex2f(7.2f,0.5f);
        glVertex2f(7.2f,3.6f);
        glVertex2f(11.8f,4.1f);
        glVertex2f(11.8f,4.7f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f,0.0f,0.0f); //3
        glVertex2f(13.0f,4.9f);
        glVertex2f(13.0f,0.6f);
        glVertex2f(14.2f,0.6f);
        glVertex2f(14.2f,4.4f);
        glVertex2f(19.9f,5.1f);
        glVertex2f(19.9f,5.9f);
    glEnd();

    //bottom tri
    glBegin(GL_TRIANGLE_FAN);
        glColor3f(0.0f,0.0f,0.0f); //1
        glVertex2f(0.8f,-3.1f);
        glVertex2f(0.8f,-0.4f);
        glVertex2f(1.7f,-0.4f);
        glVertex2f(1.7f,-2.9f);
        glVertex2f(5.1f,-3.3f);
        glVertex2f(5.1f,-3.8f);


    glEnd();
    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f,0.0f,0.0f); //2
        glVertex2f(6.2f,-4.0f);
        glVertex2f(6.2f,-0.5f);
        glVertex2f(7.2f,-0.5f);
        glVertex2f(7.2f,-3.6f);
        glVertex2f(11.8f,-4.1f);
        glVertex2f(11.8f,-4.7f);
    glEnd();

    glBegin(GL_TRIANGLE_FAN);
    glColor3f(0.0f,0.0f,0.0f); //3
        glVertex2f(13.0f,-4.9f);
        glVertex2f(13.0f,-0.6f);
        glVertex2f(14.2f,-0.6f);
        glVertex2f(14.2f,-4.4f);
        glVertex2f(19.9f,-5.1f);
        glVertex2f(19.9f,-5.9f);
    glEnd();
}
void atom() {
    glPushMatrix();
        glRotatef(rotnAtom, 1, 1, 10);
        //atom
        glPushMatrix();  //!!

            glColor3f(1,0,0);//red
            glTranslatef(0.0, -1.0, 0.0);
            glutSolidSphere(2, 36, 12);

            glColor3f(0,0,1);//blue
            glTranslatef(1.0, 0.0, 0.0);
            glutSolidSphere(2, 36, 12);

            glColor3f(0,0,1);//blue
            glTranslatef(-1.0, 0.0, 0.0);
            glutSolidSphere(2, 36, 12);

            glColor3f(1,0,0);//red
            glTranslatef(0.0, 1.0, 0.0);
            glutSolidSphere(2, 36, 12);

            glColor3f(0,0,1);//blue
            glTranslatef(0.0, 0.0, 1.0);
            glutSolidSphere(2, 36, 12);

            glColor3f(1,0,0);//red
            glTranslatef(0.0, 0.0, -1.0);
            glutSolidSphere(2, 36, 12);


            glRotatef(rotnAtom, 1, 1, 1);
            //creating electrons and rotating. z axis

            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 0, 1);
                glTranslatef(0.0, 10.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);
            glPopMatrix();

            // creating ring
            glColor3f(0,0,0); //black ring
            glutSolidTorus(0.05, 10, 99, 99);
            //2nd electron

            //creating electrons and rotating. x axis
            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 1, 0);
                glTranslatef(10.0, 0.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);

            glPopMatrix();
            // creating ring

            glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
            glColor3f(0,0,0); //black ring
            glutSolidTorus(0.05, 10, 99, 99);

            // 2nd state of electron
            glRotatef(90,1,1,0);

            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 0, 1);
                glTranslatef(0.0, 30.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);
            glPopMatrix();
            //3nd
            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 0, 1);
                glTranslatef(0.0, -30.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);
            glPopMatrix();
            // creating ring
            glColor3f(0,0,0); //black ring
            glutSolidTorus(0.1, 30, 99, 99);
            //2nd electron

            //creating electrons and rotating. x axis
            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 1, 0);
                glTranslatef(30.0, 0.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);
                // double electron
                glTranslatef(0,0,5);
                glutSolidSphere(1, 36, 12);
            glPopMatrix();
            //test
            glPushMatrix();
                glRotatef(rotnElectronAxis, 0, 1, 0);
                glTranslatef(-30.0, 0.0, 0.0);
                glColor3f(1,1,0);
                glutSolidSphere(1, 36, 12);
                // double electron
                glTranslatef(0,0,5);
                glutSolidSphere(1, 36, 12);
            glPopMatrix();
            // creating ring

            glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
            glColor3f(0,0,0); //black ring
            glutSolidTorus(0.1, 30, 99, 99);
        glPopMatrix(); //!!
    glPopMatrix(); //!!
}
void sky() { //1
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[1]);
    glColor3f(1, 1, 1);
    ////////////////////// sky ///////////////////////


    glBegin(GL_QUADS);
    glNormal3f(0, 1, 0);

            glTexCoord2f(0, 0);
            glVertex3f(-150, floor_height+10, 0);
            glTexCoord2f(1, 0);
            glVertex3f(350, floor_height+10, 00);
            glTexCoord2f(1, 1);
            glVertex3f(350, floor_height+1000, -2000);
            glTexCoord2f(0, 1);
            glVertex3f(0, floor_height+1000, -2000);



    glEnd();

    glDisable(GL_TEXTURE_2D);
}

//---- Draws two straight walls ------
void room()
{
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txId[0]);
    glColor3f(0.4, 0.4, 0.9);
    ////////////////////// FRONT WALL ///////////////////////
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
        glVertex3f(100, 0, 0);
        glTexCoord2f(2, 0);
        glVertex3f(150, 0, 0);
        glTexCoord2f(2, 2);
        glVertex3f(150, 50, 0);
        glTexCoord2f(0, 2);
        glVertex3f(100, 50, 0);
    glEnd();
    glDisable(GL_TEXTURE_2D);

    ////////////////////// FRONT WALL ///////////////////////
    glColor3f(0., 2.55, 0);
    glNormal3f(0, 0, 1);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
        glVertex3f(-150, 0, 0);
        glTexCoord2f(5, 0);
        glVertex3f(100, 0, 0);
        glTexCoord2f(5, 2);
        glVertex3f(100, 50, 0);
        glTexCoord2f(0, 2);
        glVertex3f(-150, 50, 0);
    glEnd();


    ////////////////////// LEFT WALL ///////////////////////
    glColor3f(0.1, 0.1, 0.6);
    glNormal3f(1, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-150, 0, 300);
    glTexCoord2f(9, 0);
    glVertex3f(-150, 0, -0);
    glTexCoord2f(9, 3);
    glVertex3f(-150, 50, -0);
    glTexCoord2f(0, 3);
    glVertex3f(-150, 50, 300);
    glEnd();
    ////////////////////// Right WALL ///////////////////////
    glNormal3f(1, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(150, 0, 300);
    glTexCoord2f(9, 0);
    glVertex3f(150, 0, -0);
    glTexCoord2f(9, 3);
    glVertex3f(150, 50, -0);
    glTexCoord2f(0, 3);
    glVertex3f(150, 50, 300);
    glEnd();
    ////////////////////// floor WALL ///////////////////////
    glColor3f(0.8,0.2,0.2);
    glNormal3f(1, 0, 0);
    glBegin(GL_QUADS);
    glTexCoord2f(0, 0);
    glVertex3f(-150, 0, 300);
    glTexCoord2f(9, 0);
    glVertex3f(150, 0, 300);
    glTexCoord2f(9, 3);
    glVertex3f(150, 0, 0);
    glTexCoord2f(0, 3);
    glVertex3f(-150, 0, 0);
    glEnd();

    glDisable(GL_TEXTURE_2D);
}
void sun() {
    glPushMatrix();
    //~ glEnable(GL_TEXTURE_2D);
    //~ q = gluNewQuadric();
    //~ gluQuadricDrawStyle (q, GLU_FILL );
    //~ gluQuadricNormals   (q, GLU_SMOOTH );
    //~ glEnable(GL_TEXTURE_2D);
    //~ gluQuadricTexture (q, GL_TRUE);

    //Sun
    //~ glBindTexture(GL_TEXTURE_2D, txId[4]);
    //~ glTexEnvi(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);

    glPushMatrix();
        glColor3f(1,1,0);
        glRotatef(-90., 1.0, 0., 0.0);   //make the sphere axis vertical
        glutSolidSphere ( 4.0, 36, 17 );
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glPopMatrix();

}
void display() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(eye_x, 20, eye_z, look_x, 10, look_z, 0, 1, 0); // init camera pos
    float shadowMat[16] = {light[1], 0, 0, 0, -light[0], 0, -light[2],-1, 0, 0, light[1], 0, 0, 0, 0, light[1]};

    glLightfv(GL_LIGHT0, GL_POSITION, light);   //light position
    drawFloor();

    ////AmesWindow
    glTranslatef(80,20,150);
    ameswindow();
    glTranslatef(-80,-20,-150);

    glDisable(GL_LIGHTING);
    glColor3f(0., 0., 0.); //shadow
    glTranslatef(0,1,0);
    glPushMatrix();

        glMultMatrixf(shadowMat);
        glTranslatef(80, 20, 150);

        //glRotatef(rotnWindow, 0, 1, 0);
        ameswindowshadow();
    glPopMatrix();
    glTranslatef(0,-1,0);
    ////Sun

    glTranslatef(0,80,10);
    glPushMatrix();
        sun();

    glPopMatrix();
    glTranslatef(0,-80,-10);
    ////Atom
    glTranslatef(0,5,20);
        glEnable(GL_LIGHTING);

        glLightfv(GL_LIGHT0,GL_POSITION, light);
        glEnable(GL_LIGHTING);
        glColor3f(0, 1, 1);
        glPushMatrix();
            glTranslatef(0, 10, 10);
            glRotatef(theta, 1, 1, 0);
            glScalef(0.4,0.4,0.4);
            atom();
        glPopMatrix();
        ////shadow
        glDisable(GL_LIGHTING);
        glColor3f(0.1, 0.1, 0.1);
        glPushMatrix();
            glMultMatrixf(shadowMat);
            glTranslatef(0, 10, 10);
            glRotatef(theta, 1, 1, 0);
            glScalef(0.4,0.4,0.4);
            atom();
        glPopMatrix();

    glTranslatef(0,-5,-20);
    ////
    ////moire///
    glTranslatef(-140,0,160);
    glRotatef(90,0,1,0);
    moire();
    glRotatef(-90,0,1,0);
    glTranslatef(140,-0,-160);
    ///////

    //glass();
    room();
    glass();
    //sky();
    glutSwapBuffers();

}

//------- Initialize OpenGL parameters -----------------------------------
void initialize()
{
    loadTexture();

    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);   //Background colour

    glEnable(GL_LIGHTING);                  //Enable OpenGL states
    glEnable(GL_LIGHT0);
    glEnable(GL_COLOR_MATERIAL);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_NORMALIZE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    gluPerspective(30, 1, 30, 1000);  //The camera view volume
}

//------------ Special key event callback ---------------------------------
// To enable the use of left and right arrow keys to rotate the scene
void camera_mov(int key, int x, int y)
{

    if(key == GLUT_KEY_LEFT) angle -= 0.1;  //Change direction
    else if(key == GLUT_KEY_RIGHT) angle += 0.1;
        else if(key == GLUT_KEY_DOWN)
    {  //Move backward
        eye_x -= 1*sin(angle);
        eye_z += 1*cos(angle);
    }
    else if(key == GLUT_KEY_UP)
    { //Move forward
        eye_x += 2*sin(angle);
        eye_z -= 2*cos(angle);
    }

    look_x = eye_x + 9999*sin(angle);
    look_z = eye_z -9999*cos(angle);

    glutPostRedisplay();
}
void myTimer(int value)
{
    theta++;
    rotnWindow++;
    glutPostRedisplay();
    glutTimerFunc(20, myTimer, 0);
}
void atomtimer(int value)
{
    rotnAtom += 1;
    if(rotnAtom > 360) rotnAtom = 0;

    glutTimerFunc(50, atomtimer, value);
    glutPostRedisplay();
}
void electrontimer(int value)
{
    rotnElectronAxis += 1;
    if(rotnElectronAxis > 360) rotnElectronAxis = 0;
    glutTimerFunc(10, electrontimer, value);
    glutPostRedisplay();
}
void camera_pos (unsigned char key, int x, int y) {
    if(key == '0') {//Gallery View
        look_x=0, look_z=0., eye_x=0, eye_z=700; angle =0;

    }
    if(key == '1') {//AAO-1 View Ameswindow
        look_x=150, look_z=150., eye_x=0, eye_z=150;angle =1.5;
    }
    if(key == '2') {//AAO-2 View Atom
        look_x=0, look_z=0., eye_x=0, eye_z=100;angle =0;

    }
    if(key == '3') {//AAO-3 View Moire
        look_x=-150, look_z=150., eye_x=0, eye_z=150;angle =-1.5;
    }
    glutPostRedisplay();
}
void pattern_mov(int value)
{
    if (state == false) {
        shift --;
    }
    else if (state == true ) {
        shift ++;
    }
    if (shift == -360) {
        state = true;
    }
    if (shift == 360) {
        state = false;
    }

    glutPostRedisplay();
    glutTimerFunc(50, pattern_mov, 0);

}
//  ------- Main: Initialize glut window and register call backs -----------
int main(int argc, char** argv)
{
   glutInit(&argc, argv);
   glutInitDisplayMode (GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (1000, 1000);
   glutInitWindowPosition (10, 10);
   glutCreateWindow ("Assignment1");
   initialize();

   glutDisplayFunc(display);
   glutSpecialFunc(camera_mov);
   glutKeyboardFunc(camera_pos);
   glutTimerFunc(10, myTimer, 0);
   glutTimerFunc(50, atomtimer,0);
   glutTimerFunc(50,pattern_mov,0);
   glutTimerFunc(50, electrontimer,0);
   glutMainLoop();
   return 0;
}
