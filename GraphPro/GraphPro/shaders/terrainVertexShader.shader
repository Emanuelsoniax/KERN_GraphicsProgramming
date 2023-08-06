#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vNormal;
layout(location = 2) in vec2 vUV;

out vec2 uv;
out vec4 worldPosition;

uniform mat4 world, view, projection;

uniform sampler2D mainTex;
uniform sampler2D normalTex;

void main()
{
	worldPosition = world * vec4(aPos, 1.0);
	//world space offset
	//worldPos.y += texture(mainTex, vUV).r * 200.0f;
	
	gl_Position = projection * view *  worldPosition;
	uv = vUV;
}