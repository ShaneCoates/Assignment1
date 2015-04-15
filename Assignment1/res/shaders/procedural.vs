#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoords;
layout(location = 2) in vec3 Normal; 

out vec4 vPosition;
out vec3 vTexCoords;
out vec3 vNormal;

uniform mat4 ProjectionView;
uniform sampler3D perlinTexture;
uniform float time;
uniform float zValue;
uniform float amplitude;
uniform float size;
void main() {
	vec2 tex = TexCoords;

	vTexCoords = vec3(tex, zValue * 0.5f + 0.5f);
	//vTexCoords = vec3(Position.x, Position.y, Position.z);
	float height = texture(perlinTexture, vTexCoords).r - (0.5f + amplitude);

	vec4 pos = Position;
	pos.x *= size;
	pos.z *= size;
	pos.y *= size;
	//pos += vec4(vNormal, 1) * (1 - height ) * 0.01f;
	vPosition = pos;
	gl_Position = ProjectionView * vPosition;
	vNormal = Normal;
}