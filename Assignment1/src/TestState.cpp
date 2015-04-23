#include "TestState.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include "FlyCamera.h"
#include "ObjectOBJ.h"
#include "ShaderLoader.h"
#include "ParticleEmitter.h"
#include "Fire.h"
#include "RenderTarget.h"
#include "Skybox.h"
#include "imgui.h"
void TestState::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_timeLeft = 5.0f;
	m_gameStateManager = _gameStateManager;
	m_camera = new FlyCamera(10.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera->SetLookAt(glm::vec3(10, 2, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_fire = new Fire();
	m_fire->ReloadShaders();
}
void TestState::Update(double _dt) {
	m_camera->Update(_dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS)  {
		m_fire->ReloadShaders();
	}
	static int e;

	if (ImGui::CollapsingHeader("Particles")) {
		ImGui::SliderFloat("Spread", &m_fire->m_emitter->m_spread, 0.1f, 10);
		ImGui::SliderFloat("LifeMin", &m_fire->m_emitter->m_minLifeSpan, 0.0f, m_fire->m_emitter->m_maxLifeSpan - 0.001f);
		ImGui::SliderFloat("LifeMax", &m_fire->m_emitter->m_maxLifeSpan, m_fire->m_emitter->m_minLifeSpan + 0.001f, 10.0f);
		if (ImGui::TreeNode("Emitter Type")) {
			ImGui::RadioButton("Point", &e, 0);
			ImGui::RadioButton("Circle", &e, 1);
			ImGui::RadioButton("Sphere", &e, 2);
			ImGui::RadioButton("Square", &e, 3);
			ImGui::RadioButton("Cube", &e, 4);
			ImGui::TreePop();
		}

	}
	m_fire->m_emitter->m_emitterType = (emitterType)e;
}
void TestState::Draw() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	m_fire->Draw(m_camera);
}


