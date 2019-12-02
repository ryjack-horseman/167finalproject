#include "PointCloud.h"
#include <iostream>
#include <fstream>
#include <string>
#include <sstream>
#include <stdio.h>
#include "Window.h"

/**float PointCloud::centerX;
float PointCloud::centerY;
float PointCloud::centerZ;
PointCloud::PointCloud(std::string objFilename, GLfloat pointSize, int flag) 
	: pointSize(pointSize)
{
	/* 
	 * TODO: Section 2: Currently, all the points are hard coded below. 
	 * Modify this to read points from an obj file.
	 
    std::ifstream objFile(objFilename); // The obj file we are reading.
    //std::vector<glm::vec3> points;
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
                points.push_back(point);
            }
            else if( label == "vn"){
                glm::vec3 normPoint;
                ss >> normPoint.x >> normPoint.y >> normPoint.z;
                normals.push_back(normPoint);
            }
            
            else if(label == "f"){
                glm::ivec3 facesIndices;
                std::string hi = ss.str();

                const char* hold=hi.c_str();
                int a,b,c,d,e,f;
                sscanf(hold, "f %i//%i %i//%i %i//%i",&a,&b,&c,&d,&e,&f);
               
                facesIndices.x = --a;
                facesIndices.y = --c;
                facesIndices.z = --e;
                faces.push_back(facesIndices);
            }
        }
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
        if(flag == 0){
        // apply the magic number to the now normalized objects
        GLfloat magicNum = 10.00;
        for(int i = 0; i < points.size(); i++){
            points.at(i) = points.at(i) * magicNum;
        }
        }
        else{
            glm::mat4 trans = glm::translate(Window::lightPos);
            Object * hi = (Object *) this;
            hi->model = trans;
        }
        }
    else
    {
        std::cerr << "Can't open the file " << objFilename << std::endl;
    }

    objFile.close();
	//points =
	//{
		//glm::vec3(-2.5, 2.5, 2.5),
		//glm::vec3(-2.5, -2.5, 2.5),
		//glm::vec3(2.5, -2.5, 2.5),
		//glm::vec3(2.5, 2.5, 2.5),
		//glm::vec3(-2.5, 2.5, -2.5),
		//glm::vec3(-2.5, -2.5, -2.5),
		//glm::vec3(2.5, -2.5, -2.5),
		//glm::vec3(2.5, 2.5, -2.5)
	//};

	/*
	 * TODO: Section 4, you will need to normalize the object to fit in the
	 * screen. 
	 

	// Set the model matrix to an identity matrix.
    if(flag ==0){
        model = glm::mat4(1);
    }
    //else{
       // model = glm::mat4(1);
     //   std::cout << Window::lightPos.x << std::endl;
    ///    model = glm::translate(model, Window::lightPos);
   // }
	// Set the color. 
	color = glm::vec3(1, 0, 0);

	// Generate a vertex array (VAO) and a vertex buffer objects (VBO).
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(glm::ivec3) * faces.size(), faces.data(), GL_STATIC_DRAW);
	// Unbind from the VBO.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

PointCloud::~PointCloud() 
{
	// Delete the VBO and the VAO.
	glDeleteBuffers(2, vbos);
	glDeleteVertexArrays(1, &vao);
}

void PointCloud::draw()
{
   
//  glm::mat4 model = Window::currentObj->getModel();
//  glm::vec3 color = Window::currentObj->getColor();
//  PointCloud *hold = (PointCloud *) Window::currentObj;
 // glm::vec3 matAmbient = hold->matAmbient;
  //hold->matAmbient;
 // glm::vec3 matDiffuse = hold->matDiffuse;
 /// glm::vec3 matSpecular = hold->matSpecular;
 // float matShine = hold->matShine;
 // lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    //float matShine = hold->matShine;
 //Window::lightColor = glm::vec3(1.0f, 1.0f, 1.0f);
    
    //glm::mat4 sphereMod = light->getModel();
   // glUniform1i(glGetUniformLocation(Window::program, "mode"), Window::currMode);
    //glUniformMatrix4fv(glGetUniformLocation(Window::program, "projection"), 1, GL_FALSE, glm::value_ptr(Window::projection));
    //glUniformMatrix4fv(glGetUniformLocation(Window::program, "view"), 1, GL_FALSE, glm::value_ptr(Window::view));
    
   // glUniformMatrix4fv(glGetUniformLocation(Window::program, "model"), 1, GL_FALSE, glm::value_ptr(model));
  //  glUniform3fv(glGetUniformLocation(Window::program, "color"), 1, glm::value_ptr(color));
  //  glUniform3fv(glGetUniformLocation(Window::program, "lightColor"), 1, glm::value_ptr(Window::lightColor));
  //  glUniform3fv(glGetUniformLocation(Window::program, "lightPos"), 1, glm::value_ptr(Window::lightPos));
 //   glUniform3fv(glGetUniformLocation(Window::program, "matAmbient"), 1, glm::value_ptr(matAmbient));
 //   glUniform3fv(glGetUniformLocation(Window::program, "matSpecular"), 1, glm::value_ptr(matSpecular));
 //  glUniform3fv(glGetUniformLocation(Window::program, "matDiffuse"), 1, glm::value_ptr(matDiffuse));
 //   glUniform3fv(glGetUniformLocation(Window::program, "eye"), 1, glm::value_ptr(Window::eye));
  //  glUniform1f(glGetUniformLocation(Window::program, "matShine"), matShine);
	// Bind to the VAO.
	//glBindVertexArray(vao);
	// Set point size.
	//glPointSize(pointSize);
	// Draw points
 //   glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
	// Unbind from the VAO.
//	glBindVertexArray(0);
}

void PointCloud::update()
{
	// Spin the cube by 1 degree.
	//spin(0.1f);
}

void PointCloud::updatePointSize(GLfloat size, PointCloud * curr1)
{
    if(size > 0 && pointSize < 10){
        pointSize = pointSize + size;
    }
    else if( size < 0 && pointSize > 0){
        pointSize = pointSize + size;
    }
    
}

void PointCloud::spin(float deg)
{
	// Update the model matrix by multiplying a rotation matrix
	model = glm::rotate(model, glm::radians(deg), glm::vec3(0.0f, 1.0f, 0.0f));
} **/
