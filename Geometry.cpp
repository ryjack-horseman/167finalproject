#include "Geometry.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <ctime>
float centerX;
float centerY;
float centerZ;
Geometry::Geometry(glm::mat4 C, std::string objFilename, float width, float height, glm::vec3 rgb,
	std::vector<glm::vec3> matAttr, float matShin, bool whichOne) {

	model = C;
	color = rgb;
	ambient = matAttr[0];
	diffuse = matAttr[1];
	specular = matAttr[2];
	shininess = matShin;
    if(whichOne){
        type = true;
	deskParse(objFilename);
        deskLoad();
    }
    else{
        type = false;
        Parsing(objFilename);
        Load();
    }
	
}

Geometry::~Geometry() {
	// Delete the VBO and the VAO.
	glDeleteBuffers(3, vbos);
	glDeleteVertexArrays(1, &vao);
}

void Geometry::draw(GLuint shaderProgram, glm::mat4 C)
{
	GLuint modelLoc = glGetUniformLocation(shaderProgram, "model");
	GLuint colorLoc = glGetUniformLocation(shaderProgram, "color");

	GLuint matAmbiLoc = glGetUniformLocation(shaderProgram, "material.ambient");
	GLuint matDiffLoc = glGetUniformLocation(shaderProgram, "material.diffuse");
	GLuint matSpecLoc = glGetUniformLocation(shaderProgram, "material.specular");
	GLuint matShinLoc = glGetUniformLocation(shaderProgram, "material.shininess");

	glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(C * model));
	glUniform3fv(colorLoc, 1, glm::value_ptr(color));

	glUniform3fv(matAmbiLoc, 1, glm::value_ptr(ambient));
	glUniform3fv(matDiffLoc, 1, glm::value_ptr(diffuse));
	glUniform3fv(matSpecLoc, 1, glm::value_ptr(specular));
	glUniform1f(matShinLoc, shininess);

	// Bind to the VAO.
	glBindVertexArray(vao);
	// Draw triangles using the indices in the second VBO, which is an 
	// element array buffer.
    if(type){
      //  glEnable(GL_CULL_FACE);
        glDrawElements(GL_TRIANGLES, 3 * faces.size(), GL_UNSIGNED_INT, 0);
    }
    else{
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    }
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Geometry::update(glm::mat4 M) {
}

void Geometry::Load()
{
	// Generate a vertex array (VAO) and two vertex buffer objects (VBO).
	glGenVertexArrays(1, &vao);

	glGenBuffers(4, vbos);

	// Bind to the VAO.
	glBindVertexArray(vao);

	// Bind to the first VBO. We will use it to store the vertices.
	glBindBuffer(GL_ARRAY_BUFFER, vbos[0]);
	// Pass in the data.
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * points_.size(),
		points_.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 0.
	glEnableVertexAttribArray(0);
	// We will be able to access vertices through it.
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	glBindBuffer(GL_ARRAY_BUFFER, vbos[1]);
	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * normals_.size(), normals_.data(), GL_STATIC_DRAW);
	// Enable vertex attribute 1;
	glEnableVertexAttribArray(1);
	// Color
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void*)0);

	// Bind to the third VBO. We will use it to store the point indices.
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vbos[2]);
	// Pass in the data.
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * indices.size(),
		indices.data(), GL_STATIC_DRAW);

	// Unbind from the VBOs.
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	// Unbind from the VAO.
	glBindVertexArray(0);
}

void Geometry::deskLoad(){
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
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int) * otherIndices.size(), otherIndices.data(), GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
       // Unbind from the VAO.
       glBindVertexArray(0);
}
void Geometry::Scaling(float magnitude)
{
	glm::vec3 objMin = points[0];
	glm::vec3 objMax = points[0];
	for (int i = 0; i < points.size(); i++) {
		if (points[i].x < objMin.x) {
			objMin.x = points[i].x;
		}
		else if (points[i].x > objMax.x) {
			objMax.x = points[i].x;
		}
		if (points[i].y < objMin.y) {
			objMin.y = points[i].y;
		}
		else if (points[i].y > objMax.y) {
			objMax.y = points[i].y;
		}
		if (points[i].z < objMin.z) {
			objMin.z = points[i].z;
		}
		else if (points[i].z > objMax.z) {
			objMax.z = points[i].z;
		}
	}


	// Object dimensions
	glm::vec3 objDim = abs(objMax - objMin);
	glm::vec3 objHalf = (objDim / (float)2);
	radius = glm::length(objHalf);

	// Transverse adjustment
	glm::vec3 objMid = objMax - objHalf;
	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i] - objMid;
	}

	// Farthest possible point distance
	float maxDist = std::abs(distance(objHalf, glm::vec3(0, 0, 0)));

	float mag = magnitude;

	for (int i = 0; i < points.size(); i++) {
		points[i] = points[i] * mag;
	}
	for (int i = 0; i < normals.size(); i++) {
		normals[i] = normals[i] * mag;
	}
}

