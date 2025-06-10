/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
*
* Classe que controla a logica de construcao dos
* comandos enviados pela Canvas2D.
* Tambem gerencia os VBOs, VAOs e shaders.
*/

#pragma once

#include <vector>
#include <glm/glm.hpp>
#include "glslprogram.h"
#include "Sdf.h"

enum class CommandType {
	TYPE_NONE,
	TYPE_POINT,
	TYPE_COLOR,
	TYPE_CLEAR,
	TYPE_RECT,
	TYPE_CIRCLE,
	TYPE_POLY,
	TYPE_LINE,
	TYPE_TRANSLATE,
	TYPE_TEXT
};

struct CommandPoint
{
public:
	float x, y;
};

struct CommandLine
{
public:
	float x1, y1, x2, y2;
};

struct CommandRect
{
public:
	float x1, y1, x2, y2;
	bool fill;
};

struct CommandPolygon
{
public:
	float* vx;
	float* vy;
	int n_elems;
	bool fill;
};

struct CommandCircle
{
public:
	float x, y, radius;
	int div;
	bool fill;
};

struct CommandColor
{
public:
	float r, g, b, alpha;
};

struct CommandClear
{
public:
	float r, g, b;
};

struct CommandText
{
public:
	float x, y;
	const char* t;
	float fontSize;
};

struct CommandTranslate
{
public:
	float x, y;
};

struct Command {
public:
	CommandType type;
	union {
		CommandPoint point;
		CommandColor color;
		CommandClear clear;
		CommandLine line;
		CommandRect rect;
		CommandPolygon polygon;
		CommandCircle circle;
		CommandText text;
		CommandTranslate translate;
	};
};

struct DrawStructure {
public:
	unsigned int vao;
	unsigned int vbo_positions;
	unsigned int vbo_colors;
	unsigned int vbo_uv;
	unsigned int texture;
	std::vector<glm::vec3> positions;
	std::vector<glm::vec3> colors;
	std::vector<glm::vec2> uv;

	void reset();
};

class Controller {
public:
	void init();
	void submit(const Command &command);
	void newFrame();
	void render(int screenWidth, int screenHeight);
private:
	GLSLProgram uiShader;
	GLSLProgram textShader;
	Sdf sdf;
	DrawStructure fillStructure;
	DrawStructure lineStructure;
	DrawStructure textStructure;
	std::vector<Command> commands;
	void compileShaders();

	void createUiBuffers();
	void createTextBuffers();
	void createTextures();

	void build();
	void draw(int screenWidth, int screenHeight);

	// primitive processing
	void processRect(const CommandRect& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
	void processPoint(const CommandPoint& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
	void processCircle(const CommandCircle& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
	void processPoly(const CommandPolygon& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
	void processLine(const CommandLine& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
	void processText(const CommandText& cmd, float z, const glm::vec3& color, const glm::vec2& translation);
};