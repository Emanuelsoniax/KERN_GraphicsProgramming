#version 330 core
out vec4 FragColor;

in vec2 uv;
in vec4 worldPosition;

uniform sampler2D mainTex;
uniform sampler2D normalTex;

uniform sampler2D dirt, sand, grass, rock, snow;

uniform vec3 lightPosition;
uniform vec3 cameraPosition;


vec3 lerp(vec3 a, vec3 b, float t) {
	return a + (b - a) * t;
}

vec2 lerp(vec2 a, vec2 b, float t) {
	return a + (b - a) * t;
}

void main(){
	//normal map
	vec3 normal = texture(normalTex, uv).rgb;
	normal = normalize(normal * 2.0 - 1.0);
	normal.gb = normal.bg;

	//specular data
	vec3 viewDir = normalize(worldPosition.rgb - cameraPosition);
	//vec3 reflDir = normalize(reflect(lightDirection, normal));

	//lighting
	vec3 lightDirection = normalize(worldPosition.rgb - lightPosition);
	float lightValue = max(-dot(normal, lightPosition), 0.0);
	//float specular = pow(max(-dot(reflDir, viewDir), 0.0), 2);

	//build color
	float y = worldPosition.y;
	float ds = clamp((y - 25) / 10, -1, 1) * 0.5 + 0.5;			//dirt to sand
	float sg = clamp((y - 50) / 10, -1, 1) * 0.5 + 0.5;		//sand to grass
	float gr = clamp((y - 100) / 10, -1, 1) * 0.5 + 0.5;		//grass to rock
	float rs = clamp((y - 200) / 10, -1, 1) * 0.5 + 0.5;		//rock to snow

	float dist = length(worldPosition.xyz - cameraPosition);
	float uvLerp = clamp((dist - 250) / 150, -1, 1) * 0.5 + 0.5;

	vec3 dirtColorClose = texture(dirt, uv * 100).rgb;		vec3 dirtColorFar = texture(dirt, uv * 10).rgb;
	vec3 sandColorClose = texture(sand, uv * 100).rgb;		vec3 sandColorFar = texture(sand, uv * 10).rgb;
	vec3 grassColorClose = texture(grass, uv * 100).rgb;	vec3 grassColorFar = texture(grass, uv * 10).rgb;
	vec3 rockColorClose = texture(rock, uv * 100).rgb;		vec3 rockColorFar = texture(rock, uv * 10).rgb;
	vec3 snowColorClose = texture(snow, uv * 100).rgb;		vec3 snowColorFar = texture(snow, uv * 100).rgb;

	vec3 dirtColor = lerp(dirtColorClose, dirtColorFar, uvLerp);
	vec3 sandColor = lerp(sandColorClose, sandColorFar, uvLerp);
	vec3 grassColor = lerp(grassColorClose, grassColorFar, uvLerp);
	vec3 rockColor = lerp(rockColorClose, rockColorFar, uvLerp);
	vec3 snowColor = lerp(snowColorClose, snowColorFar, uvLerp);

	vec3 diffuse = lerp(lerp(lerp(lerp(dirtColor, sandColor, ds), grassColor, sg), rockColor, gr), snowColor, rs);

	float fog = pow(clamp((dist - 250) / 1000, 0, 1), 2);

	vec3 topColor = vec3(68.0 / 255.0, 118.0 / 255.0, 189.0 / 255.0);
	vec3 botColor = vec3(118.0 / 255.0, 214.0 / 255.0, 231.0 / 255.0);

	vec3 fogColor = lerp(botColor, topColor, max(viewDir.y, 0.0));

	//seperate RGB and RGBA
	vec4 output = vec4( lerp(diffuse * min(lightValue + 0.1, 1.0), fogColor, fog), 1.0); //+ vec3(texture(specularTex, uv) * specular)

	FragColor = output;

	//returns uv as colors
    //FragColor = vec4(uv, 0.0f, 1.0f);
}