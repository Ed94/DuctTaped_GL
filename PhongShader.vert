#version 330 core

layout (location = 0) in vec3 VertPosition;
layout (location = 1) in vec3 VertNormal  ;

out vec3 FragPosition;
out vec3 Normal      ;

uniform mat4 ModelSpace       ;
uniform mat4 Viewport         ;
uniform mat4 Projection       ;

uniform mat4 InverseModelSpace;



void main()
{
	FragPosition = vec3(ModelSpace * vec4(VertPosition, 1.0));

    Normal = mat3(transpose(InverseModelSpace)) * VertNormal;
//    Normal = VertNormal;

	gl_Position = Projection * Viewport * ModelSpace * vec4(FragPosition, 1.0);
}