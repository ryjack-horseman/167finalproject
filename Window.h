#ifndef _WINDOW_H_
#define _WINDOW_H_

#ifdef __APPLE__
#define GL_SILENCE_DEPRECIATION
#define GLFW_INCLUDE_GLCOREARB
#define GLM_ENABLE_EXPERIMENTAL
#include <OpenGL/gl3.h>
#else
#include <GL/glew.h>
#endif
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/ext.hpp>
#include <iostream>
#include <vector>
#include <memory>

#include "Object.h"
#include "Cube.h"
#include "PointCloud.h"
#include "shader.h"
#include "Transform.h"
#include "Node.h"
#include "Geometry.h"
class Window
{
public:
	static int width;
	static int height;
    static int Movement;
    static glm::vec3 lastPosition;
    static glm::vec3 startingPos;
    static glm::vec3 currPos;
	static const char* windowTitle;
    //left for culling if we want to add in
    struct plane{
        glm::vec3 normal;
        glm::vec3 point;
    };
    static Transform * root;
	static glm::mat4 projection;
	static glm::mat4 view;
	static glm::vec3 eye, center, up;
    static GLuint program;
    
    static float fov;
    static float position;
	static bool initializeProgram();
	static bool initializeObjects();
	static void cleanUp();
	static GLFWwindow* createWindow(int width, int height);
	static void resizeCallback(GLFWwindow* window, int width, int height);
    static void idleCallback();
	static void displayCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(GLFWwindow * window, int button, int action, int mods);
    static void cursor_callback(GLFWwindow * window, double xpos, double ypos);
    //culling commented out for now
    //static void culling(bool testMode);
    static glm::vec3 trackBallMapping(glm::vec2 point);
    static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void renderQuad();
	static void buildShadowMap();
	static void drawQuadMap();
	static void drawScene();
};

#endif
