#version 410

in vec3 vTexCoords;

out vec4 FragColour;

uniform sampler3D perlinTexture;
uniform sampler2D terrainTexture[4];


void main() {
	float height = texture(perlinTexture, vTexCoords).r - 0.5f;
	vec2 texcoords = vec2(vTexCoords.x * 20, vTexCoords.y * 20);

	vec4 levelOne = texture(terrainTexture[0], texcoords);
	vec4 levelTwo = texture(terrainTexture[1], texcoords);
	vec4 levelThree = texture(terrainTexture[2], texcoords);
	vec4 levelFour = texture(terrainTexture[3], texcoords);
	if(height < 0.2f) {
		FragColour = levelOne;
	} 
	else if(height >= 0.2f && height < 0.3f) {
		FragColour = mix(levelOne, levelTwo, (height - 0.2f) * 10);
	}
	else if(height >= 0.3f && height < 0.4f) {
		FragColour = levelTwo;
	} 
	else if(height >= 0.4f && height < 0.5f) {
		FragColour = mix(levelTwo, levelThree, (height - 0.4f) * 10);
	}
	else if(height >= 0.5f && height < 0.7f) {
		FragColour = levelThree;
	} 
	else if(height >= 0.7f && height < 0.8f) {
		FragColour = mix(levelThree, levelFour, (height - 0.7f) * 10);
	}
	else {
		FragColour = levelFour;	
	}
	FragColour *= height;
	FragColour.a = 1;


	//FragColour = vec4(1, 1, 1, 1);
	//if(height <= 1) {
	//	FragColour.b = height / 5 ;
	//} else {
	//	FragColour.g = height / 5 ;
	//}

}