//
//  Geometry.hpp
//  cse 167
//
//  Created by Ryan Jackson on 10/29/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#ifndef Geometry_h
#define Geometry_h

#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <list>
#include "Node.h"
#include <stdio.h>

class Geometry : public Node{
    public:
    Geometry(std::string objFilename, bool whichone);
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    bool type;
    int sphereOn;
    std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<int> faces;
    std::vector<int> indices;
    std::vector<glm::vec3> prepoints;
    std::vector<glm::vec3> prenormals;
    std::vector<int> otherIndices;
    GLuint vao, ebo;
    GLuint vbos[2];
    glm::mat4 model;
    static float centerX;
    static float centerY;
    static float centerZ;
    
};

#endif /* Geometry_hpp */
