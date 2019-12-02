#include "Window.h"
#include <glm/gtx/transform.hpp>
int Window::width;
int Window::height;
int Window::Movement;
float Window::fov = 50.0f;
float Window::position = 0.0f;
Transform * Window::root = new Transform(glm::mat4(1));
glm::vec3 Window::lastPosition;
glm::vec3 Window::startingPos;

const char* Window::windowTitle = "GLFW Starter Project";
// Objects to display.
Geometry*sphere;

glm::mat4 Window::projection; // Projection matrix.

glm::vec3 Window::eye(0, 0, 5); // Camera position.
glm::vec3 Window::center(0, 0, 0); // The point we are looking at.
glm::vec3 Window::up(0, 1, 0); // The up direction of the camera.

// View matrix, defined by eye, center and up.
glm::mat4 Window::view = glm::lookAt(Window::eye, Window::center, Window::up);

GLuint projectionLoc; // Location of projection in shader.
GLuint viewLoc; // Location of view in shader.
GLuint modelLoc; // Location of model in shader.
GLuint viewPosLoc;

GLuint lgtPosLoc, lgtConsLoc, lgtLineLoc, lgtQuadLoc, lgtAmbiLoc, lgtDiffLoc, lgtSpecLoc;

GLuint celFlagLoc;
bool cFlag;

GLuint Window::program; // The shader program id.

std::vector<glm::vec3> highSpecular = {

	// Modified ruby values
	glm::vec3(0.1745f, 0.01175f, 0.01175f),
	glm::vec3(0.0f, 0.0f, 0.0f),
	glm::vec3(0.727811f, 0.626959f, 0.626959f),

};

std::vector<glm::vec3> highDiffuse = {

	// Modified jade values
	glm::vec3(0.135f, 0.2225f, 0.1575f),
	glm::vec3(0.54f, 0.89f, 0.63f),
	glm::vec3(0.0f, 0.0f, 0.0f),

};

std::vector<glm::vec3> highDiffSpec = {

	// Brass values
	glm::vec3(0.329412f, 0.223529f, 0.027451f),
	glm::vec3(0.780392f, 0.568627f, 0.113725f),
	glm::vec3(0.992157f, 0.941176f, 0.807843f),

};

std::vector<glm::vec3> lightAmbi = {

	glm::vec3(0.135f, 0.2225f, 0.1575f),
	glm::vec3(0.54f, 0.89f, 0.63f),
	glm::vec3(0.0f, 0.0f, 0.0f),

};

std::vector<glm::vec3> emerald = {

	// emerald values 0.6 shininess
	glm::vec3(0.0215f, 0.1745f, 0.0215f),
	glm::vec3(0.07568f, 0.61424f, 0.07568f),
	glm::vec3(0.633f, 0.727811f, 0.633f),

};

std::vector<glm::vec3> chrome = {

	// chrome values 0.6 shininess
	glm::vec3(0.25f, 0.25f, 0.25f),
	glm::vec3(0.4f, 0.4f, 0.4f),
	glm::vec3(0.774597f, 0.774597f, 0.774597f),

};

bool Window::initializeProgram() {
	// Create a shader program with a vertex shader and a fragment shader.
    //call the program what shader it is supposed to be for
   program = LoadShaders("shaders/shader.vert", "shaders/shader.frag");
	// Check the shader program.
	if (!program)
	{
		std::cerr << "Failed to initialize shader program" << std::endl;
		return false;
	}

	// Activate the shader program.
	glUseProgram(program);
	// Get the locations of uniform variables.
	projectionLoc = glGetUniformLocation(program, "projection");
	viewLoc = glGetUniformLocation(program, "view");
	viewPosLoc = glGetUniformLocation(program, "viewPos");

	lgtPosLoc = glGetUniformLocation(program, "light.position");
	lgtConsLoc = glGetUniformLocation(program, "light.constant");
	lgtLineLoc = glGetUniformLocation(program, "light.linear");
	lgtQuadLoc = glGetUniformLocation(program, "light.Quadratic");
	lgtAmbiLoc = glGetUniformLocation(program, "light.ambient");
	lgtDiffLoc = glGetUniformLocation(program, "light.diffuse");
	lgtSpecLoc = glGetUniformLocation(program, "light.specular");

	celFlagLoc = glGetUniformLocation(program, "celFlag");
	cFlag = true;
   // do locs here

	return true;
}

bool Window::initializeObjects()
{
        // build out environment and the scene graph
	sphere = new Geometry(glm::mat4(1), "sphere.obj", (float)width, (float)height,
		glm::vec3(237.0f/256.0f, 116.0f / 256.0f, 116.0f / 256.0f), chrome, 0.6f);
	root->addChild(sphere);

	return true;
}

void Window::cleanUp()
{
	// Deallcoate the objects.
	
	glDeleteProgram(program);
}

