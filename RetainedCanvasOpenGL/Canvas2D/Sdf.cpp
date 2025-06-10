/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
* 
* Classe para renderizacao de fontes ttf 
* pre processadas utilizando a 
* tecnica de MSDF.
*/

#include "Sdf.h"

#include <fstream>

void Sdf::init() {
	this->atlas = readAtlas("Resources/arial.txt");
	this->glyphs = readLayoutFile("Resources/arial.csv");
}

Sdf::~Sdf() {
	delete[] atlas.data;
}

const Sdf::Glyph& Sdf::getGlyph(int codepoint) const {
	return glyphs.at(codepoint);
}

const Sdf::Atlas& Sdf::getAtlas() const {
	return atlas;
}

std::map<int,Sdf::Glyph> Sdf::readLayoutFile(const std::string& filepath) const {
	auto file = std::ifstream(filepath);

	if (!file.is_open()) {
		throw std::runtime_error("Could not open file");
	}

	std::map<int,Glyph> glyphs;
	std::string line;
	while (std::getline(file, line)) {
		Glyph glyph{};
		sscanf_s(line.c_str(), "%d,%f,%f,%f,%f,%f,%f,%f,%f,%f",
			&glyph.codepoint,
			&glyph.advance,
			&glyph.planeLeft,
			&glyph.planeBottom,
			&glyph.planeRight,
			&glyph.planeTop,
			&glyph.atlasLeft,
			&glyph.atlasBottom,
			&glyph.atlasRight,
			&glyph.atlasTop
		);
		glyphs.emplace(glyph.codepoint, glyph);
	}
	return glyphs;
}

Sdf::Atlas Sdf::readAtlas(const std::string& filepath) const {
	auto file = std::ifstream(filepath);

	if (!file.is_open()) {
		throw std::runtime_error("Could not open file");
	}

	Atlas atlas{};
	int width;
	int height;
	file >> width >> height;
	atlas.data = new float[width * height * 3]; // x3 pq msdf eh RGB. single channel eh o SDF normal
	atlas.width = width;
	atlas.height = height;

	for (int i = 0; i < width * height * 3; i++) {
		file >> atlas.data[i];
	}
	return atlas;
}

