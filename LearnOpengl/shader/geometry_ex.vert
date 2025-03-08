#version 330 core
layout(location = 0) in vec2 aPos;
layout(location = 1) in vec3 aColor;

out VS_OUT_DATA {
    vec3 Color;
} vs_out_data;

void main()
{
    gl_Position = vec4(aPos.x, aPos.y, 0.0, 1.0);
    vs_out_data.Color = aColor;
}
