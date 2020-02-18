#version 330 core

out vec4 FragColor;


in vec3 FragPosition;
in vec3 Normal      ;



uniform vec3 ObjectColor;


uniform vec3 LightPosition;
uniform vec3 LightColor   ;

uniform vec3 ViewPosition;



void main()
{
	float AmbientStrength  = 0.1;
	float SpecularStrength = 0.5;

	vec3 ambient = AmbientStrength * LightColor ;

	vec3 Direction      = normalize(Normal                      );
	vec3 LightDirection = normalize(LightPosition - FragPosition);  

	float DiffuseStrength  = max(dot(Normal, LightDirection), 0.0);

	vec3  diffuse          = DiffuseStrength * LightColor         ;

	vec3 ViewDirection = normalize(ViewPosition - FragPosition);
    
	vec3 ReflectionDirection = reflect(-LightDirection, Normal);  
    
	float Spec = pow(max(dot(ViewDirection, ReflectionDirection), 0.0), 32);

    vec3 specular = SpecularStrength * Spec * LightColor;  

	vec3 result = (ambient + diffuse + specular) * ObjectColor;

	FragColor = vec4(result, 1.0);

}