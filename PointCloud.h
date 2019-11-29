#ifndef _POINT_CLOUD_H_
#define _POINT_CLOUD_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>
#include <iostream>
#include <string>

#include "Object.h"

class PointCloud : public Object
{
private:
	std::vector<glm::vec3> points;
    std::vector<glm::vec3> normals;
    std::vector<glm::ivec3> faces;
	GLuint vao, ebo;
    GLuint vbos[2];
	GLfloat pointSize;
   
public:
	PointCloud(std::string objFilename, GLfloat pointSize, int flag);
	~PointCloud();

	void draw();
	void update();

	void updatePointSize(GLfloat size, PointCloud * curr1);
	void spin(float deg);
    static float centerX;
       static float centerY;
       static float centerZ;
    glm::vec3 matDiffuse;
    glm::vec3 matAmbient;
    glm::vec3 matSpecular;
    float matShine;
};

#endif

