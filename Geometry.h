#ifndef _GEOMETRY_H_
#define _GEOMETRY_H_

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
#include <fstream>
#include <sstream>
#include <string>
#include "Node.h"

class Geometry : public Node
{
private:
	glm::mat4 model;
	std::vector<Node*> children;
	std::vector<glm::vec3> points;
	std::vector<glm::vec3> points_;
	std::vector<glm::vec3> normals_;
	std::vector<glm::vec3> normals;
	std::vector<glm::ivec3> facesP;
	std::vector<glm::ivec3> facesT;
	std::vector<glm::ivec3> facesN;
	std::vector<int> indices;
    std::vector<int> faces;
    std::vector<glm::vec3> prepoints;
    std::vector<glm::vec3> prenormals;
    std::vector<int> otherIndices;
    //float centerX, centerY, centerZ;
    bool type;
	GLuint vao, vbos[4];
    GLuint ebo;
	glm::vec3 color, ambient, diffuse, specular;
	float shininess;
	glm::vec3 center;
	float radius;

public:
	Geometry(glm::mat4 C, std::string objFilename, float width, float height, glm::vec3 rgb,
		std::vector<glm::vec3> matAttr, float matShin, bool deskParse);
	void Load();
    void deskLoad();
	void Scaling(float magnitude);
	void Parsing(std::string& objFilename);
    void deskParse(std::string & objFilename);
	~Geometry();
	void draw(GLuint shaderProgram, glm::mat4 C);
	void update(glm::mat4 C);
	float getRadius();
};

#endif

