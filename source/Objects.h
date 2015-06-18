/* 
 * File:   Objects.h
 * Author: dAmihl
 *
 * Created on 17. Juni 2015, 14:34
 */

#ifndef OBJECTS_H
#define	OBJECTS_H

#ifdef	__cplusplus
extern "C" {
#endif

/*
 Structure for a world object
 */
typedef struct {
    
    std::vector<glm::vec3> vertex_buffer;
    std::vector<GLuint> index_buffer;
    std::vector<glm::vec3> normal_buffer;
    std::vector<glm::vec2> uv_buffer;
    std::vector<glm::vec3> tangent_buffer;
    
    GLuint VBO;
    GLuint CBO;
    GLuint NBO;
    GLuint IBO;
    GLuint UVBO;
    GLuint TBO;
    
    TextureDataPtr TextureData;
    GLuint TextureID;
    
    glm::mat4 ModelMatrix;
    
} WorldObject;


/*
 Structure for a billboard object
 */
typedef struct {
    
    std::vector<GLfloat> vertex_buffer;
    std::vector<GLfloat> uv_buffer;
    std::vector<GLfloat> tangent_buffer;
    
    GLuint VBO;
    GLuint UVBO;
    GLuint TBO;
    
    TextureDataPtr TextureData;
    GLuint TextureID;
    
} BillboardObject;



#ifdef	__cplusplus
}
#endif

#endif	/* OBJECTS_H */

