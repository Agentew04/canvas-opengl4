/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
*
* Classe que gerencia os recursos do OpenGL 4.0
* e desenho de primitivas 2D.
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
	Controller(int& screenWidth, int& screenHeight);

	/// <summary>
	/// Compila shaders, cria texturas e buffers.
	/// </summary>
	void init();
	void newFrame();

	void processColor(const CommandColor& cmd);
	void processTranslate(const CommandTranslate& cmd);
	void processClear(const CommandClear& cmd);
	void processRect(const CommandRect& cmd);
	void processPoint(const CommandPoint& cmd);
	void processCircle(const CommandCircle& cmd);
	void processPoly(const CommandPolygon& cmd);
	void processLine(const CommandLine& cmd);
	void processText(const CommandText& cmd);
private:
	int &screenWidth;
	int &screenHeight;
	GLSLProgram uiShader;
	GLSLProgram textShader;
	Sdf sdf;
	// essas structures tem os vbos e vaos para cada tipo de desenho
	DrawStructure fillStructure;
	DrawStructure lineStructure;
	DrawStructure textStructure;

	void drawFilled();
	void drawLines();
	void drawText();
	void compileShaders();
	void createUiBuffers();
	void createTextBuffers();
	void createTextures();

	// variaveis de controle internas
	glm::vec2 translation{ 0.0f, 0.0f };
	glm::vec3 color{ 0.0f, 0.0f, 0.0f };
};