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
#include "imgui.h"
void Procedural::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_gameStateManager = _gameStateManager;
	m_camera = new FlyCamera(100.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 10000.0f);
	m_camera->SetLookAt(glm::vec3(10, 10, 10), glm::vec3(0), glm::vec3(0, 1, 0));
	m_skybox = new Skybox();
	m_terrain = new Terrain();
	m_terrain->NewSeed();
}
void Procedural::Update(double _dt) {
	m_camera->Update(_dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		m_terrain->ReloadShaders();
	}
	if (ImGui::CollapsingHeader("Perlin Noise")) {
		ImGui::SliderFloat("Z-Value", &m_terrain->m_zValue, -100.0f, 100.0f);
		ImGui::Separator();
		ImGui::SliderInt("Seed", &m_terrain->m_seed, -100000, 100000);
		
		ImGui::Separator();
		
		ImGui::SliderInt("Octaves", &m_terrain->m_octaves, 1, 20);
		ImGui::SliderFloat("Amplitude", &m_terrain->m_amplitude, 0, 2);
		ImGui::SliderFloat("Persistence", &m_terrain->m_persistence, 0, 1);
		
		ImGui::Separator();
		
		if (ImGui::Button("Use Seed")) {
			m_terrain->NewSeed(m_terrain->m_seed);
		}
		ImGui::SameLine(0, (int)ImGui::GetStyle().ItemInnerSpacing.x);
		if (ImGui::Button("Random Seed")) {
			m_terrain->NewSeed();
		}
	}
	//ImGui::ShowTestWindow();
}
void Procedural::Draw() {
	m_skybox->Draw(m_camera);
	m_terrain->Draw(m_camera);
	
}
