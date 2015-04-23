#version 410

in vec4 Colour;
in vec2 TexCoords;
in float Time;

out vec4 fragColour;

uniform sampler2D textureStart;
uniform sampler2D textureEnd;

void main()
{	
    vec4 tStart = texture(textureStart, TexCoords);
	vec4 tEnd = texture(textureEnd, TexCoords);

	vec4 vColour = mix(tStart, tEnd, Time) * Colour;
	fragColour = vColour;
}