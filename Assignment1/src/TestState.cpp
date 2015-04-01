#include "TestState.h"
#include <gl_core_4_4.h>
#include <GLFW\glfw3.h>
#include "FlyCamera.h"
#include "ObjectOBJ.h"
#include "ShaderLoader.h"
#include "ParticleEmitter.h"
#include "SoundManager.h"
#include "DebugGUI.h"
#include "Fire.h"
#include "RenderTarget.h"
#include "Skybox.h"
void TestState::Init(GLFWwindow* _window, GameStateManager* _gameStateManager) {
	m_window = _window;
	m_timeLeft = 5.0f;
	m_gameStateManager = _gameStateManager;
	m_emitter = new ParticleEmitter();
	m_emitter->Init(50000, 1.0f, 2.0f, 1.0f, 5.0f, 0.1f, 0.15f, glm::vec4(1.0f, 1.0f, 1.0f, 0.75f), glm::vec4(1.0f, 1.0f, 1.0f, 0.25f));
	const char* path[2];
	path[0] = "res/textures/fire.png";
	path[1] = "res/textures/smoke.png";
	m_emitter->LoadTexture(path);
	m_soundManager = new SoundManager();
	m_soundManager->AddSound("res/sounds/explosion.wav", "explosion");
	m_camera = new FlyCamera(10.0f);
	m_camera->SetInputWindow(m_window);
	m_camera->SetPerspective(glm::pi<float>() * 0.25f, 16.0f / 9.0f, 0.1f, 1000.0f);
	m_camera->SetLookAt(glm::vec3(100, 20, 100), glm::vec3(0), glm::vec3(0, 1, 0));
	m_debugGUI = new DebugGUI(m_window);
	m_fire = new Fire();
	m_renderTarget = new RenderTarget(m_camera);
	m_skybox = new Skybox();
	m_object = new ObjectOBJ("res/models/Dragon/Dragon.obj", ShaderLoader::LoadProgram("res/shaders/simpleOBJ.vs", "res/shaders/simpleOBJ.fs"));
}
void TestState::Update(double _dt) {
	m_soundManager->Update(m_camera);
	if (glfwGetKey(m_window, GLFW_KEY_B) == GLFW_PRESS) {
		m_soundManager->PlayExistingSound("explosion");
	}
	m_camera->Update(_dt);
	m_object->Update(_dt);
}
void TestState::Draw() {
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	//m_skybox->Draw(m_camera);

	//m_renderTarget->DrawBegin();
	
	//m_emitter->Draw((float)glfwGetTime(), m_camera->GetTransform(), m_camera->GetProjectionView());
	//m_object->Draw(m_camera);
	//m_debugGUI->Draw();
	//m_fire->Draw(m_camera);

	//m_renderTarget->DrawEnd();
}


