/*==================================================================================
* COSC 363  Computer Graphics (2023)
* Department of Computer Science and Software Engineering, University of Canterbury.
*
* A basic ray tracer
* See Lab06.pdf   for details.
*===================================================================================
*/
#include <iostream>
#include <cmath>
#include <vector>
#include <glm/glm.hpp>
#include "Sphere.h"
#include "SceneObject.h"
#include "Ray.h"
#include <GL/freeglut.h>
#include "Plane.h"
#include "TextureBMP.h"

using namespace std;
TextureBMP texture,texture1;


const float EDIST = 40.0;
const int NUMDIV = 800;
const int MAX_STEPS = 5;
const float XMIN = -20.0;
const float XMAX = 20.0;
const float YMIN = -20.0;
const float YMAX = 20.0;
bool fog_state = false;
bool anti_aliasing = false;
vector<SceneObject*> sceneObjects;


//---The most important function in a ray tracer! ----------------------------------
//   Computes the colour value obtained by tracing a ray and finding its
//     closest point of intersection with objects in the scene.
//----------------------------------------------------------------------------------
glm::vec3 trace(Ray ray, int step)
{
    glm::vec3 backgroundCol(0);						//Background colour = (0,0,0)
     glm::vec3 lightPos(15, 4, -12);					//Light's position
    glm::vec3 color(0);
    SceneObject* obj;

    ray.closestPt(sceneObjects);					//Compare the ray with all objects in the scene
    if(ray.index == -1) return backgroundCol;		//no intersection
    obj = sceneObjects[ray.index];					//object on which the closest point of intersection is found
    //checkered floor pattern
    if (ray.index == 0 ) //4 hardcoded need fixing for index of plane
    {
         //checkered pattern
         int stripeWidth = 4;
         int ix = (ray.hit.x+20) / stripeWidth;
         int iz = (ray.hit.z) / stripeWidth;
         int k = (iz+ix) % 2; //2 colors
         if (k == 0) color = glm::vec3(0, 0, 0);
         else color = glm::vec3(1, 1, 1);
         obj->setColor(color);
    }
    // Procedural Pattern
    if (ray.index == 8) {

        float waveFrequency = 1.5;
        float waveAmplitude = 1.0;
        float wave = sin(ray.hit.x * waveFrequency) * waveAmplitude;
        int iz = (ray.hit.z) + wave;
        int k = ( iz) % 4; // 2 colors
        if (k == 0) {
            color = glm::vec3(0, 0, 1);
        }
        else
            color = glm::vec3(0, 1, 0);
        obj->setColor(color);
    }
    // Diagonal Pattern
    if ( ray.index == 1) {

        float waveFrequency = 1.5;
        float waveAmplitude = 1.0;
        float wave = (ray.hit.x * waveFrequency) * waveAmplitude;
        int iz = (ray.hit.z) + (wave);
        int k = ( iz) % 3; // 2 colors
        if (k == 0) {
            color = glm::vec3(0.4, 0., 0.6);
        }
        else
            color = glm::vec3(1, 1, 0);
        obj->setColor(color);
    }

    //sphere texture
    if (ray.index == 12 )
    {
        obj->setColor(color);
        glm::vec3 normalise = obj->normal(ray.hit);

         float texcoordsu = 0.5 + std::atan2(normalise.z, normalise.x)/(2*M_PI);
         float texcoordtv = 0.5 + std::asin(normalise.y)/(M_PI);
         color=texture.getColorAt(texcoordsu, texcoordtv);
         obj->setColor(color);
    }



    color = obj->getColor();						//Object's colour
    color = obj -> lighting(lightPos,-ray.dir,ray.hit);

    //shadow
    glm::vec3 lightVec = lightPos - ray.hit;
    Ray shadowRay(ray.hit, lightVec);
    shadowRay.closestPt(sceneObjects);
    SceneObject* shadow_obj = sceneObjects[shadowRay.index];

    shadowRay.closestPt(sceneObjects);
    int numRays = 20;
    float shadow = 0.0;
    if (shadowRay.index > -1 && shadowRay.dist < glm::length(lightVec)) {

        for (int i = 0; i < numRays; i++) {
            float u = (float)rand() / RAND_MAX; // Random value between 0 and 1
            float v = (float)rand() / RAND_MAX;
            glm::vec3 randomPoint = lightPos + glm::vec3(u, v, 0.0);

            glm::vec3 lightVec = randomPoint - ray.hit;
            Ray shadowRay(ray.hit, lightVec);
            shadowRay.closestPt(sceneObjects);
            SceneObject* shadow_obj = sceneObjects[shadowRay.index];

            if ((shadowRay.index > -1) && (shadowRay.dist < glm::length(lightVec))) {
                if (shadow_obj->isRefractive() || shadow_obj->isTransparent()) {
                    shadow += 0.3f;
                } else {
                    shadow += 0.5f;
                }
            }
        }
        shadow /= numRays;
    }

    color = (1.0f - shadow) * color;
    // Reflective
    if (obj->isReflective() && step < MAX_STEPS)   //reflective
    {
        float rho = obj->getReflectionCoeff();
        glm::vec3 normalVec = obj->normal(ray.hit);
        glm::vec3 reflectedDir = glm::reflect(ray.dir, normalVec);
        Ray reflectedRay(ray.hit, reflectedDir);
        glm::vec3 reflectedColor = trace(reflectedRay, step + 1);
        color = color + (rho * reflectedColor);
    }
    // Transparency
    if (obj->isTransparent() && step < MAX_STEPS)
    {

        // Internal ray
        Ray internalRay(ray.hit, ray.dir);
        internalRay.closestPt(sceneObjects);
        glm::vec3 internalCol = trace(internalRay, step + 1);
        // External ray
        Ray externalRay(internalRay.hit, ray.dir);
        externalRay.closestPt(sceneObjects);
        glm::vec3 externalCol = trace(externalRay, step + 1);
        color = (color * (1 - obj->getTransparencyCoeff())) + (externalCol * obj->getTransparencyCoeff());
    }
    // Refractions
        if (obj->isRefractive() && step < MAX_STEPS)
        {
            float eta = 1.0f / obj->getRefractiveIndex();

            // Internal refeaction
            glm::vec3 normalVec = obj->normal(ray.hit);
            glm::vec3 refractedDir = glm::refract(ray.dir, normalVec, eta);
            Ray refrRay(ray.hit, refractedDir);
            refrRay.closestPt(sceneObjects);

            // External refraction
            glm::vec3 refractedNormalVec = obj->normal(refrRay.hit);
            glm::vec3 secondRefractedDir = glm::refract(refractedDir, -refractedNormalVec, 1.0f / eta);

            Ray refrRay2(refrRay.hit, secondRefractedDir);

            glm::vec3 refractedColor = trace(refrRay2, step + 1);
            color = (color * (1 - obj->getRefractionCoeff())) +  (refractedColor * obj->getRefractionCoeff());
        }


        //Fog
        if (fog_state == true ) {
            float gamma = ((ray.hit.z) - (-40)) / (-120-(-40));
            color = (1-gamma)*color + gamma * glm::vec3(1,1,1);
        }

    return color;
}

