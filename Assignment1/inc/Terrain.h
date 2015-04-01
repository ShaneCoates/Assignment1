/*
	Author: Shane Coates
	Description: Terrain class
*/

#ifndef _TERRAIN_H_
#define	_TERRAIN_H_
class Camera;
struct TerrainVertex {
	glm::vec4 Position;
	glm::vec2 UV;
};
class Terrain {
public:
	Terrain();
	~Terrain();

	void Update(double _dt);
	void Draw(Camera* _camera);

	void ReloadShaders();

protected:
private:
	void GenerateGrid(unsigned int _size);
	void CreateTexture(unsigned int _size);
	void LoadTextures(const char* _path[4]);
	
	unsigned int m_program;
	unsigned int m_perlinTexture;
	unsigned int m_terrainTextures[4];
	unsigned int m_VAO, m_VBO, m_IBO;
	unsigned int m_indexCount;

};

#endif