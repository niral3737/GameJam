#ifndef _USER_IO_HG_
#define _USER_IO_HG_

#include <glad/glad.h>
#include <GLFW/glfw3.h>

enum eSelectionMode
{
	LIGHT_SELECTION,
	MESH_SELECTION
};

class cUserIO
{
public:
	static void key_callback(GLFWwindow* window,
		int key,
		int scancode,
		int action,
		int mods);
	static void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);
	static void cursor_enter_callback(GLFWwindow* window, int entered);
	static bool isMouseInsideWindow;
	static void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
	static void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
	static void processAsynKeys(GLFWwindow* window);
	static void processAsynMouse(GLFWwindow* window);
	static bool isInMeshSelectionMode;
	static eSelectionMode selectionMode;
	static bool includeInvisibleObjects;
private:
	static bool mIsShiftDown(GLFWwindow* window);
	static bool mIsCtrlDown(GLFWwindow* window);
	static bool mIsAltDown(GLFWwindow* window);
	static bool mAreAllModifiersUp(GLFWwindow* window);
	static void mSaveSettings();
};

#endif // !_USER_IO_HG_



