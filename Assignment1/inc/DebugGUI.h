#ifndef DEBUG_GUI_H
#define DEBUG_GUI_H
#include <AntTweakBar.h>
#include <GLFW/glfw3.h>

struct GLFWwindow;
class DebugGUI
{
public:
	DebugGUI(GLFWwindow* _window);
	~DebugGUI();
	
	void Draw();

	void AddValue(const char* _name, TwType _type, void* _var, const char* _def);
protected:
private:

	GLFWwindow* m_window;
	TwBar* m_bar;

	static void OnMouseButton(GLFWwindow*, int b, int a, int m)		{ TwEventMouseButtonGLFW(b, a); }
	static void OnMousePosition(GLFWwindow*, double x, double y)	{ TwEventMousePosGLFW((int)x, (int)y); }
	static void OnMouseScroll(GLFWwindow*, double x, double y)		{ TwEventMouseWheelGLFW((int)y); }
	static void OnKey(GLFWwindow*, int k, int s, int a, int m)		{ TwEventKeyGLFW(k, a); }
	static void OnChar(GLFWwindow*, unsigned int c)					{ TwEventCharGLFW(c, GLFW_PRESS); }
	static void OnWindowResize(GLFWwindow*, int w, int h)			{ TwWindowSize(w, h);	glViewport(0, 0, w, h); }
};

#endif