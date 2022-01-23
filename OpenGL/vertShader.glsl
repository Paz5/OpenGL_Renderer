#version 430
layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoords;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform mat3 n_matrix;

out vec2 uv;
out vec3 objectNormal;
out vec3 fragPos;

void main(void)
{ 
	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position,1.0);
	fragPos = (m_matrix * vec4(position,1.0)).xyz;
	objectNormal = n_matrix * (vec4(normal,1.0)).xyz;
	uv = texCoords;
}