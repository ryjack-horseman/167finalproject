//
//  Transform.cpp
//  cse 167
//
//  Created by Ryan Jackson on 10/29/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//
#include <stdio.h>

#include <iostream>
#include "Transform.h"
#include "Transform.h"

Transform::Transform(glm::mat4 mat){
    transMat = mat;
    drawChildren = true;
}

void Transform::addChild(Node * child){
    Transform::children.push_back(child);
}

void Transform::draw(GLuint shaderProgram, glm::mat4 C){
    if(this->drawChildren){
    for(auto const& i : Transform::children){
        if(i == NULL){
            std::cout << "oops" << std::endl;
        }
        i->draw(shaderProgram, C * Transform::transMat);
    }
}
}
void Transform::update(glm::mat4 C){
    transMat = C * transMat;
}
