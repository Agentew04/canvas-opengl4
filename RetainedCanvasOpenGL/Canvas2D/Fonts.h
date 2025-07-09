#pragma once
/*
* Autor: Rodrigo Appelt
* Data: 08/07/2025
* 
* Arquivo com definicoes de structs usados por todas as
* classes que renderizam fontes. Criado para nao duplicar
* codigo nos arquivos de renderizacao (M)SDF e SOFTMASK.
*/

// Ver https://github.com/Chlumsky/msdf-atlas-gen.
// arial.csv e arial.txt foram criados com comando: 
//	.\msdf-atlas-gen.exe -font .\arial.ttf -type msdf -format textfloat -imageout arial.txt -csv arial.csv -pxrange 3

#include <string>
#include <map>

/// <summary>
/// Representa um glifo de fonte que pode ser renderizado.
/// Tem dados sobre coordenadas UV no atlas e tamanho/deslocamento do caractere.
/// </summary>
struct Glyph {
public:
	// informacoes sobre o glifo. 
	// Quem ele eh
	int codepoint;
	// quanto ele avanca na horizontal p/ o texto
	float advance;

	// define a posicao/tamanho do rect desse glifo
	float planeLeft;
	float planeBottom;
	float planeRight;
	float planeTop;

	// coordenadas UV no atlas
	float atlasLeft;
	float atlasBottom;
	float atlasRight;
	float atlasTop;

	/// <summary>
	/// Le um arquivo csv com o layout do atlas gerado pela ferramenta msdf-atlas-gen.
	/// </summary>
	/// <param name="filepath">O caminho do csv</param>
	/// <returns></returns>
	static void readLayoutFile(const std::string& filepath, std::map<int, Glyph>& out);
};

/// <summary>
/// Struct que representa um atlas de fonte. Eh basicamente um
/// wrapper para uma textura.
/// </summary>
struct Atlas {
	/// <summary>
	/// Ponteiro para os dados do atlas
	/// </summary>
	float* data;

	/// <summary>
	/// A largura em pixels da textura
	/// </summary>
	int width;

	/// <summary>
	/// A altura em pixels da textura
	/// </summary>
	int height;

	/// <summary>
	/// Libera o vetor de textura.
	/// </summary>
	~Atlas();

	/// <summary>
	/// Le uma textura de floats
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns></returns>
	static void readAtlas(const std::string& filepath, Atlas& atlas);
};
