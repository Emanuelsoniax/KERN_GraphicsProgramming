#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec4 worldPosition;

uniform sampler2D mainTex;
uniform sampler2D normalTex;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;

void main(){
	//normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.gb = normal.bg;

	//specular data
	//vec3 viewDir = normalize(worldPosition - cameraPosition);
	//vec3 reflDir = normalize(reflect(lightDirection, normal));

	//lighting
	vec3 lightDirection = normalize(worldPosition.rgb - lightPosition);
	float lightValue = max(-dot(normal, lightPosition), 0.0);
	//float specular = pow(max(-dot(reflDir, viewDir), 0.0), 2);

	//seperate RGB and RGBA
	vec4 output = texture(mainTex, uv);
	output.rgb = vec4(uv, 0.0f, 1.0f).rgb + vec3(min(lightValue + 0.1, 1.0)); //+ vec3(texture(specularTex, uv) * specular)

	FragColor = output;

	//returns uv as colors
    //FragColor = vec4(uv, 0.0f, 1.0f);
}