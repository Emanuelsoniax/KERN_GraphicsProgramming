#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in mat3 tbn;
in vec4 worldPosition;
in vec3 normals;

uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

const float levels = 3.0f;

void main() {
	//normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.b = -normal.b;
	normal.gb = normal.bg;

	//scale down
	//normal.rg = normal.rg * .75f;
	//transform with TBN
	//normal = tbn * normal;

	vec3 lightDirection = normalize(vec3(lightPosition.x, -lightPosition.y, lightPosition.z));

	//specular data
	vec3 viewDir = normalize(worldPosition.rgb - cameraPosition);
	vec3 reflDir = normalize(reflect(lightDirection, normal));

	//lighting
	float intensity = max(-dot(normal, lightDirection), 0.05);
	float specular = pow(max(-dot(reflDir, viewDir), 0.0), 2);

	float level = floor(intensity * levels);
	intensity = level / levels;
	specular = level / levels;

	//seperate RGB and RGBA
	vec4 output = vec4(color, 1.0)*texture(mainTex, uv);
	output.rgb = output.rgb * min(intensity + 0.5, 1.0) +vec3(texture(specularTex, uv) * specular);

	FragColor = output;

	//returns uv as colors
	//FragColor = vec4(uv, 0.0f, 1.0f) * min(intensity + 0.5, 1.0);
}