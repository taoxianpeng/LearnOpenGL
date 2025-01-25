#version 330 core

uniform sampler2D windowTex;

in vec2 vTexCoords;


void main()
{    
   
    gl_FragColor = texture(windowTex, vTexCoords);
}