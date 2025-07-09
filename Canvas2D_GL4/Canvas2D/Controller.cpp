/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
*
* Classe que renderiza comandos vindos da Canvas2D
* usando VAOs, VBOs e shaders. Tambem se encarrega de renderizar
* textos.
*/

#include "Controller.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glslprogram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <array>

void DrawStructure::reset() {
    positions.clear();
    colors.clear();
    uv.clear();
}

Controller::Controller(int& screenWidth, int& screenHeight)
    : screenWidth(screenWidth), screenHeight(screenHeight) {
}

void Controller::compileShaders() {
    try {
        uiShader.compileShader("Shaders/ui.vert", GLSLShader::VERTEX);
        uiShader.compileShader("Shaders/ui.frag", GLSLShader::FRAGMENT);

        uiShader.link();

		textShader.compileShader("Shaders/text.vert", GLSLShader::VERTEX);
		textShader.compileShader("Shaders/text_softmask.frag", GLSLShader::FRAGMENT);
        textShader.link();
    }
    catch (GLSLProgramException& e) {
        printf("Erro ao compilar shader: %s\n", e.what());
        system("pause");
        exit(EXIT_FAILURE);
    }
}

void Controller::init() {
    // agora que nao tem classe especifica pro sdf, lemos aqui
    Atlas::readAtlas("Resources/arial_softmask.txt", font_atlas, 1); // 1 para softmask/sdf. 3 para msdf
	Glyph::readLayoutFile("Resources/arial_softmask.csv", font_glyphs);

    createUiBuffers();
    createTextBuffers();
    createTextures();

    compileShaders();
}

void Controller::createUiBuffers() {
    unsigned int vao[2];
    glGenVertexArrays(2, vao);
    fillStructure.vao = vao[0];
    lineStructure.vao = vao[1];
    glBindVertexArray(fillStructure.vao);
    unsigned int vbo[2];
    glGenBuffers(2, vbo);
    fillStructure.vbo_positions = vbo[0];
    fillStructure.vbo_colors = vbo[1];
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_positions);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_colors);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

    glBindVertexArray(lineStructure.vao);
    glGenBuffers(2, vbo);
    lineStructure.vbo_positions = vbo[0];
    lineStructure.vbo_colors = vbo[1];
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_positions);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_colors);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);
}

void Controller::createTextBuffers() {
    glGenVertexArrays(1, &textStructure.vao);
    glBindVertexArray(textStructure.vao);
    unsigned int vbo[3];
    glGenBuffers(3, vbo);
    textStructure.vbo_positions = vbo[0];
    textStructure.vbo_colors = vbo[1];
    textStructure.vbo_uv = vbo[2];

    glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_positions);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_colors);
    //glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(1);

	glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_uv);
	//glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 0, nullptr);
	glEnableVertexAttribArray(2);
}

void Controller::createTextures() {
    glGenTextures(1, &textStructure.texture);
    glBindTexture(GL_TEXTURE_2D, textStructure.texture);
    
    // params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // upload dos dados
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, font_atlas.width, font_atlas.height, 0, GL_RED, GL_FLOAT, font_atlas.data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Controller::newFrame() {
    fillStructure.reset();
    lineStructure.reset();
	textStructure.reset();
}

void Controller::drawFilled() {
    mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.0f, 10.0f);
    uiShader.use();
    uiShader.setUniform("projection", projection);

    // draw triangles
    glBindVertexArray(fillStructure.vao);
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, fillStructure.positions.size() * sizeof(glm::vec3), fillStructure.positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, fillStructure.colors.size() * sizeof(glm::vec3), fillStructure.colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, fillStructure.positions.size());
}

void Controller::drawLines() {
    mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.0f, 10.0f);
    uiShader.use();
    uiShader.setUniform("projection", projection);

    // draw lines
    glBindVertexArray(lineStructure.vao);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, lineStructure.positions.size() * sizeof(glm::vec3), lineStructure.positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, lineStructure.colors.size() * sizeof(glm::vec3), lineStructure.colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, lineStructure.positions.size());
}

