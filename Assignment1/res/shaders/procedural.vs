#version 410

layout(location = 0) in vec4 Position;
layout(location = 1) in vec2 TexCoords;

out vec3 vTexCoords;

uniform mat4 ProjectionView;
uniform sampler3D perlinTexture;
uniform float time;

void main() {
	vTexCoords = vec3(TexCoords, 0);// sin(time) * 0.5f + 0.5f);
	float height = texture(perlinTexture, vTexCoords).r - 0.5f;
	if(height < 0.5f) {
		height = 0.5f;
	}
	vec4 pos = Position;
	pos.x *= 50;
	pos.z *= 50;
	pos.y -= 50;
	//pos.y += vTexCoords.z * 5;
	pos.y += height * 50;
	//if(pos.y <= 5) {
	//	pos.y = 5;
	//}
	//pos.y += TexCoords.x;
	//pos.y = clamp(pos.y, 0, 1);

	gl_Position = ProjectionView * pos;

}