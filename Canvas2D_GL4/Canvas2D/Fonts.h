#pragma once
/*
* Autor: Rodrigo Appelt
* Data: 08/07/2025
* 
* Arquivo com definicoes de structs usados por todas as
* classes que renderizam fontes.
* 
* Remarks:
* Ver https://github.com/Chlumsky/msdf-atlas-gen.
* arial.csv e arial.txt foram criados com comando: 
* .\msdf-atlas-gen.exe -font .\arial.ttf -type msdf -format textfloat -imageout arial.txt -csv arial.csv -pxrange 3
* .\msdf-atlas-gen.exe -font .\arial.ttf -type softmask -format textfloat -imageout arial_softmask.txt -csv arial.csv -pxrange 3
*   -> Adicionar width e height na 1a linha do .txt
*/


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
	/// <param name="out">Referencia para o mapa onde os glifos serao armazenados.</param>
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
	/// Le uma textura de floats. Espera um arquivo no formato: 
	/// width height\nfloat float float...\nfloat float float ...EOF
	/// </summary>
	/// <param name="filepath">O caminho do arquivo do atlas</param>
	/// <param name="pixelSize">Quantos bytes cada pixel tem. Para SDF e softmask, usar 1. Para MSDF usar 3.</param>
	static void readAtlas(const std::string& filepath, Atlas& atlas, int pixelSize);
};
