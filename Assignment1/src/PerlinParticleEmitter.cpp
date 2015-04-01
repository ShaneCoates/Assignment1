#include "PerlinParticleEmitter.h"
#include <gl_core_4_4.h>
#include <ext.hpp>
#include "ShaderLoader.h"
#include "Camera.h"
#include <stb_image.h>
PerlinParticleEmitter::PerlinParticleEmitter() : m_particles(nullptr), m_maxParticles(0), m_position(0, 0, 0), m_drawShader(0), m_updateShader(0), m_lastDrawTime(0) {
	for (int i = 0; i <= 1; ++i) {
		m_vao[i] = 0;
		m_vbo[i] = 0;
	}
}
PerlinParticleEmitter::~PerlinParticleEmitter() {
	delete[] m_particles;
	glDeleteVertexArrays(2, m_vao);
	glDeleteBuffers(2, m_vbo);
	glDeleteProgram(m_drawShader);
	glDeleteProgram(m_updateShader);
}
void PerlinParticleEmitter::Init(unsigned int _maxParticles, float _minLifeSpan, float _maxLifeSpan, float _minVelocity, float _maxVelocity, float _startSize, float _endSize, const glm::vec4& _startColour, const glm::vec4& _endColour) {
	m_maxParticles = _maxParticles;
	m_minLifeSpan = _minLifeSpan;
	m_maxLifeSpan = _maxLifeSpan;
	m_minVelocity = _minVelocity;
	m_maxVelocity = _maxVelocity;
	m_startSize = _startSize;
	m_endSize = _endSize;
	m_startColour = _startColour;
	m_endColour = _endColour;
	m_particles = new Particle[_maxParticles];
	m_activeBuffer = 0;
	CreateBuffers();
	CreateUpdateShader();
	CreateDrawShader();
	GeneratePerlin(10);
	emitting = false;
}
void PerlinParticleEmitter::Draw(float _time, const glm::mat4& _cameraTransform, const glm::mat4& _projectionView) {
	glUseProgram(m_updateShader);
	int loc = glGetUniformLocation(m_updateShader, "time");
	glUniform1f(loc, _time);
	float deltaTime = _time - m_lastDrawTime;
	m_lastDrawTime = _time;
	loc = glGetUniformLocation(m_updateShader, "emitting");
	glUniform1i(loc, emitting);
	loc = glGetUniformLocation(m_updateShader, "deltaTime");
	glUniform1f(loc, deltaTime);
	loc = glGetUniformLocation(m_updateShader, "emitterPosition");
	glUniform3fv(loc, 1, &m_position[0]);
	glEnable(GL_RASTERIZER_DISCARD);
	glBindVertexArray(m_vao[m_activeBuffer]);
	unsigned int otherBuffer = (m_activeBuffer + 1) % 2;
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, m_vbo[otherBuffer]);
	glBeginTransformFeedback(GL_POINTS);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);
	glEndTransformFeedback();
	glDisable(GL_RASTERIZER_DISCARD);
	glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);
	glUseProgram(m_drawShader);
	loc = glGetUniformLocation(m_drawShader, "projectionView");
	glUniformMatrix4fv(loc, 1, false, &_projectionView[0][0]);
	loc = glGetUniformLocation(m_drawShader, "cameraTransform");
	glUniformMatrix4fv(loc, 1, false, &_cameraTransform[0][0]);
	loc = glGetUniformLocation(m_drawShader, "perlin");
	glUniform1i(loc, 0);
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_3D, m_perlinTexture);
	glBindVertexArray(m_vao[otherBuffer]);
	glDrawArrays(GL_POINTS, 0, m_maxParticles);
	m_activeBuffer = otherBuffer;
}
void PerlinParticleEmitter::CreateBuffers() {
	glGenVertexArrays(2, m_vao);
	glGenBuffers(2, m_vbo);
	glBindVertexArray(m_vao[0]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[0]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), m_particles, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);
	glBindVertexArray(m_vao[1]);
	glBindBuffer(GL_ARRAY_BUFFER, m_vbo[1]);
	glBufferData(GL_ARRAY_BUFFER, m_maxParticles * sizeof(Particle), 0, GL_STREAM_DRAW);
	glEnableVertexAttribArray(0);
	glEnableVertexAttribArray(1);
	glEnableVertexAttribArray(2);
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), 0);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 12);
	glVertexAttribPointer(2, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 24);
	glVertexAttribPointer(3, 1, GL_FLOAT, GL_FALSE, sizeof(Particle), ((char*)0) + 28);
	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}
