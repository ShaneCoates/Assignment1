/*
	Author: Shane Coates
	Description: Sound Manager class
				 uses irrKlang 3rd party library
*/

#ifndef _SOUND_MANAGER_H_
#define	_SOUND_MANAGER_H_
#include <irrKlang.h>
#include <map>
#include <string>
class Camera;
using namespace irrklang;
class SoundManager
{
public:
	SoundManager();
	~SoundManager();

	void Update(Camera* _camera);

	void PlaySoundEffect(const char* _filePath);
	void PlaySoundEffect(const char* _filePath, vec3df _pos);

	void AddSound(const char* _filePath, const char* _name);
	void PlayExistingSound(const char* _name);
	void PlayExistingSound(const char* _name, vec3df _pos);


	void SetBackgroundMusic(const char* _filePath);
private:

	ISoundEngine* m_engine;
	ISound* m_backgroundMusic;

	std::map<std::string, ISoundSource*> m_sounds;
};


#endif