#version 330 core

layout (location = 0) in vec3 aPos;   



uniform mat4 ScreenSpaceTransform;



void main()
{
    gl_Position = ScreenSpaceTransform * vec4(aPos, 1.0);
}