void Controller::drawText() {
    mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.0f, 10.0f);
    textShader.use();
    textShader.setUniform("projection", projection);
    textShader.setUniform("threshold", 0.45f);
    textShader.setUniform("smoothness", 0.5f);
    glBindTexture(GL_TEXTURE_2D, textStructure.texture);
    textShader.setUniform("atlas", textStructure.texture);

    // draw text
    glBindVertexArray(textStructure.vao);
    glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, textStructure.positions.size() * sizeof(glm::vec3), textStructure.positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, textStructure.colors.size() * sizeof(glm::vec3), textStructure.colors.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, textStructure.vbo_uv);
    glBufferData(GL_ARRAY_BUFFER, textStructure.uv.size() * sizeof(glm::vec2), textStructure.uv.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, textStructure.positions.size());
    glBindTexture(GL_TEXTURE_2D, 0);
}

void Controller::processRect(const CommandRect& cmd) {
	float x1 = cmd.x1 + translation.x;
	float y1 = cmd.y1 + translation.y;
	float x2 = cmd.x2 + translation.x;
	float y2 = cmd.y2 + translation.y;

    if (cmd.fill) {
        fillStructure.reset();
        fillStructure.positions.push_back(glm::vec3(x1, y1, 0));
        fillStructure.positions.push_back(glm::vec3(x2, y1, 0));
        fillStructure.positions.push_back(glm::vec3(x2, y2, 0));
        fillStructure.positions.push_back(glm::vec3(x1, y1, 0));
        fillStructure.positions.push_back(glm::vec3(x2, y2, 0));
        fillStructure.positions.push_back(glm::vec3(x1, y2, 0));
        for (int i = 0; i < 6; i++) {
            fillStructure.colors.push_back(color);
        }
        drawFilled();
    }
    else {
		lineStructure.reset();
        lineStructure.positions.push_back(glm::vec3(x1, y1, 0));
        lineStructure.positions.push_back(glm::vec3(x2, y1, 0));
        lineStructure.positions.push_back(glm::vec3(x2, y1, 0));
        lineStructure.positions.push_back(glm::vec3(x2, y2, 0));
        lineStructure.positions.push_back(glm::vec3(x2, y2, 0));
        lineStructure.positions.push_back(glm::vec3(x1, y2, 0));
        lineStructure.positions.push_back(glm::vec3(x1, y2, 0));
        lineStructure.positions.push_back(glm::vec3(x1, y1, 0));
        for (int i = 0; i < 8; i++) {
            lineStructure.colors.push_back(color);
        }
		drawLines();
    }
}

void Controller::processPoint(const CommandPoint& cmd){
	float x = cmd.x + translation.x;
	float y = cmd.y + translation.y;
	fillStructure.reset();
    fillStructure.positions.push_back(glm::vec3(x, y, 0));
    fillStructure.positions.push_back(glm::vec3(x+1, y, 0));
    fillStructure.positions.push_back(glm::vec3(x+1, y+1, 0));
    fillStructure.positions.push_back(glm::vec3(x, y, 0));
    fillStructure.positions.push_back(glm::vec3(x+1, y+1, 0));
    fillStructure.positions.push_back(glm::vec3(x, y+1, 0));
    for (int i = 0; i < 6; i++) {
        fillStructure.colors.push_back(color);
    }
	drawFilled();
}

void Controller::processCircle(const CommandCircle& cmd){
	float x = cmd.x + translation.x;
	float y = cmd.y + translation.y;
	DrawStructure& structure = cmd.fill ? fillStructure : lineStructure;
	structure.reset();
    for (int i = 0; i < cmd.div; i++) {
        float angle1 = 2 * 3.14159265359f * i / cmd.div;
        float angle2 = 2 * 3.14159265359f * (i + 1) / cmd.div;
        float x1 = x + cos(angle1) * cmd.radius;
        float y1 = y + sin(angle1) * cmd.radius;
        float x2 = x + cos(angle2) * cmd.radius;
        float y2 = y + sin(angle2) * cmd.radius;

        if (cmd.fill) {
			
            structure.positions.push_back(glm::vec3(x, y, 0));
            structure.positions.push_back(glm::vec3(x1, y1, 0));
            structure.positions.push_back(glm::vec3(x2, y2, 0));
            for (int j = 0; j < 3; j++) {
                structure.colors.push_back(color);
            }
        }
        else {
            structure.positions.push_back(glm::vec3(x1, y1, 0));
            structure.positions.push_back(glm::vec3(x2, y2, 0));
            for (int j = 0; j < 2; j++) {
                structure.colors.push_back(color);
            }
        }
    }
	if (cmd.fill) {
		drawFilled();
	}
    else {
        drawLines();
    }
}

