#version 430

layout (location=0) in vec3 pos;

uniform mat4 proj_matrix;
uniform mat4 v_matrix;
uniform mat4 m_matrix;

void main(void)
{
	//gl_Position = proj_matrix * v_matrix * m_matrix * vec4(pos,1.0);
	gl_Position = vec4(pos,1.0);
} 
