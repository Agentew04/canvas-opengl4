#version 400

layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;
layout (location = 2) in vec2 atlasUv;

out vec2 fragAtlasUv;
out vec3 fragColor;

uniform mat4 projection;

void main() {
	fragColor = color;
	fragAtlasUv = atlasUv;
	gl_Position = projection * vec4(position, 1.0);
}