void Controller::processPoly(const CommandPolygon& cmd){
    // TODO: simple fan triangulation
}

void Controller::processLine(const CommandLine& cmd){
	float x1 = cmd.x1 + translation.x;
	float y1 = cmd.y1 + translation.y;
	float x2 = cmd.x2 + translation.x;
	float y2 = cmd.y2 + translation.y;
	lineStructure.reset();
	lineStructure.positions.push_back(glm::vec3(x1, y1, 0));
	lineStructure.positions.push_back(glm::vec3(x2, y2, 0));
    for (int i = 0; i < 2; i++) {
        lineStructure.colors.push_back(color);
    }
	drawLines();
}

void Controller::processText(const CommandText& cmd) {
	glm::vec2 cursor = { cmd.x + translation.x, cmd.y + translation.y };
	const char* str = cmd.t;
    int i = 0;
    char currentChar = str[0];
	textStructure.reset();
    while (currentChar != '\0') {
		const Glyph& glyph = font_glyphs[currentChar];

        // emit tris
		glm::vec2 uvBottomLeft = { glyph.atlasLeft / font_atlas.width, glyph.atlasBottom / font_atlas.height };
		glm::vec2 uvTopRight = { glyph.atlasRight / font_atlas.width, glyph.atlasTop / font_atlas.height };

		float x1 = cursor.x + glyph.planeLeft * cmd.fontSize;
		float y1 = cursor.y + glyph.planeBottom * cmd.fontSize;
		float x2 = cursor.x + glyph.planeRight * cmd.fontSize;
		float y2 = cursor.y + glyph.planeTop * cmd.fontSize;

		textStructure.positions.push_back(glm::vec3(x1, y1, 0));
		textStructure.positions.push_back(glm::vec3(x2, y1, 0));
		textStructure.positions.push_back(glm::vec3(x2, y2, 0));
		textStructure.positions.push_back(glm::vec3(x1, y1, 0));
		textStructure.positions.push_back(glm::vec3(x2, y2, 0));
		textStructure.positions.push_back(glm::vec3(x1, y2, 0));

		for (int i = 0; i < 6; i++) {
			textStructure.colors.push_back(color);
		}

		textStructure.uv.push_back(glm::vec2(uvBottomLeft.x, uvBottomLeft.y));
		textStructure.uv.push_back(glm::vec2(uvTopRight.x, uvBottomLeft.y));
		textStructure.uv.push_back(glm::vec2(uvTopRight.x, uvTopRight.y));
		textStructure.uv.push_back(glm::vec2(uvBottomLeft.x, uvBottomLeft.y));
		textStructure.uv.push_back(glm::vec2(uvTopRight.x, uvTopRight.y));
		textStructure.uv.push_back(glm::vec2(uvBottomLeft.x, uvTopRight.y));
        
        // next char
		cursor.x += glyph.advance * cmd.fontSize;
		currentChar = str[++i];
    }
	if (textStructure.positions.size() > 0) {
		drawText();
	}
}

void Controller::processColor(const CommandColor& cmd) {
	color.r = cmd.r;
	color.g = cmd.g;
	color.b = cmd.b;
}
void Controller::processTranslate(const CommandTranslate& cmd) {
	translation.x = cmd.x;
	translation.y = cmd.y;
}

void Controller::processClear(const CommandClear& cmd) {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glClearColor(cmd.r, cmd.g, cmd.b, 1.0f);
}