#include "Fonts.h"
/*
* Autor: Rodrigo Appelt
* Data: 08/07/2025
*
* Arquivo com definicoes de structs usados por todas as
* classes que renderizam fontes. Criado para nao duplicar
* codigo nos arquivos de renderizacao (M)SDF e SOFTMASK.
*/

#include <fstream>
#include <iostream>

void Atlas::readAtlas(const std::string& filepath, Atlas& atlas) {
	auto file = std::ifstream(filepath);

	if (!file.is_open()) {
		std::cout << "Erro ao abrir o arquivo: " << filepath << std::endl;
		throw std::runtime_error("Could not open file");
	}
	int width;
	int height;
	file >> width >> height;
	atlas.data = new float[width * height * 3]; // x3 pq msdf eh RGB. single channel eh o SDF normal
	atlas.width = width;
	atlas.height = height;

	for (int i = 0; i < width * height * 3; i++) {
		file >> atlas.data[i];
	}
	file.close();
	std::cout << "Atlas carregado: " << filepath << " (" << width << "x" << height << ")" << std::endl;
}

Atlas::~Atlas() {
	if (data) {
		// deixar esse print aqui. caso o usuario esteja usando memoria errada e o atlas esteja sendo liberado,
		// isso vai avisar ele o que esta acontecendo de errado.
		std::cout << "Liberando memoria do atlas" << std::endl;
		free(data);
	}
}

void Glyph::readLayoutFile(const std::string& filepath, std::map<int, Glyph>& out)
{
	auto file = std::ifstream(filepath);

	if (!file.is_open()) {
		throw std::runtime_error("Could not open file");
	}
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
		out.emplace(glyph.codepoint, glyph);
	}
	std::cout << "Glyphs carregados: " << out.size() << " do arquivo " << filepath << std::endl;
}
