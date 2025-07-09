/*
* Autor: Cesar Pozzer
* Data: 10/03/2025
* 
* Cabecalho original da Canvas2D.
* Unica adaptacao eh a adicao do fontSize
* nas funcoes text(). Adicionado tambem
* enum com cores nomeadas ao inves de 
* acessar cores por indice.
*/


#pragma once

#include "../Vector2.h"

extern int screenWidth, screenHeight;


class CV //classe Canvas2D
{
public:

    enum class Color {
        Black,
        Gray,
        Red,
        Green,
        Blue,
        Aqua,
        Magenta,
        Yellow,
        Orange,
        Brown,
        Olive,
        Teal,
        Purple,
        White
    };

    //funcoes para desenho de ponto e linha
    static void point(float x, float y); //coordeandas do ponto
    static void point(Vector2 pos); //coordeandas do ponto

    static void line(float x1, float y1, float x2, float y2); //coordenadas da linha x1, y1, x2, y2
    static void line(Vector2 p1, Vector2 p2); //coordenadas da linha (p1, p2)

    //desenha um retangulo alinhado nos eixos x e y
    static void rect(float x1, float y1, float x2, float y2); //coordenadas do retangulo x1, y1, x2, y2
    static void rect(Vector2 p1, Vector2 p2); //coordenadas do retangulo (p1, p2) - TO DO

    static void rectFill(float x1, float y1, float x2, float y2); //coordenadas do retangulo x1, y1, x2, y2
    static void rectFill(Vector2 p1, Vector2 p2); //coordenadas do retangulo (p1, p2)

    //desenha um poligono CONVEXO. Para um retangulo, deve-se passar 4 vertices
    static void polygon(float vx[], float vy[], int n_elems);
    static void polygonFill(float vx[], float vy[], int n_elems);

    //centro e raio do circulo
    static void circle(float x, float y, float radius, int div);
    static void circle(Vector2 pos, float radius, int div);

    static void circleFill(float x, float y, float radius, int div);
    static void circleFill(Vector2 pos, float radius, int div);

    //especifica a cor de desenho e de limpeza de tela
    static void color(float r, float g, float b);
    static void color(float r, float g, float b, float alpha);
    static void color(Color color);

    static void clear(float r, float g, float b);

    //desenha texto na coordenada (x,y)
    static void text(float x, float y, const char* t, float fontSize);
    static void text(Vector2 pos, const char* t, float fontSize);  //varias funcoes ainda nao tem implementacao. Faca como exercicio
    static void text(Vector2 pos, int valor, float fontSize);      //varias funcoes ainda nao tem implementacao. Faca como exercicio
    static void text(Vector2 pos, float valor, float fontSize);    //varias funcoes ainda nao tem implementacao. Faca como exercicio

    //coordenada de offset para desenho de objetos.
    static void translate(float x, float y);
    static void translate(Vector2 pos);

    //funcao de inicializacao da Canvas2D. Recebe a largura, altura, e um titulo para a janela
    static void init(int w, int h, const char* title, bool antiAliasing);

    //funcao para executar a Canvas2D
    static void run();
};