#version 430

in float distance;
in vec3 varyingVertPos;
in vec3 varyingLightDir;
in vec3 varyingNormal;
in vec2 tc;

out vec4 fragColor;

uniform sampler2D diffuse;
//uniform sampler2D specular;
//uniform sampler2D normal;
//uniform sampler2D height;

//bool has_diffuse;
//bool has_specular;
//bool has_normal;
//bool has_height;

struct Material
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
uniform Material material;

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

uniform vec4 global_ambient;

void main(void)
{
	vec4 texture_color = texture(diffuse, tc);

	vec3 L = normalize(varyingLightDir);
	vec3 N = normalize(varyingNormal);
	vec3 V = normalize(-varyingVertPos);
	vec3 R = normalize(reflect(-L, N));

	float cosTheta = dot(L,N);
	float cosPhi = dot(V,R);

	vec3 ambient = ((global_ambient * vec4(material.ambient, 1.0f)) + (light.ambient * vec4(material.ambient, 1.0f))).xyz;
	vec3 diffuse = light.diffuse.xyz * material.diffuse * max(cosTheta, 0.0);
	vec3 specular = light.specular.xyz * material.specular * pow(max(cosPhi, 0.0), material.shininess * 3);

	float att = 1 / (light.constant_att + light.linear_att * distance + light.quad_att * distance * distance);

	//if(has_diffuse) {
		fragColor = texture_color * vec4(ambient + (diffuse + specular) * clamp(att, 0.0, 1.0), texture_color.a);
	//} else {
	//	fragColor = vec4((ambient + diffuse + specular), 1.0);
	//}
}