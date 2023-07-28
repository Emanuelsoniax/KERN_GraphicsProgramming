#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec2 uv;
out vec3 normal;
out vec3 worldPosition;

uniform mat4 world, view, projection;

uniform sampler2D mainTex;

void main()
{
	vec4 worldPos = world * vec4(aPos, 1.0);
	//world space offset
	worldPos.y += texture(mainTex, vUV).r * 100.0f;
	
	gl_Position = projection * view *  worldPos;
	normal = vNormal;
	uv = vUV;

	worldPosition = mat3(world) * aPos;
}