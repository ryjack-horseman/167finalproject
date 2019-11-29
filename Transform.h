//
//  Transform.hpp
//  cse 167
//
//  Created by Ryan Jackson on 10/29/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#ifndef Transform_h
#define Transform_h


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
class Transform : public Node{
public:
    Transform(glm::mat4 mat);
    ~Transform();
    glm::mat4 transMat;
    std::vector<Node *> children;
    void addChild(Node * child);
    void draw(GLuint shaderProgram, glm::mat4 C);
    void update(glm::mat4 C);
    bool drawChildren;
};
#endif /* Transform_hpp */
