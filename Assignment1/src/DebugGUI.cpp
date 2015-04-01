#include "DebugGUI.h"
#include <glm.hpp>

DebugGUI::DebugGUI(GLFWwindow* _window) {
	m_window = _window;
	TwInit(TW_OPENGL_CORE, nullptr);
	TwWindowSize(1240, 768);
	glfwSetMouseButtonCallback(_window, OnMouseButton);
	glfwSetCursorPosCallback(_window, OnMousePosition);
	glfwSetScrollCallback(_window, OnMouseScroll);
	glfwSetKeyCallback(_window, OnKey);
	glfwSetCharCallback(_window, OnChar);
	glfwSetWindowSizeCallback(_window, OnWindowResize);
	m_bar = TwNewBar("Debug");
}

DebugGUI::~DebugGUI() {
	TwDeleteAllBars();
	TwTerminate();
}

void DebugGUI::Draw() {
	TwDraw();
}

void DebugGUI::AddValue(const char* _name, TwType _type, void* _var, const char* _def) {
	TwAddVarRW(m_bar, _name, _type, _var, _def);
}