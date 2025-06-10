/*
* Autor: Rodrigo Appelt
* Data: 10/03/2025
*
* Classe para renderizacao de fontes ttf
* pre processadas utilizando a
* tecnica de MSDF.
*/

#pragma once

#include <string>
#include <map>
#include <stdexcept>

class Sdf {
	// Ver https://github.com/Chlumsky/msdf-atlas-gen.

	// arial.csv e arial.txt foram criados com comando: 
	//	.\msdf-atlas-gen.exe -font .\arial.ttf -type msdf -format textfloat -imageout arial.txt -csv arial.csv -pxrange 3
public:
	void init();

	struct Glyph {
	public:
		int codepoint;
		float advance;
		float planeLeft;
		float planeBottom;
		float planeRight;
		float planeTop;
		float atlasLeft;
		float atlasBottom;
		float atlasRight;
		float atlasTop;
	};

	struct Atlas {
		float* data;
		int width;
		int height;
	};

	const Glyph& getGlyph(int codepoint) const;

	const Atlas& getAtlas() const;

	~Sdf();
private:
	std::map<int,Glyph> glyphs;
	Atlas atlas;

	/// <summary>
	/// Le um arquivo csv com o layout do atlas gerado pela ferramenta msdf-atlas-gen.
	/// </summary>
	/// <param name="filepath">O caminho do csv</param>
	/// <returns></returns>
	std::map<int,Glyph> readLayoutFile(const std::string &filepath) const;
	/// <summary>
	/// Le uma textura de floats
	/// </summary>
	/// <param name="filepath"></param>
	/// <returns></returns>
	Atlas readAtlas(const std::string& filepath) const;
};
