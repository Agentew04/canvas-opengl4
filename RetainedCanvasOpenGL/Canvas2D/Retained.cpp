/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
*
* Classe que controla a logica de construcao dos
* comandos enviados pela Canvas2D. 
* Tambem gerencia os VBOs, VAOs e shaders.
*/

#include "Retained.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "glslprogram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <array>

#include "Sdf.h"


void DrawStructure::reset() {
    positions.clear();
    colors.clear();
    uv.clear();
}

void Controller::compileShaders() {
    try {
        uiShader.compileShader("Shaders/ui.vert", GLSLShader::VERTEX);
        uiShader.compileShader("Shaders/ui.frag", GLSLShader::FRAGMENT);

        uiShader.link();

		textShader.compileShader("Shaders/text.vert", GLSLShader::VERTEX);
		textShader.compileShader("Shaders/text.frag", GLSLShader::FRAGMENT);
        textShader.link();
    }
    catch (GLSLProgramException& e) {
        printf("Erro ao compilar shader: %s\n", e.what());
        system("pause");
        exit(EXIT_FAILURE);
    }
}

void Controller::submit(const Command &cmd) {
    commands.push_back(cmd);
}

void Controller::init() {
    sdf.init();

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
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, 0, nullptr, GL_DYNAMIC_DRAW);
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
    auto atlas = sdf.getAtlas();
    float* data = atlas.data;
    int width = atlas.width;
    int height = atlas.height;

    glGenTextures(1, &textStructure.texture);
    glBindTexture(GL_TEXTURE_2D, textStructure.texture);
    
    // params
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    // upload dos dados
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_FLOAT, data);

    glBindTexture(GL_TEXTURE_2D, 0);
}

void Controller::newFrame() {
    commands.clear();
    fillStructure.reset();
    lineStructure.reset();
	textStructure.reset();
}

void Controller::render(int screenWidth, int screenHeight) {
    build();
    draw(screenWidth, screenHeight);
}

void Controller::build() {
    glm::vec2 currentTranslation{ 0.0f, 0.0f };
    glm::vec3 currentColor{ 0.0f, 0.0f, 0.0f };
    float far = commands.size() * 0.1f + 1;

    for (int i = 0; i < commands.size(); i++) {
        const Command &cmd = commands[i];
        float z = far - i * 0.1f;
        switch (cmd.type) {
        case CommandType::TYPE_NONE:
            printf("Invalid NONE command!\n");
            break;
        case CommandType::TYPE_POINT:
			processPoint(cmd.point, z, currentColor, currentTranslation);
            break;
        case CommandType::TYPE_COLOR:
            currentColor[0] = cmd.color.r;
            currentColor[1] = cmd.color.g;
            currentColor[2] = cmd.color.b;
            break;
        case CommandType::TYPE_CLEAR:
            glClearColor(cmd.clear.r, cmd.clear.g, cmd.clear.b, 1.0f);
            break;
        case CommandType::TYPE_RECT:
            processRect(cmd.rect, z, currentColor, currentTranslation);
            break;
        case CommandType::TYPE_CIRCLE:
            processCircle(cmd.circle, z, currentColor, currentTranslation);
            break;
        case CommandType::TYPE_POLY:
            processPoly(cmd.polygon, z, currentColor, currentTranslation);
            break;
        case CommandType::TYPE_LINE:
            processLine(cmd.line, z, currentColor, currentTranslation);
            break;
        case CommandType::TYPE_TRANSLATE:
			currentTranslation[0] = cmd.translate.x;
			currentTranslation[1] = cmd.translate.y;
            break;
        case CommandType::TYPE_TEXT:
            processText(cmd.text, z, currentColor, currentTranslation);
            break;
        }
    }

}

void Controller::draw(int screenWidth, int screenHeight) {
    float far = commands.size() * 0.1f + 1;
    mat4 projection = glm::ortho(0.0f, (float)screenWidth, 0.0f, (float)screenHeight, 0.0f, far);
    uiShader.use();
    uiShader.setUniform("projection", projection);
    
    // draw triangles
    glBindVertexArray(fillStructure.vao);
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, fillStructure.positions.size() * sizeof(glm::vec3), fillStructure.positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, fillStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, fillStructure.colors.size() * sizeof(glm::vec3), fillStructure.colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_TRIANGLES, 0, fillStructure.positions.size());

    // draw lines
    glBindVertexArray(lineStructure.vao);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_positions);
    glBufferData(GL_ARRAY_BUFFER, lineStructure.positions.size() * sizeof(glm::vec3), lineStructure.positions.data(), GL_DYNAMIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, lineStructure.vbo_colors);
    glBufferData(GL_ARRAY_BUFFER, lineStructure.colors.size() * sizeof(glm::vec3), lineStructure.colors.data(), GL_DYNAMIC_DRAW);
    glDrawArrays(GL_LINES, 0, lineStructure.positions.size());

    // draw text
    textShader.use();
	textShader.setUniform("projection", projection);
    textShader.setUniform("threshold", 0.45f);
    textShader.setUniform("smoothness", 0.5f);
    glBindTexture(GL_TEXTURE_2D, textStructure.texture);
    textShader.setUniform("atlas", textStructure.texture);
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

