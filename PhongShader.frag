#version 330 core

out vec4 FragColor;


in vec3 FragPosition     ;
in vec3 Normal           ;
in vec3 LightViewPosition;
in vec3 LightRawPos;


uniform vec3 ObjectColor;
uniform vec3 LightColor  ;
uniform vec3 ViewPosition;



void main()
{
	// Ambient

	float ambientStrength  = 0.1;

	vec3 ambient = ambientStrength * LightColor ;

	// Diffuse

	vec3 direction      = normalize(Normal                    );
	vec3 lightDirection = normalize(LightViewPosition - FragPosition);  

	float diffuseStrength  = max(dot(direction, lightDirection), 0.0);
	vec3  diffuse          = diffuseStrength * LightColor            ;

	// Specular

	float specularStrength = 0.5;

	vec3 viewDirection = normalize(ViewPosition - FragPosition);
//	vec3 viewDirection = normalize(-FragPosition);

	vec3 reflectionDirection = reflect(-lightDirection, direction);  

	float spec = pow(max(dot(viewDirection, reflectionDirection), 0.0), 32);

    vec3 specular = specularStrength * spec * LightColor;

	// Combining

	vec3 result = (ambient + diffuse + specular) * ObjectColor;

	FragColor = vec4(result, 1.0);
}