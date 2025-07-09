#version 400

in vec2 fragAtlasUv;
in vec3 fragColor;

out vec4 color;

uniform sampler2D atlas;


void main() {
	// Sample the texture
	float value = texture(atlas, fragAtlasUv).r;

	if (value <= 0.2) {
		discard;
	}
	color = vec4(fragColor, value);
}
