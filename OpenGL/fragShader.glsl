#version 430
out vec4 FragColor;

uniform vec3 sunPos;
uniform vec3 sunColor;
uniform vec3 ambientColor;
uniform vec3 albedo;
uniform vec3 viewPos;

layout(binding = 0) uniform sampler2D samp;

in vec2 uv;
in vec3 objectNormal;
in vec3 fragPos;

float specularIntensity = 0.8;

void main(void)
{ 
//ambient
	vec3 ambient = ambientColor * albedo;

//diffuse
	vec3 norm = normalize(objectNormal);
	vec3 lightDir = normalize(sunPos - fragPos);
	vec3 diffuse = sunColor * max(dot(norm,lightDir),0.0);
//specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(-lightDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),64);
	vec3 specular = sunColor * specularIntensity * spec;

	vec3 color = (diffuse + specular + ambient) * albedo;
	FragColor = vec4(diffuse + specular + ambient,1.0);
}