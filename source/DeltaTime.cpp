#include "DeltaTime.h"
#include <GL/freeglut.h>

int deltaTime = 0;
int oldTimeSinceStart = 0;

/*
 Computes the time elapsed since last Display()
 */
void computeDeltaTime(){
    
    int newTime = glutGet(GLUT_ELAPSED_TIME);
    deltaTime = newTime - oldTimeSinceStart;
    oldTimeSinceStart = newTime; 
}

float fpsIndependentFloat(float f){
    f = f * deltaTime/1000;
    return f;
}


