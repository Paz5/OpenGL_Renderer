#version 430
out vec4 FragColor;
uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform vec3 sunColor;

void main(void)
{ 
	FragColor = vec4(sunColor,1.0);
}