//---The main display module -----------------------------------------------------------
// In a ray tracing application, it just displays the ray traced image by drawing
// each cell as a quad.
//---------------------------------------------------------------------------------------
void display()
{
    float xp, yp;  // Grid point
    float cellX = (XMAX - XMIN) / NUMDIV;  // Cell width
    float cellY = (YMAX - YMIN) / NUMDIV;  // Cell height
    int numSubPixels = 2; // Number of sub-pixels per dimension

    glm::vec3 eye(0., 0., 0.);

    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glBegin(GL_QUADS);  // Each cell is a tiny quad.
    for (int i = 0; i < NUMDIV; i++)   // Scan every cell of the image plane
    {
        xp = XMIN + i * cellX;
        for (int j = 0; j < NUMDIV; j++)
        {
            yp = YMIN + j * cellY;

            glm::vec3 col(0);

            if (anti_aliasing) {
                for (int si = 0; si < numSubPixels; si++)
                {
                    for (int sj = 0; sj < numSubPixels; sj++)
                    {
                        float x = xp + (si + 0.5) * (cellX / numSubPixels);
                        float y = yp + (sj + 0.5) * (cellY / numSubPixels);
                        glm::vec3 dir(x + 0.5 * cellX, y + 0.5 * cellY, -EDIST);  // Direction of the primary ray
                        Ray ray(eye, dir);
                        glm::vec3 subCol = trace(ray, 1);  // Trace the primary ray and get the color value
                        col += subCol;
                    }
                }

                col /= (numSubPixels * numSubPixels);  // Average the accumulated color values
            }
            else {
                glm::vec3 dir(xp + 0.5 * cellX, yp + 0.5 * cellY, -EDIST);  // Direction of the primary ray
                Ray ray(eye, dir);
                col = trace(ray, 1);  // Trace the primary ray and get the color value
            }

            glColor3f(col.r, col.g, col.b);
            glVertex2f(xp, yp);              // Draw each cell with its color value
            glVertex2f(xp + cellX, yp);
            glVertex2f(xp + cellX, yp + cellY);
            glVertex2f(xp, yp + cellY);
        }
    }

    glEnd();
    glFlush();
}




