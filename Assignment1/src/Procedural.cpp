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
#include <iostream>
#include <sstream>
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
	m_seedCount = 0;
	m_seeds.push_back(m_terrain->m_seed);
}
void Procedural::Update(double _dt) {
	m_camera->Update(_dt);
	if (glfwGetKey(m_window, GLFW_KEY_R) == GLFW_PRESS) {
		m_terrain->ReloadShaders();
	}
	if (ImGui::CollapsingHeader("Realtime Terrain")) {
		ImGui::SliderFloat("Z-Value", &m_terrain->m_zValue, -100.0f, 100.0f);
		ImGui::SliderFloat("Amplitude", &m_terrain->m_amplitude, 0, 3);
		ImGui::SliderInt("Size", &m_terrain->m_size, 10, 100);
	}
	if (ImGui::CollapsingHeader("Reload Terrain")) {
		ImGui::SliderInt("Seed", &m_terrain->m_seed, 0, 100000);
		ImGui::SliderInt("Octaves", &m_terrain->m_octaves, 1, 10);
		ImGui::SliderFloat("Persistence", &m_terrain->m_persistence, 0, 1);
		if (ImGui::Button("Use Seed")) {
			m_terrain->NewSeed(m_terrain->m_seed);
			m_seeds.push_back(m_terrain->m_seed);
		}
		ImGui::SameLine(0, (int)ImGui::GetStyle().ItemInnerSpacing.x);
		if (ImGui::Button("Random Seed")) {
			m_terrain->NewSeed();
			m_seeds.push_back(m_terrain->m_seed);
		}
	
		ImGui::PushStyleVar(ImGuiStyleVar_ChildWindowRounding, 5.0f);
		ImGui::BeginChild("Sub2", ImVec2(0, 300), true);
		ImGui::Text("Recent Seeds");
		ImGui::Columns(3);
		for (int i = 0; i < m_seeds.size(); i++)
		{
			if (i == 5) {
				ImGui::NextColumn();
			}
			char buf[32];
			std::stringstream temp_str;
			temp_str << (m_seeds[i]);
			std::string str = temp_str.str();
			const char* cstr2 = str.c_str();
			if (ImGui::Button(cstr2)) {
				m_terrain->NewSeed(m_seeds[i]);
			}

		}
		ImGui::EndChild();
		ImGui::PopStyleVar();
	}
	//ImGui::ShowTestWindow();
}
void Procedural::Draw() {
	m_skybox->Draw(m_camera);
	m_terrain->Draw(m_camera);
	
}