void Geometry::Parsing(std::string& objFilename)
{
	std::ifstream objFile(objFilename);
	// Parsing file
	if (objFile.is_open())
	{
		std::string line;
		while (std::getline(objFile, line))
		{
			std::stringstream ss;
			ss << line;
			std::string label;
			ss >> label;
			// Determine vertex
			if (label == "v")
			{
				// Load coordinates
				glm::vec3 point;
				ss >> point.x >> point.y >> point.z;
				points.push_back(point);
			}
			else if (label == "vn") {
				glm::vec3 normal;
				ss >> normal.x >> normal.y >> normal.z;
				normals.push_back(normal);
			}
			else if (label == "f") {
				glm::ivec3 faceP;
				glm::ivec3 faceT;
				glm::ivec3 faceN;

				std::string indices = ss.str();

				indices = indices.substr(indices.find(' ') + 1);
				faceP.x = stoi(indices.substr(0, indices.find('/'))) - 1;
				indices = indices.substr(indices.find('/') + 1);
				//if (!sphereFlag) faceT.x = stoi(indices.substr(0, indices.find('/'))) - 1;

				indices = indices.substr(indices.find('/') + 1);
				faceN.x = stoi(indices.substr(0, indices.find(' '))) - 1;

				indices = indices.substr(indices.find(' ') + 1);
				faceP.y = stoi(indices.substr(0, indices.find('/'))) - 1;
				indices = indices.substr(indices.find('/') + 1);
				//if (!sphereFlag) faceT.y = stoi(indices.substr(0, indices.find('/'))) - 1;
				indices = indices.substr(indices.find('/') + 1);
				faceN.y = stoi(indices.substr(0, indices.find(' '))) - 1;

				indices = indices.substr(indices.find(' ') + 1);
				faceP.z = stoi(indices.substr(0, indices.find('/'))) - 1;
				indices = indices.substr(indices.find('/') + 1);
				//if (!sphereFlag) faceT.z = stoi(indices.substr(0, indices.find('/'))) - 1;
				indices = indices.substr(indices.find('/') + 1);
				faceN.z = stoi(indices.substr(0, indices.find(' '))) - 1;

				facesP.push_back(faceP);
				//facesT.push_back(faceT);
				facesN.push_back(faceN);

				/*
				if (sphereFlag) {
					std::cout << faceP.x << faceP.y << faceP.z << std::endl;
					std::cout << faceN.x << faceN.y << faceN.z << std::endl;
				}
				*/
			}
		}
	}
	objFile.close();

	Scaling(1);

	for (unsigned i = 0; i < facesP.size(); i++) {
		points_.push_back(points[facesP[i].x]);
		points_.push_back(points[facesP[i].y]);
		points_.push_back(points[facesP[i].z]);
		//texcoords here later
		normals_.push_back(normals[facesN[i].x]);
		normals_.push_back(normals[facesN[i].y]);
		normals_.push_back(normals[facesN[i].z]);
		indices.push_back(i * 3 - 3);
		indices.push_back(i * 3 - 2);
		indices.push_back(i * 3 - 1);
	}
}

float Geometry::getRadius() {
	return radius;
}

void Geometry::deskParse(std::string &objFilename){
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
           
           else if(label == "f" ){
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
           }}
   }
    objFile.close();
           
                      for(int i = 0; i < faces.size(); i++){
                      
                          points.push_back(prepoints[faces[i]]);
                          normals.push_back(prenormals[indices[i]]);
                         otherIndices.push_back(i);
                
}
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
