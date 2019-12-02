#include "Geometry.h"
#include <iostream>
#include <istream>
#include <fstream>
#include <sstream>
#include <ctime>

Geometry::Geometry(glm::mat4 C, std::string objFilename, float width, float height, glm::vec3 rgb,
	std::vector<glm::vec3> matAttr, float matShin) {

	model = C;
	color = rgb;
	ambient = matAttr[0];
	diffuse = matAttr[1];
	specular = matAttr[2];
	shininess = matShin;
	Parsing(objFilename);
	Load();
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
	glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
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

