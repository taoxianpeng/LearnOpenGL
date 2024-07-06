#version 330 core

struct Material{
    sampler2D texture_diffuse1;
    sampler2D texture_specular1;
};

uniform Material material;

in vec2 TexCoords;


void main()
{    
   
    gl_FragColor = texture(material.texture_diffuse1, TexCoords);
}