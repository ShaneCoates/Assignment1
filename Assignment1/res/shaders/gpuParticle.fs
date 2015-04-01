#version 410

in vec4 Colour;
in vec2 TexCoords;
in float Time;

out vec4 fragColour;

uniform sampler2D tex1; 
uniform sampler2D tex2;

void main()
{
	fragColour = Colour * mix(texture(tex1, TexCoords), texture(tex2, TexCoords), Time);
}