#version 400

/*
Autor: Rodrigo Appelt
Data: 09/07/2025

Fragment shader para renderizar textos usando
um atlas estilo softmask. O valor lido do atlas eh 
repassado como alfa do fragmento.
*/

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
