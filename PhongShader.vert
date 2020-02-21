#version 330 core

#define VertexIndex  0
#define NormalIndex  1
#define TextureIndex 2

layout (location = VertexIndex ) in vec3 VertPosition;
layout (location = NormalIndex ) in vec3 VertNormal  ;
layout (location = TextureIndex) in vec3 VertTexture ;

out vec3 FragPosition     ;
out vec3 Normal           ;
out vec3 LightViewPosition;

uniform mat4 InverseModelSpace;

uniform mat4 ModelSpace;
uniform mat4 Viewport  ;
uniform mat4 Projection;

uniform vec3 LightPosition;



void main()
{
	gl_Position = Projection * Viewport * ModelSpace * vec4(VertPosition, 1.0);

	FragPosition = vec3(Viewport * ModelSpace * vec4(VertPosition, 1.0));

    Normal = mat3(transpose(inverse(InverseModelSpace))) * VertNormal;

	LightViewPosition = vec3(Viewport * vec4(LightPosition, 1.0));
}