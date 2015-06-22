/* 
 * File:   CameraMovement.hpp
 * Author: dAmihl
 *
 * Created on 17. Juni 2015, 14:41
 */


#ifndef CAMERAMOVEMENT_H
#define	CAMERAMOVEMENT_H

    glm::mat4 CameraFreeMove(glm::mat4 ViewMatrix);
    glm::mat4 CameraFixedMove(glm::mat4 ViewMatrix);
    
    void moveForward(GLboolean flag);
    void moveBack(GLboolean flag);
    void moveLeft(GLboolean flag);
    void moveRight(GLboolean flag);
    
    void setAxis(int turnAxis);
    
    void MouseMoveUpdate(int x, int y);

    extern int mouseDeltaX;
    extern int mouseDeltaY;

    extern int CAMERA_FREE_MOVE;
    extern int CAMERA_FIXED_MOVE;
    extern int cameraMode;
    
    enum {Xaxis=0, Yaxis=1, Zaxis=2};
    
    extern int axis;
    
    extern glm::vec3 camPosition;
    extern glm::vec3 camDirection;
    extern glm::vec3 camUp;

#endif	/* CAMERAMOVEMENT_H */

