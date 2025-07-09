#version 400

/*
Autor: Rodrigo Appelt
Data: 09/07/2025

Vertex shader que repassa informacoes uteis para
o fragment shader. A uniform 'projection' eh a projection
matrix que traduz coordenadas da tela para [-1,1]
*/

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
