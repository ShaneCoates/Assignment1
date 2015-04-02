#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoords;

out vec3 vTexCoords;

uniform mat4 ProjectionView;
uniform sampler3D perlinTexture;
uniform float time;
uniform float zValue;
uniform float amplitude;
uniform int size;
void main() {
	vTexCoords = vec3(TexCoords, zValue * 0.5f + 0.5f);
	float height = texture(perlinTexture, vTexCoords).r - 0.5f;
	if(height < 0.5f) {
		height = 0.5f;
	}
	vec4 pos = Position;
	pos.x *= size;
	pos.z *= size;
	pos.y -= 10;
	pos.y += height * amplitude * size;
	pos.y -= amplitude * size;
	gl_Position = ProjectionView * pos;

}