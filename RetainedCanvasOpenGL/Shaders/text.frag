#version 400

in vec2 fragAtlasUv;
in vec3 fragColor;

out vec4 color;

uniform sampler2D atlas;
uniform float threshold;
uniform float smoothness;

float median(float r, float g, float b) {
	return max(min(r, g), min(max(r, g), b));
}

void main() {
	// Sample the texture
	vec3 value = texture(atlas, fragAtlasUv).rgb;

	float alpha = smoothstep(
		threshold - smoothness,
		threshold + smoothness,
		median(value.r, value.g, value.b)
	);
	if (alpha < 0.5) {
		discard;
	}
	color = vec4(fragColor, 1);
}
