#include "SoundManager.h"
#include "Camera.h"
using namespace irrklang;
#pragma comment(lib, "irrKlang.lib")

SoundManager::SoundManager() {
	m_engine = createIrrKlangDevice();
}
SoundManager::~SoundManager() {
	if (m_backgroundMusic) {
		m_backgroundMusic->drop();
	}
	m_engine->drop();
}
void SoundManager::Update(Camera* _camera) {
	vec3df up = vec3df(_camera->GetTransform()[1].x, _camera->GetTransform()[1].y, _camera->GetTransform()[1].z);
	vec3df facing = vec3df(_camera->GetTransform()[2].x, _camera->GetTransform()[2].y, _camera->GetTransform()[2].z);
	vec3df pos = vec3df(_camera->GetTransform()[3].x, _camera->GetTransform()[3].y, _camera->GetTransform()[3].z);
	m_engine->setListenerPosition(pos, facing, vec3df(0, 0, 0), up);
}
void SoundManager::PlaySoundEffect(const char* _filePath) {
	m_engine->play2D(_filePath);
}
void SoundManager::PlaySoundEffect(const char* _filePath, vec3df _pos) {
	m_engine->play3D(_filePath, _pos);
}
void SoundManager::AddSound(const char* _filePath, const char* _name) {
	bool existing = false;
	for (auto i = m_sounds.begin(); i != m_sounds.end(); ++i) {
		if (i->first == _name) {
			existing = true;
		}
	}
	if (!existing) {
		ISoundSource* sound =  m_engine->addSoundSourceFromFile(_filePath, ESM_AUTO_DETECT, true);
		m_sounds.insert(std::pair<std::string, ISoundSource*>(_name, sound));
	}
}
void SoundManager::PlayExistingSound(const char* _name) {
	for (auto i = m_sounds.begin(); i != m_sounds.end(); ++i) {
		if (i->first == _name) {
			m_engine->play2D(i->second);
		}
	}
}
void SoundManager::PlayExistingSound(const char* _name, vec3df _pos) {
	for (auto i = m_sounds.begin(); i != m_sounds.end(); ++i) {
		if (i->first == _name) {
			m_engine->play3D(i->second, _pos);
		}
	}
}
void SoundManager::SetBackgroundMusic(const char* _filePath) {
	if (m_backgroundMusic) {
		m_backgroundMusic->stop();
	}
	m_backgroundMusic = m_engine->play2D(_filePath, true);
}