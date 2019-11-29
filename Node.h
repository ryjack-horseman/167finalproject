//
//  Node.h
//  cse 167
//
//  Created by Ryan Jackson on 10/29/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#ifndef Node_h
#define Node_h
#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <stdio.h>

class Node{
public:
    virtual void draw(GLuint shaderProgram, glm::mat4 C) = 0;
    virtual void update(glm::mat4 C) = 0;
};
#endif /* Node_h */
