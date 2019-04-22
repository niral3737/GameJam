#ifndef _GLFW_UTILS_HG_
#define _GLFW_UTILS_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

class cGLFWUtils
{
public:
	static GLFWwindow* getWindowInstance(void);
	static void setUpGLFW(void);

private:
	cGLFWUtils();
	static GLFWwindow* window;
	static void error_callback(int error, const char* description);
};

#endif // !_GLFW_UTILS_HG_

