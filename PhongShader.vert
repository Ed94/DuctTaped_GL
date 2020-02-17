#version 330 core

layout (location = 0) in vec3 VertPosition;
//layout (location = 1) in vec3 VertNormal  ;



//out vec3 FragPosition;
//out vec3 Normal      ;



uniform mat4 ModelScreenSpace ;
uniform mat4 ModelSpace       ;
uniform mat4 InverseModelSpace;



void main()
{
	gl_Position = ModelScreenSpace * vec4(VertPosition, 1.0);

//	FragPosition = vec3(ModelSpace * vec4(VertPosition, 1.0));

//	Normal = mat3(transpose(InverseModelSpace));
}