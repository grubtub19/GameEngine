#version 430

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;

out vec3 varyingVertPos;
out vec3 varyingLightDir;
out vec3 varyingNormal;
out vec2 tc;

struct PositionalLight
{
    vec4 ambient;
    vec4 diffuse;
    vec4 specular;
    vec3 position;
};
uniform PositionalLight light;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;

void main(void)
{
	varyingVertPos = (v_matrix * m_matrix * vec4(position, 1.0)).xyz;
	varyingLightDir = light.position - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(normal, 1.0)).xyz;

	gl_Position = proj_matrix * v_matrix * m_matrix * vec4(position, 1.0);
	tc = texCoord;
}