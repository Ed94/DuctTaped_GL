#version 330 core

layout (location = 0) in vec3 VertPosition;
layout (location = 1) in vec3 VertNormal  ;
layout (location = 2) in vec3 VertTexture ;

out vec3 FragPosition     ;
out vec3 Normal           ;
out vec3 LightViewPosition;
out vec3 LightRawPos;

uniform mat4 InverseModelSpace;

uniform mat4 ModelSpace;
uniform mat4 Viewport  ;
uniform mat4 Projection;

uniform vec3 LightPosition;



void main()
{
	gl_Position = Projection * Viewport * ModelSpace * vec4(VertPosition, 1.0);

	FragPosition = vec3(Viewport * ModelSpace * vec4(VertPosition, 1.0));

    Normal = mat3(transpose(inverse(Viewport * ModelSpace))) * VertNormal;

	LightViewPosition = vec3(Viewport * vec4(LightPosition, 1.0));

	LightRawPos = LightPosition;
}