void PerlinParticleEmitter::CreateDrawShader() {
	unsigned int vs = ShaderLoader::LoadShader("res/shaders/gpuParticle.vs", GL_VERTEX_SHADER);
	unsigned int gs = ShaderLoader::LoadShader("res/shaders/gpuParticle.gs", GL_GEOMETRY_SHADER);
	unsigned int fs = ShaderLoader::LoadShader("res/shaders/gpuParticlePerlin.fs", GL_FRAGMENT_SHADER);
	m_drawShader = glCreateProgram();
	glAttachShader(m_drawShader, vs);
	glAttachShader(m_drawShader, gs);
	glAttachShader(m_drawShader, fs);
	glLinkProgram(m_drawShader);
	int success = GL_FALSE;
	glGetProgramiv(m_drawShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_drawShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];
		glGetProgramInfoLog(m_drawShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
	}
	glDeleteShader(vs);
	glDeleteShader(gs);
	glDeleteShader(fs);
	glUseProgram(m_drawShader);
	int loc = glGetUniformLocation(m_drawShader, "sizeStart");
	glUniform1f(loc, m_startSize);
	loc = glGetUniformLocation(m_drawShader, "sizeEnd");
	glUniform1f(loc, m_endSize);
	loc = glGetUniformLocation(m_drawShader, "colourStart");
	glUniform4fv(loc, 1, &m_startColour[0]);
	loc = glGetUniformLocation(m_drawShader, "colourEnd");
	glUniform4fv(loc, 1, &m_endColour[0]);
}
void PerlinParticleEmitter::CreateUpdateShader() {
	unsigned int vs = ShaderLoader::LoadShader("res/shaders/gpuParticleUpdatePerlin.vs", GL_VERTEX_SHADER);
	m_updateShader = glCreateProgram();
	glAttachShader(m_updateShader, vs);
	const char* varyings[] = { "position", "velocity", "lifetime", "lifespan" };
	glTransformFeedbackVaryings(m_updateShader, 4, varyings, GL_INTERLEAVED_ATTRIBS);
	glLinkProgram(m_updateShader);
	int success = GL_FALSE;
	glGetProgramiv(m_updateShader, GL_LINK_STATUS, &success);
	if (success == GL_FALSE) {
		int infoLogLength = 0;
		glGetProgramiv(m_updateShader, GL_INFO_LOG_LENGTH, &infoLogLength);
		char* infoLog = new char[infoLogLength];

		glGetProgramInfoLog(m_updateShader, infoLogLength, 0, infoLog);
		printf("Error: Failed to link shader program!\n");
		printf("%s", infoLog);
		printf("\n");
		delete[] infoLog;
	}
	glDeleteShader(vs);
	glUseProgram(m_updateShader);
	int loc = glGetUniformLocation(m_updateShader, "lifeMin");
	glUniform1f(loc, m_minLifeSpan);
	loc = glGetUniformLocation(m_updateShader, "lifeMax");
	glUniform1f(loc, m_maxLifeSpan);
	loc = glGetUniformLocation(m_updateShader, "velocityMin");
	glUniform1f(loc, m_minVelocity);
	loc = glGetUniformLocation(m_updateShader, "velocityMax");
	glUniform1f(loc, m_maxVelocity);
}
void PerlinParticleEmitter::GeneratePerlin(unsigned int _size)
{
	float *perlin_data = new float[_size * _size * _size];
	float scale = (1.0f / _size) * 3;
	int octaves = 10;
	for (unsigned int r = 0; r < _size; ++r) {
		for (unsigned int c = 0; c < _size; ++c) {
			for (unsigned int d = 0; d < _size; ++d) {
				float amplidute = 1.f;
				float persistence = 0.3f;
				perlin_data[r + _size*c + _size*_size*d] = glm::perlin(glm::vec3(r, c, d) * scale) * 0.5f + 0.5f;

				for (int o = 0; o < octaves; ++o) {
					float freq = powf(2, (float)o);
					float perlin_sample = glm::perlin(glm::vec3((float)r, (float)c, (float)d) * scale * freq) * 0.5f + 0.5f;
					perlin_data[r + _size*c + _size*_size*d] += perlin_sample * amplidute;
					amplidute *= persistence;
				}
			}
		}
	}

	glGenTextures(1, &m_perlinTexture);
	glBindTexture(GL_TEXTURE_3D, m_perlinTexture);
	glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, _size, _size, _size, 0, GL_RED, GL_FLOAT, perlin_data);

	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

}