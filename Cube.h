#ifndef _CUBE_H_
#define _CUBE_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <vector>

#include "Object.h"

class Cube : public Object
{
private:
	GLuint vao;
	GLuint vbos[2];
public:
	Cube();
	~Cube();
unsigned int cubemapTexture;
	void draw();
	void update();
  unsigned int loadCubemap(std::vector<std::string> faces);
	void spin(float deg);
};

#endif

