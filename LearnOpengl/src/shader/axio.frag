#version 330 core


in vec4 fAmbient;
in vec4 fDiffuse;
in vec4 fSpecular;

void main()
{    
    
    gl_FragColor = fDiffuse; 

}