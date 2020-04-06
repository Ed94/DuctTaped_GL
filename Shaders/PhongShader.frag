#version 330 core



out vec4 FragColor;

in vec3 FragPosition     ;
in vec3 Normal           ;
in vec3 LightViewPosition;

uniform vec3 ObjectColor ;
uniform vec3 LightColor  ;

uniform float AmbientStrength ;
uniform float DiffuseStrength ;
uniform float SpecularStrength;



void main()
{
	// Ambient

	vec3 ambient = AmbientStrength * LightColor ;

	// Diffuse

	vec3 direction      = normalize(Normal                          );
	vec3 lightDirection = normalize(LightViewPosition - FragPosition);  

	float diffuseMagnitude = max(dot(direction, lightDirection), 0.0);

	vec3  diffuse = DiffuseStrength * diffuseMagnitude * LightColor;

	// Specular

	vec3 viewDirection = normalize(-FragPosition);

	vec3 reflectionDirection = reflect(-lightDirection, direction);  

	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

    vec3 specular = SpecularStrength * spec * LightColor;

	// Combining

	vec3 result = (ambient + diffuse + specular) * ObjectColor;

	FragColor = vec4(result, 1.0);
}