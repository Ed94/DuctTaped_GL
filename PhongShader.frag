#version 330 core

out vec4 FragColor;


//in vec3 FragPosition;
//in vec3 Normal      ;



uniform vec3 ObjectColor;


//uniform vec3 LightPosition;
uniform vec3 LightColor   ;



void main()
{
	float AmbientStrength = 0.1;

	vec3 ambient = AmbientStrength * LightColor ;

//	vec3 Direction      = normalize(Normal                      );
//	vec3 LightDirection = normalize(LightPosition - FragPosition);  
//
//	float DiffuseStrength  = max(dot(Normal, LightDirection), 0.0);
//	vec3  diffuse          = DiffuseStrength * LightColor         ;

	vec3 result  = (ambient ) * ObjectColor;

	FragColor = vec4(result, 1.0);
}