void Controller::processRect(const CommandRect& cmd, float z, const glm::vec3& color, const glm::vec2& translation) {
	float x1 = cmd.x1 + translation.x;
	float y1 = cmd.y1 + translation.y;
	float x2 = cmd.x2 + translation.x;
	float y2 = cmd.y2 + translation.y;

    if (cmd.fill) {
        fillStructure.positions.push_back(glm::vec3(x1, y1, -z));
        fillStructure.positions.push_back(glm::vec3(x2, y1, -z));
        fillStructure.positions.push_back(glm::vec3(x2, y2, -z));
        fillStructure.positions.push_back(glm::vec3(x1, y1, -z));
        fillStructure.positions.push_back(glm::vec3(x2, y2, -z));
        fillStructure.positions.push_back(glm::vec3(x1, y2, -z));
        for (int i = 0; i < 6; i++) {
            fillStructure.colors.push_back(color);
        }
    }
    else {
        lineStructure.positions.push_back(glm::vec3(x1, y1, -z));
        lineStructure.positions.push_back(glm::vec3(x2, y1, -z));
        lineStructure.positions.push_back(glm::vec3(x2, y1, -z));
        lineStructure.positions.push_back(glm::vec3(x2, y2, -z));
        lineStructure.positions.push_back(glm::vec3(x2, y2, -z));
        lineStructure.positions.push_back(glm::vec3(x1, y2, -z));
        lineStructure.positions.push_back(glm::vec3(x1, y2, -z));
        lineStructure.positions.push_back(glm::vec3(x1, y1, -z));
        for (int i = 0; i < 8; i++) {
            lineStructure.colors.push_back(color);
        }
    }
}

void Controller::processPoint(const CommandPoint& cmd, float z, const glm::vec3& color, const glm::vec2& translation){
	float x = cmd.x + translation.x;
	float y = cmd.y + translation.y;
    fillStructure.positions.push_back(glm::vec3(x, y, -z));
    fillStructure.positions.push_back(glm::vec3(x+1, y, -z));
    fillStructure.positions.push_back(glm::vec3(x+1, y+1, -z));
    fillStructure.positions.push_back(glm::vec3(x, y, -z));
    fillStructure.positions.push_back(glm::vec3(x+1, y+1, -z));
    fillStructure.positions.push_back(glm::vec3(x, y+1, -z));
    for (int i = 0; i < 6; i++) {
        fillStructure.colors.push_back(color);
    }
}

void Controller::processCircle(const CommandCircle& cmd, float z, const glm::vec3& color, const glm::vec2& translation){
	float x = cmd.x + translation.x;
	float y = cmd.y + translation.y;
    for (int i = 0; i < cmd.div; i++) {
        float angle1 = 2 * 3.14159265359f * i / cmd.div;
        float angle2 = 2 * 3.14159265359f * (i + 1) / cmd.div;
        float x1 = x + cos(angle1) * cmd.radius;
        float y1 = y + sin(angle1) * cmd.radius;
        float x2 = x + cos(angle2) * cmd.radius;
        float y2 = y + sin(angle2) * cmd.radius;

        if (cmd.fill) {
            fillStructure.positions.push_back(glm::vec3(x, y, -z));
            fillStructure.positions.push_back(glm::vec3(x1, y1, -z));
            fillStructure.positions.push_back(glm::vec3(x2, y2, -z));
            for (int j = 0; j < 3; j++) {
                fillStructure.colors.push_back(color);
            }
        }
        else {
            lineStructure.positions.push_back(glm::vec3(x1, y1, -z));
            lineStructure.positions.push_back(glm::vec3(x2, y2, -z));
            for (int j = 0; j < 2; j++) {
                lineStructure.colors.push_back(color);
            }
        }
    }
}

void Controller::processPoly(const CommandPolygon& cmd, float z, const glm::vec3& color, const glm::vec2& translation){
    // TODO: simple fan triangulation
}

void Controller::processLine(const CommandLine& cmd, float z, const glm::vec3& color, const glm::vec2& translation){
	float x1 = cmd.x1 + translation.x;
	float y1 = cmd.y1 + translation.y;
	float x2 = cmd.x2 + translation.x;
	float y2 = cmd.y2 + translation.y;
	lineStructure.positions.push_back(glm::vec3(x1, y1, -z));
	lineStructure.positions.push_back(glm::vec3(x2, y2, -z));
    for (int i = 0; i < 2; i++) {
        lineStructure.colors.push_back(color);
    }
}

void Controller::processText(const CommandText& cmd, float z, const glm::vec3& color, const glm::vec2& translation) {
	glm::vec2 cursor = { cmd.x + translation.x, cmd.y + translation.y };
	auto str = cmd.t;
	const Sdf::Atlas &atlas = sdf.getAtlas();
    int i = 0;
    char currentChar = str[0];
    while (currentChar != '\0') {
		const Sdf::Glyph &glyph = sdf.getGlyph(currentChar);

        // emit tris
		glm::vec2 uvBottomLeft = { glyph.atlasLeft / atlas.width, glyph.atlasBottom / atlas.height };
		glm::vec2 uvTopRight = { glyph.atlasRight / atlas.width, glyph.atlasTop / atlas.height };

		float x1 = cursor.x + glyph.planeLeft * cmd.fontSize;
		float y1 = cursor.y + glyph.planeBottom * cmd.fontSize;
		float x2 = cursor.x + glyph.planeRight * cmd.fontSize;
		float y2 = cursor.y + glyph.planeTop * cmd.fontSize;

		textStructure.positions.push_back(glm::vec3(x1, y1, -z));
		textStructure.positions.push_back(glm::vec3(x2, y1, -z));
		textStructure.positions.push_back(glm::vec3(x2, y2, -z));
		textStructure.positions.push_back(glm::vec3(x1, y1, -z));
		textStructure.positions.push_back(glm::vec3(x2, y2, -z));
		textStructure.positions.push_back(glm::vec3(x1, y2, -z));

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
}