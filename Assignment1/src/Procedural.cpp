#include "Procedural.h"
#include "FlyCamera.h"
#include "ShaderLoader.h"
#include <gl_core_4_4.h>
#include "GLFW\glfw3.h"
#include "PerlinParticleEmitter.h"
#include <stb-master\stb_image.h>
#include "Skybox.h"
#include "aieutilities\Gizmos.h"
#include "Terrain.h"
void Procedural::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_gameStateManager = _gameStateManager;
	m_camera = new FlyCamera(100.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera->SetLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_skybox = new Skybox();
	m_terrain = new Terrain();
}
void Procedural::Update(double _dt) {
	m_camera->Update(_dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		m_terrain->ReloadShaders();
	}
}
void Procedural::Draw() {
	m_terrain->Draw(m_camera);
}
