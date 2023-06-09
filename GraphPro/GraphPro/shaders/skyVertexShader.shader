#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 3) in vec3 vNormal;

out vec3 worldPosition;

uniform mat4 world, view, projection;

void main()
{
	gl_Position = projection * view *  world * vec4(aPos, 1.0);

	uv = vUV;

	worldPosition = mat3(world) * aPos;
}