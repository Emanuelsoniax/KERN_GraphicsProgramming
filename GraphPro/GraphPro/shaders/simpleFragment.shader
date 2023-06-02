#version 330 core
out vec4 FragColor;

in vec3 color;
in vec2 uv;
in mat3 tbn;
in vec3 worldPosition;

uniform sampler2D mainTex;
uniform sampler2D normalTex;
uniform sampler2D specularTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main(){
	//normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);

	//scale down
	normal.rg = normal.rg * .75f;
	normal = normalize(normal);
	//transform with TBN
	normal = tbn * normal;

	vec3 lightDirection = normalize(worldPosition - lightPosition);

	//specular data
	vec3 viewDir = normalize(worldPosition - cameraPosition);
	vec3 reflDir = normalize(reflect(lightDirection, normal));

	//lighting
	float diffuse = max(-dot(normal, lightDirection), 0.0);
	float specular = pow(max(-dot(reflDir, viewDir), 0.0), 2);

	//seperate RGB and RGBA
	vec4 output = vec4(color, 1.0) * texture(mainTex, uv);
	output.rgb = output.rgb * min(diffuse + 0.1, 1.0) + vec3(texture(specularTex, uv) * specular);

	FragColor = output;

	//returns uv as colors
	//FragColor = vec4(uv, 0.0f, 1.0f);
}