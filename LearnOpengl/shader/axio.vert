#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec3 aNorm;
layout (location = 2) in vec3 aTexCoords;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

layout(std140) uniform Mat{
	vec4 aAmbient;
	vec4 aDiffuse;
	vec4 aSpecular;
}mat;

out vec3 TexCoords;
out vec3 FragPos;
out vec3 Normal;
out vec4 fAmbient;
out vec4 fDiffuse;
out vec4 fSpecular;

void main()
{
	FragPos = vec3(model * vec4(aPos, 1.0));
	Normal = mat3(transpose(inverse(model)))*aNorm;
	fAmbient = mat.aAmbient;
	fDiffuse = mat.aDiffuse;
	fSpecular = mat.aSpecular;
	TexCoords = aTexCoords;
	gl_Position = projection * view * model * vec4(aPos, 1.0);
}