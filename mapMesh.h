//
//  mapMesh.hpp
//  cse 167
//
//  Created by Ryan Jackson on 12/3/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#ifndef mapMesh_h
#define mapMesh_h

#ifdef __APPLE__
#define GLFW_INCLUDE_GLCOREARB
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include <vector>
#include <memory>
#include <string>
class mapMesh{
    
    std::vector<glm::vec3> triNormals;

public:
    GLuint vbomesh, normalBuffer, vaoID;

    std::vector<GLfloat> mesh, normals;

    int sideNum; //Number of squares on side of mesh
    GLfloat meshSize;
    int mapSize;


    mapMesh(int mapSize, int meshSize);

    void genTriangleMesh(std::vector<GLfloat> heightData);
    void genTriangleNormals();

    void genVertexMesh(std::vector<GLfloat> heightData);
    void genVertexNormals();

    void initVertArray(); //Requires GLFW and GLEW to be initialized
    void disableVertArrays();
    void draw();

    GLfloat getNormals();

};
#endif /* mapMesh_hpp */
