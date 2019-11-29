//
//  Geometry.cpp
//  cse 167
//
//  Created by Ryan Jackson on 10/29/19.
//  Copyright © 2019 Ryan Jackson. All rights reserved.
//

#include "Geometry.h"
#include <stdio.h>
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include "Window.h"
float Geometry::centerX;
float Geometry::centerY;
float Geometry::centerZ;
void update(glm::mat4 C){
    
}
Geometry::Geometry(std::string objFilename, bool whichone){
    type = whichone;
    sphereOn = 2;
    std::ifstream objFile(objFilename); // The obj file we are reading.
        GLfloat minx, miny, minz, maxx, maxy, maxz;
        minx = miny = maxx = minz = maxy = maxz = -12;
        // Check whether the file can be opened.
        if (objFile.is_open())
        {
            std::string line; // A line in the file.

            // Read lines from the file.
            while (std::getline(objFile, line))
            {
                // Turn the line into a string stream for processing.
                std::stringstream ss;
                ss << line;
                
                // Read the first word of the line.
                std::string label;
                ss >> label;
                
                // If the line is about vertex (starting with a "v").
                if (label == "v")
                {
                    // Read the later three float numbers and use them as the
                    // coordinates.
                    glm::vec3 point;
                    ss >> point.x >> point.y >> point.z;
                    
                    // Process the point. For example, you can save it to a.
                    prepoints.push_back(point);
                }
                else if( label == "vn"){
                    glm::vec3 normPoint;
                    ss >> normPoint.x >> normPoint.y >> normPoint.z;
                    prenormals.push_back(normPoint);
                }
                
                else if(label == "f" && type == true){
                    glm::ivec3 facesIndices;
                    glm::ivec3 normalIndices;
                    std::string hi = ss.str();

                    const char* hold=hi.c_str();
                    int a,b,c,d,e,f,g,h,i;
                    sscanf(hold, "f %i/%i/%i %i/%i/%i %i/%i/%i",&a,&b,&c,&d,&e,&f,&g,&h, &i);
                   
                    facesIndices.x = --a;
                    facesIndices.y = --d;
                    facesIndices.z = --g;
                    normalIndices.x = --c;
                    normalIndices.y = --f;
                    normalIndices.z = --i;
                    
                    faces.push_back(facesIndices.x);
                    faces.push_back(facesIndices.y);
                    faces.push_back(facesIndices.z);
                    indices.push_back(normalIndices.x);
                    indices.push_back(normalIndices.y);
                    indices.push_back(normalIndices.z);
                }
                else if(label == "f" && type == false){
                 glm::ivec3 facesIndices;
                 glm::ivec3 normalIndices;
                 std::string hi = ss.str();

                 const char* hold=hi.c_str();
                    int a,b,c,d,e,f;
                 sscanf(hold, "f %i//%i %i//%i %i//%i",&a,&b,&c,&d,&e,&f);
                
                 facesIndices.x = --a;
                 facesIndices.y = --c;
                 facesIndices.z = --e;
                 normalIndices.x = --b;
                 normalIndices.y = --d;
                 normalIndices.z = --f;
                 
                 faces.push_back(facesIndices.x);
                 faces.push_back(facesIndices.y);
                 faces.push_back(facesIndices.z);
                 indices.push_back(normalIndices.x);
                 indices.push_back(normalIndices.y);
                 indices.push_back(normalIndices.z);
            }
            }
                if(type == true){
            for(int i = 0; i < faces.size(); i++){
            
                points.push_back(prepoints[faces[i]]);
                normals.push_back(prenormals[indices[i]]);
               otherIndices.push_back(i);
            }
                }
                else{
                    sphereOn = 1;
                    points = prepoints;
                    normals = prenormals;
                }
          //  std::cout << points.size() << std::endl;
            
            //find the min and max in each direction
            for(int i = 0; i < points.size(); i++){
                if( i == 0){
                    minx = maxx = points.at(i).x;
                    miny = maxy = points.at(i).y;
                    minz = maxz = points.at(i).z;
                }
                else{
                    if(points.at(i).x < minx){
                        minx = points.at(i).x;
                        
                    }
                    if(points.at(i).x > maxx){
                        maxx = points.at(i).x;
                    }
                    if(points.at(i).y < miny){
                        miny = points.at(i).y;
                    }
                    if(points.at(i).y > maxy){
                        maxy = points.at(i).y;
                    }
                    if(points.at(i).z < minz){
                        minz = points.at(i).z;
                    }
                    if(points.at(i).z > maxz){
                        maxz = points.at(i).z;
                    }
                }
            }
            //find the center
            centerX = (maxx+minx)/2;
            centerY = (maxy+miny)/2;
            centerZ = (maxz+minz)/2;
            
            
            GLfloat currMax;
            //normalize the points
            for( int i = 0; i < points.size(); i++){
                points.at(i).x = points.at(i).x - centerX;
                points.at(i).y = points.at(i).y - centerY;
                points.at(i).z = points.at(i).z - centerZ;
                
                
            }
            //find the new center for the scaling calculation, should be 0,0,0 but calculated for clarity
            for(int i = 0; i < points.size(); i++){
                       if( i == 0){
                           minx = maxx = points.at(i).x;
                           miny = maxy = points.at(i).y;
                           minz = maxz = points.at(i).z;
                       }
                       else{
                           if(points.at(i).x < minx){
                               minx = points.at(i).x;
                               
                           }
                           if(points.at(i).x > maxx){
                               maxx = points.at(i).x;
                           }
                           if(points.at(i).y < miny){
                               miny = points.at(i).y;
                           }
                           if(points.at(i).y > maxy){
                               maxy = points.at(i).y;
                           }
                           if(points.at(i).z < minz){
                               minz = points.at(i).z;
                           }
                           if(points.at(i).z > maxz){
                               maxz = points.at(i).z;
                           }
                       }
                   }
            //setting the new center after centering, should be 0,0,0
            centerX = (maxx+minx)/2;
            centerY = (maxy+miny)/2;
            centerZ = (maxz+minz)/2;
           
            glm::vec3 center(centerX,centerY,centerZ);
            //find the max distance between a point and the center
            
            for(int i = 0; i < points.size(); i++){
                if(i == 0){
                    currMax = distance(points.at(i), center);
                }
                else if(distance(points.at(i), center) > currMax){
                    currMax = distance(points.at(i), center);
                }
            }
            
    //normalize the models to 1
            
            for(int i = 0; i< points.size(); i++){
                points.at(i) = points.at(i) *  (1/currMax);
            }
            // apply the magic number to the now normalized objects
            GLfloat magicNum = 5.00;
            for(int i = 0; i < points.size(); i++){
                points.at(i) = points.at(i) * magicNum;
            }
    
            }
        else
        {
            std::cerr << "Can't open the file " << objFilename << std::endl;
        }

        objFile.close();
    model = glm::mat4(1.0f);
    glGenVertexArrays(1, &vao);
    glGenBuffers(2, vbos);

    // Bind to the VAO.
    glBindVertexArray(vao);

    // Bind to the first VBO. We will use it to store the points.
    glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points.size(),
        points.data(), GL_STATIC_DRAW);
    // Enable vertex attribute 0.
    // We will be able to access points through it.
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);

    //second vbo with normal data
    glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
    // Pass in the data.
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals.size(),
     normals.data(), GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    
    glGenBuffers(1, &ebo);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
    if(type){
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * otherIndices.size(), otherIndices.data(), GL_STATIC_DRAW);
    }
    else{
       glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);
    }
    // Unbind from the VBO.
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}

void Geometry::update(glm::mat4 C){
   model = C * model;
}
void Geometry::draw(GLuint shaderProgram, glm::mat4 C){
    glm::mat4 toWorld = model * C;
    
    glUseProgram(shaderProgram);
    glBindVertexArray(vao);

    glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
    // Unbind from the VAO.
    glBindVertexArray(0);
}
