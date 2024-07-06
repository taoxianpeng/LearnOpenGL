#version 330 core
out vec4 FragColor;

uniform vec3 lightColor;

void main()
{
	// linearly interpolate between both textures (80% container, 20% awesomeface)
	FragColor = vec4(lightColor, 1.0);
}