void roomobjects() {

    Sphere *sphere1 = new Sphere(glm::vec3(-4.0, -6.0, -40.0), 4.0);
    sphere1->setColor(glm::vec3(0, 0, 0));   //Set colour to white
    sceneObjects.push_back(sphere1);		 //Add sphere to scene objects
    sphere1->setReflectivity(true,1); //l7
    //refractive
    Sphere *sphere2 = new Sphere(glm::vec3(2.0, -8.0, -35.0), 2.0);
    sphere2->setColor(glm::vec3(0, 0, 0));
    sceneObjects.push_back(sphere2);
    sphere2->setRefractivity(true, 1,1.5);


    //Transparent
    Sphere *sphere3 = new Sphere(glm::vec3(8.0, -5.0, -40.0), 5.0);
    sphere3->setColor(glm::vec3(0.6, 1, 1));
    sceneObjects.push_back(sphere3);
    sphere3->setTransparency(true);
    sphere3->setSpecularity(true);

    //ball
    Sphere *sphere4 = new Sphere(glm::vec3(-8.0, -7.0, -35.0), 3.0);
    sphere4->setColor(glm::vec3(0, 1, 1));   //Set colour to cyan
    sceneObjects.push_back(sphere4);		 //Add sphere to scene objects

}
//---This function initializes the scene -------------------------------------------
//   Specifically, it creates scene objects (spheres, planes, cones, cylinders etc)
//     and add them to the list of scene objects.
//   It also initializes the OpenGL 2D orthographc projection matrix for drawing the
//     the ray traced image.
//----------------------------------------------------------------------------------
void initialize()
{

   texture = TextureBMP("ball.bmp");
//   texture1 = TextureBMP("ball.bmp");
    glMatrixMode(GL_PROJECTION);
    gluOrtho2D(XMIN, XMAX, YMIN, YMAX);

    glClearColor(0, 0, 0, 1);

    //floor
    Plane *floorplane = new Plane (glm::vec3(-20., -12.4, -0), //Point A
     glm::vec3(20., -12.4, -0), //Point B
     glm::vec3(20., -12.4, -120), //Point C
     glm::vec3(-20., -12.4, -120)); //Point D
    floorplane->setColor(glm::vec3(0., 0., 0));
    sceneObjects.push_back(floorplane);
    floorplane->setSpecularity(false);

    //desk
    Plane *desk = new Plane (glm::vec3(-10., -10., -30), //Point A
     glm::vec3(10., -10., -30), //Point B
     glm::vec3(10., -10., -50), //Point C
     glm::vec3(-10., -10., -50)); //Point D
    desk->setColor(glm::vec3(1., 1., 1.));
    sceneObjects.push_back(desk);
    desk ->setSpecularity(false);

    //farplane120
    Plane *farplane = new Plane (glm::vec3(-20., -12.4, -90), //Point A
     glm::vec3(20., -12.4, -90), //Point B
     glm::vec3(20., 12.4, -90), //Point C
     glm::vec3(-20., 12.4, -90)); //Point D
    farplane->setColor(glm::vec3(0., 1, 0));
    sceneObjects.push_back(farplane);
    farplane->setSpecularity(false);
    //mirror
    Plane *mirror = new Plane (glm::vec3(-8.2, -5, -80), //Point A
                               glm::vec3(8.2, -5, -80), //Point B
                               glm::vec3(8.2, 5, -80), //Point C
                               glm::vec3(-8.2, 5, -80)); //Point D
  mirror->setColor(glm::vec3(0., 0., 0));
  sceneObjects.push_back(mirror);
  mirror->setReflectivity(true);

  //mirror
  Plane *mirror1 = new Plane (glm::vec3(8.1, -5, 1), //Point A
                             glm::vec3(-8.1, -5, 1), //Point B
                             glm::vec3(-8.1, 5, 1), //Point C
                             glm::vec3(8.1, 5, 1)); //Point D
    mirror1->setColor(glm::vec3(0., 0., 0));
    sceneObjects.push_back(mirror1);
    mirror1->setReflectivity(true);


    //backplane
    Plane *backplane = new Plane (glm::vec3(-20., 12.4, 2), //Point A
     glm::vec3(20., 12.4, 2), //Point B
     glm::vec3(20., -12.4, 2), //Point C
     glm::vec3(-20., -12.4, 2)); //Point D
    backplane->setColor(glm::vec3(1., 1, 0));
    sceneObjects.push_back(backplane);
    backplane->setSpecularity(false);

    //leftplane
    Plane *leftplane = new Plane (glm::vec3(-20., -12.4, -0), //Point A
     glm::vec3(-20., -12.4, -120), //Point B
     glm::vec3(-20., 12.4, -120), //Point C
     glm::vec3(-20., 12.4, -0)); //Point D
    leftplane->setColor(glm::vec3(1, 0., 0));
    sceneObjects.push_back(leftplane);
      leftplane->setSpecularity(false); //l7

    //rightplane
    Plane *rightplane = new Plane (glm::vec3(20., 12.4, -0), //Point A
     glm::vec3(20., 12.4, -120), //Point B
     glm::vec3(20., -12.4, -120), //Point C
     glm::vec3(20., -12.4, -0)); //Point D
    rightplane->setColor(glm::vec3(0., 0., 1));
    sceneObjects.push_back(rightplane);
    rightplane->setSpecularity(false); //l7
      //ceilling
      Plane *roofplane = new Plane (glm::vec3(20., 12.4, -0), //Point A
       glm::vec3(-20., 12.4, -0), //Point B
       glm::vec3(-20., 12.4, -120), //Point C
       glm::vec3(20., 12.4, -120)); //Point D
      roofplane->setColor(glm::vec3(1., 1., 1));
      sceneObjects.push_back(roofplane);
      roofplane->setSpecularity(false);

}


int main(int argc, char *argv[]) {
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB );
    glutInitWindowSize(800, 800);
    glutInitWindowPosition(1200, 20);
    glutCreateWindow("Raytracing");
    fog_state = false;
    anti_aliasing = true;
    glutDisplayFunc(display);
    initialize();
    roomobjects();
    glutMainLoop();
    return 0;
}
