#version 430

const int MAX_BONES = 100;

layout (location=0) in vec3 position;
layout (location=1) in vec3 normal;
layout (location=2) in vec2 texCoord;
layout (location=3) in ivec4 bone_indices;
layout (location=4) in vec4 bone_weights;

out float distance;
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
    float constant_att;
	float linear_att;
	float quad_att;
};

uniform PositionalLight light;

uniform mat4 m_matrix;
uniform mat4 v_matrix;
uniform mat4 proj_matrix;
uniform mat4 norm_matrix;
uniform mat4 bone_matrices[MAX_BONES];

void main(void)
{
	mat4 bone_matrix = bone_matrices[bone_indices[0]] * bone_weights[0];
    bone_matrix += bone_matrices[bone_indices[1]] * bone_weights[1];
    bone_matrix += bone_matrices[bone_indices[2]] * bone_weights[2];
    bone_matrix += bone_matrices[bone_indices[3]] * bone_weights[3];

	varyingVertPos = (v_matrix * m_matrix * vec4(position, 1.0)).xyz;
	varyingLightDir = (v_matrix * vec4(light.position, 1.0)).xyz - varyingVertPos;
	varyingNormal = (norm_matrix * vec4(normal, 1.0)).xyz;

	distance = length(varyingLightDir);

	gl_Position = proj_matrix * v_matrix * m_matrix * bone_matrix * vec4(position, 1.0);	
	tc = texCoord;
}