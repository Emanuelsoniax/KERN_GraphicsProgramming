#version 330 core
layout(location = 0) in vec3 aPos;
layout(location = 1) in vec3 vColor;
layout(location = 2) in vec2 vUV;
layout(location = 3) in vec3 vNormal;
layout(location = 4) in vec3 vTangent;
layout(location = 5) in vec3 vBiTangent;

out vec3 color;
out vec2 uv;
out mat3 tbn;
out vec4 worldPosition;
out vec3 normals;

uniform mat4 world, view, projection;

void main()
{
	worldPosition = world * vec4(aPos, 1.0);
	gl_Position = projection * view * worldPosition;

	normals = normalize(mat3(inverse(transpose(world))) * vNormal);

	color = vColor;
	uv = vUV;
	vec3 n = normalize(mat3(world) * vNormal);
	vec3 b = normalize(mat3(world) * vTangent);
	vec3 t = normalize(mat3(world) * vBiTangent);
	tbn = mat3(t, b, n);
}