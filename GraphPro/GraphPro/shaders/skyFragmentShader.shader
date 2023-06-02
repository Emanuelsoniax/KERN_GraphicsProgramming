#version 330 core
out vec4 FragColor;

in vec3 worldPosition;

uniform vec3 lightDirection;
uniform vec3 cameraPosition;

void main(){

	//specular data
	vec3 viewDir = normalize(worldPosition - cameraPosition);

	FragColor = vec4(1,0,0,1);

	//returns uv as colors
	//FragColor = vec4(uv, 0.0f, 1.0f);
}