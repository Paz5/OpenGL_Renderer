#version 430
out vec4 FragColor;

struct Material{

	float shininess;
};

uniform Material material;

uniform vec3 sunDir;
uniform vec3 sunColor;
uniform vec3 ambientColor;
uniform vec3 albedo;
uniform vec3 viewPos;

in vec2 uv;
in vec3 objectNormal;
in vec3 fragPos;

float specularIntensity = 0.8;

uniform layout(binding=0) sampler2D diffuse;
uniform layout(binding=1) sampler2D specular;


void main(void)
{ 
//ambient
	vec3 ambient = ambientColor * texture(diffuse,uv).rgb;

//diffuse
	vec3 norm = normalize(objectNormal);
	vec3 diffuse = sunColor * max(dot(norm,-sunDir),0.0) * texture(diffuse,uv).rgb;
//specular
	vec3 viewDir = normalize(viewPos - fragPos);
	vec3 reflectDir = reflect(sunDir,norm);
	float spec = pow(max(dot(viewDir,reflectDir),0.0),material.shininess);
	vec3 specular = sunColor * specularIntensity * spec * texture(specular,uv).rgb;;

	vec3 color = (diffuse + specular + ambient) * albedo;
	FragColor = vec4(color,1.0);
}