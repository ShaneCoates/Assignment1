/*
Author: Shane Coates
Description: GPU Particle Emitter class using 3D perlin noise
*/
#ifndef _PERLIN_PARTICLE_EMITTER_H_
#define _PERLIN_PARTICLE_EMITTER_H_
#include <glm.hpp>
struct Particle
{
	Particle() : lifetime(1), lifespan(0) {}

	glm::vec3 position;
	glm::vec3 velocity;
	float lifetime;
	float lifespan;
};

class PerlinParticleEmitter
{
public:

	PerlinParticleEmitter();
	virtual ~PerlinParticleEmitter();

	void Init(unsigned int _maxParticles,
		float _minLifeSpan, float _maxLifeSpan,
		float _minVelocity, float _maxVelocity,
		float _startSize, float _endSize,
		const glm::vec4& _startColour, const glm::vec4& _endColour);

	void Draw(float _time, const glm::mat4& _cameraTransform, const glm::mat4& _projectionView);
	bool emitting;

protected:

	void CreateBuffers();
	void CreateUpdateShader();
	void CreateDrawShader();
	void GeneratePerlin(unsigned int _size);

	Particle* m_particles;

	unsigned int m_maxParticles;

	glm::vec3 m_position;

	float m_minLifeSpan;
	float m_maxLifeSpan;

	float m_minVelocity;
	float m_maxVelocity;

	float m_startSize;
	float m_endSize;

	glm::vec4 m_startColour;
	glm::vec4 m_endColour;

	unsigned int m_activeBuffer;
	unsigned int m_vao[2];
	unsigned int m_vbo[2];

	unsigned int m_drawShader;
	unsigned int m_updateShader;

	float m_lastDrawTime;

	unsigned int m_perlinTexture;

private:
};

#endif