GLFWwindow* Window::createWindow(int width, int height)
{
	// Initialize GLFW.
	if (!glfwInit())
	{
		std::cerr << "Failed to initialize GLFW" << std::endl;
		return NULL;
	}

	// 4x antialiasing.
	glfwWindowHint(GLFW_SAMPLES, 4);

#ifdef __APPLE__ 
	// Apple implements its own version of OpenGL and requires special treatments
	// to make it uses modern OpenGL.

	// Ensure that minimum OpenGL version is 3.3
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	// Enable forward compatibility and allow a modern OpenGL context
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

	// Create the GLFW window.
	GLFWwindow* window = glfwCreateWindow(width, height, windowTitle, NULL, NULL);

	// Check if the window could not be created.
	if (!window)
	{
		std::cerr << "Failed to open GLFW window." << std::endl;
		glfwTerminate();
		return NULL;
	}

	// Make the context of the window.
	glfwMakeContextCurrent(window);

#ifndef __APPLE__
	// On Windows and Linux, we need GLEW to provide modern OpenGL functionality.

	// Initialize GLEW.
	if (glewInit())
	{
		std::cerr << "Failed to initialize GLEW" << std::endl;
		return NULL;
	}
#endif

	// Set swap interval to 1.
	glfwSwapInterval(0);

	// Call the resize callback to make sure things get drawn immediately.
	Window::resizeCallback(window, width, height);

	return window;
}

void Window::resizeCallback(GLFWwindow* window, int width, int height)
{
#ifdef __APPLE__
	// In case your Mac has a retina display.
	glfwGetFramebufferSize(window, &width, &height); 
#endif
	Window::width = width;
	Window::height = height;
	// Set the viewport size.
	glViewport(0, 0, width, height);

	// Set the projection matrix.
	Window::projection = glm::perspective(glm::radians(60.0), 
		double(width) / (double)height, 1.0, 1000.0);
}

void Window::idleCallback()
{
  // animation happens here
  
}

void Window::displayCallback(GLFWwindow* window)
{	
	// Clear the color and depth buffers.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    
	glUniformMatrix4fv(projectionLoc, 1, GL_FALSE, glm::value_ptr(projection));
	glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
	glUniform3fv(viewPosLoc, 1, glm::value_ptr(eye));

	glUniform3f(lgtPosLoc, 3.0f, 3.0f, 6.0f);
	glUniform1f(lgtConsLoc, 1.0f);
	glUniform1f(lgtLineLoc, 0.09f);
	glUniform1f(lgtQuadLoc, 0.032f);
	glUniform3f(lgtAmbiLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lgtDiffLoc, 1.0f, 1.0f, 1.0f);
	glUniform3f(lgtSpecLoc, 1.0f, 1.0f, 1.0f);

	glUniform1i(celFlagLoc, cFlag);

	// Render the object with the appropriate shader program, might need something special inside subclasses so
    // the transforms know which shader they should use
	root->draw(program, glm::mat4(1.0f));
    
	// Gets events, including input such as keyboard and mouse or window resizing.
	glfwPollEvents();
	// Swap buffers.
	glfwSwapBuffers(window);
}

void Window::cursor_callback(GLFWwindow* window, double xpos, double ypos)
{
    startingPos.x = xpos;
    startingPos.y = ypos;
    
    if(Movement == 1){
    glm::vec3 direction;
    float rotAngle;
    glm::vec3 currPoint;
    currPoint = trackBallMapping(startingPos);
        
        
    direction = currPoint-lastPosition;
            float velocity = glm::length(direction);
            if(velocity > 0.001){
                glm::vec3 rotAxis = glm::cross(lastPosition, currPoint);
                rotAxis = glm::normalize(rotAxis);
               
          
                float check = glm::dot(lastPosition, currPoint);
            
                rotAngle = glm::acos(check);
                    glm::mat4 trans = glm::translate(-eye);
                    glm::mat4 rot = glm::rotate(rotAngle, rotAxis);
                    glm::vec4 holdCent(Window::center, 1.0f);
                    holdCent =  glm::inverse(trans) * rot * trans * holdCent;
                    center = holdCent;
                    lastPosition = currPoint;
 
            }
    }
}

void Window::mouse_callback(GLFWwindow * window, int button, int actions, int mods){
    if(button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_PRESS){
     Movement = 1;
     lastPosition = trackBallMapping(startingPos);
    }
    if(button == GLFW_MOUSE_BUTTON_LEFT && actions == GLFW_RELEASE){
        
        Movement = 0;
    }
}

void Window::scroll_callback(GLFWwindow * window, double xoffset, double yoffset){
   if (fov >= 5.0f && fov <= 50.0f)
       fov -= yoffset;
   if (fov <= 5.0f)
       fov = 5.0f;
   if (fov >= 50.0f)
       fov = 50.0f;
}

glm::vec3 Window::trackBallMapping(glm::vec2 point){
    
    glm::vec3 v;
    float d;
    
    v.x = ((2.0f * point.x) - width)/height;
    v.y = (height - 2.0f * point.y)/height;
    v.z = 0.0f;
    
    d = glm::length(v);
    d = (d < 1.0f) ? d : 1.0f;
    v.z = sqrtf(1.001f-(d*d));
    v = glm::normalize(v);
    return v;
}


void Window::keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    
	if (action == GLFW_PRESS)
	{
		switch (key)
		{
            case GLFW_KEY_ESCAPE:{
			// Close the window. This causes the program to also terminate.
			glfwSetWindowShouldClose(window, GL_TRUE);				
			break;
            }
            case GLFW_KEY_1:{
			// Set currentObj to cube
                
			break;
            }
            case GLFW_KEY_2: {
			// Set currentObj to cubePoints
			//currentObj = dragonPoints;
                
			break;
            }
            case GLFW_KEY_3:{
              
                break;
            }
            case GLFW_KEY_4:{
              
                break;
                
            }
            case GLFW_KEY_5:{
             
                break;
                
            }
            case GLFW_KEY_6:{
              
                break;
            }
            case GLFW_KEY_C:{
				cFlag = !cFlag;
                break;
            }
            case GLFW_KEY_N:{
               
            }
            default:{
			break;
            }
		}
	}
}
