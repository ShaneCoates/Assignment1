#version 410

in vec3 vTexCoords;

out vec4 FragColour;

uniform sampler3D perlinTexture;
uniform sampler2D terrainTexture[3];


void main() {
	float height = texture(perlinTexture, vTexCoords).r - 0.5f;
	vec2 texcoords = vec2(vTexCoords.x * 40, vTexCoords.y * 40);

	vec4 levelOne = texture(terrainTexture[0], texcoords);
	vec4 levelTwo = texture(terrainTexture[1], texcoords);
	vec4 levelThree = texture(terrainTexture[2], texcoords);
	if(height < 0.4f) {
		FragColour = levelOne;
	} 
	else if(height >= 0.4f && height < 0.5f) {
		FragColour = mix(levelOne, levelTwo, (height - 0.4f) * 10);
	}
	else if(height >= 0.5f && height < 0.6f) {
		FragColour = levelTwo;
	} 
	else if(height >= 0.6f && height < 0.7f) {
		FragColour = mix(levelTwo, levelThree, (height - 0.6f) * 10);
	}
	else{
		FragColour = levelThree;
	} 
	//else if(height >= 0.8f && height < 0.9f) {
	//	FragColour = mix(levelThree, levelFour, (height - 0.8f) * 10);
	//}
	//else {
	//	FragColour = levelFour;	
	//}
	if(height < 1) {
		FragColour *= height;
	}
	FragColour.a = 1;


	//FragColour = vec4(1, 1, 1, 1);
	//if(height <= 1) {
	//	FragColour.b = height / 5 ;
	//} else {
	//	FragColour.g = height / 5 ;
	//}

}