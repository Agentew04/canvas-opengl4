/*
* Autor original: Cesar Pozzer
* Autor adaptacao: Rodrigo Appelt
* Data: 10/03/2025
* 
* Versao adaptada da Canvas2D para suporte
* ao OpenGL 4.X e renderizacao avancada de texto.
*/

#include "gl_canvas2d.h"

#include "Controller.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "glslprogram.h"
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <string>

GLFWwindow* window;
double mouseX = 0, mouseY = 0;
Controller controller(screenWidth, screenHeight);

const float Colors[14][3] =
{
    {0, 0, 0}, //Black
    {0.5, 0.5, 0.5}, //Gray
    {1, 0, 0}, //Red
    {0, 1, 0}, //Green
    {0, 0, 1}, //Blue
    {0, 1, 1}, //Aqua
    {1, 0, 1}, //Magenta
    {1, 1, 0}, //Yellow
    {1, 0.5, 0}, //Orange
    {0.5, 0, 0}, //Brown
    {0.5, 0.5, 0}, //Olive
    {0, 0.5, 0.5}, // Teal
    {0.5, 0, 0.5}, // Purple
    {1, 1, 1}, //white
};

// callbacks para o usuario implementar
void render();
void keyboard(int key);
void keyboardUp(int key);
void mouse(int button, int state, int wheel, int direction, int x, int y);

// iniciar
void initGlfw(int w, int h, const char* title, bool antiAliasing);
void initCallbacks();
void initGlew();
void initOpenGL(int w, int h);


#pragma region Init

void CV::init(int w, int h, const char* title, bool antiAliasing)
{
    initGlfw(w, h, title, antiAliasing); // inicializa glfw e janela
    initGlew(); // inicializa glew
    initCallbacks(); // seta callbacks pro user
    initOpenGL(w, h);
    
    // inicializa controlador do retained mode opengl
    controller.init();
}

void initGlfw(int w, int h, const char* title, bool antiAliasing) {
    if (!glfwInit()) {
        // get error description
        const char* description;
        glfwGetError(&description);
        // print error description
        printf("Error: %s\n", description);
        exit(EXIT_FAILURE);
    }

    screenWidth = w;
    screenHeight = h;

    /*glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);*/
    if (antiAliasing) {
        glfwWindowHint(GLFW_SAMPLES, 8); // 8xMSAA
    }
    else {
        glfwWindowHint(GLFW_SAMPLES, 0); // desliga anti-aliasing
    }

    window = glfwCreateWindow(screenWidth, screenHeight, title, NULL, NULL);
    if (!window)
    {
        printf("Nao consegui criar janela do GLFW :(\n");
        glfwTerminate();
        system("pause");
        exit(EXIT_FAILURE);
    }
    glfwMakeContextCurrent(window);
}

void initGlew() {
    glewExperimental = GL_TRUE;
    GLenum err = glewInit(); // <- sem isso todos os gl_* depois do 1.0 nao funcionam
    if (GLEW_OK != err)
    {
        printf("Erro ao inicializar o GLEW: %s\n", glewGetErrorString(err));
        exit(EXIT_FAILURE);
    }
}

void initCallbacks() {
    // ainda tem q arrumar
    glfwSetKeyCallback(window, [](GLFWwindow*, int key, int scancode, int action, int mods) {
        printf("Key: %d, Action: %d, Mods: %d\n", key, action, mods);
        if (action == GLFW_PRESS) {
            keyboard(key);
        }
        else if (action == GLFW_RELEASE) {
            keyboardUp(key);
        }
    });

    // button click
    glfwSetMouseButtonCallback(window, [](GLFWwindow* window, int button, int action, int mods) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouse(button, action, -2, -2, static_cast<int>(xpos), static_cast<int>(ypos));
    });

    // scroll whell callback
    glfwSetScrollCallback(window, [](GLFWwindow* window, double xoffset, double yoffset) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        mouse(-2, -2, 0, static_cast<int>(yoffset), static_cast<int>(xpos), static_cast<int>(ypos));
    });
    // atualiza viewport e as variaveis do usuario
    glfwSetFramebufferSizeCallback(window, [](GLFWwindow* window, int width, int height) {
        glViewport(0, 0, width, height);
        screenHeight = height;
        screenWidth = width;
    });
    glfwSetErrorCallback([](int error, const char* description) {
        printf("Error: %s\n", description);
    });
}

void initOpenGL(int w, int h) {
    // Configuracoes de OpenGL
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

    glDisable(GL_DEPTH_TEST); // habilitar isso se estivar usando retained mode!
    glDepthFunc(GL_LESS);

	// teoricamente desabilita o anti-aliasing
    glDisable(GL_MULTISAMPLE);

    // habilita transparencia
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA); 
}

#pragma endregion

void CV::run() {
    while (!glfwWindowShouldClose(window))
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        controller.newFrame();
        render(); // funcao do usuario
        glfwSwapBuffers(window);
        glfwPollEvents();

        // verifica se o mouse moveu
        double epsilon = 0.01;
        double x, y;
        glfwGetCursorPos(window, &x, &y);
        if (fabs(x - mouseX) > epsilon || fabs(y - mouseY) > epsilon) {
            // mouse move event
            mouse(-2, -2, -2, -2, static_cast<int>(x), static_cast<int>(y));
            mouseX = x;
            mouseY = y;
        }
    }

    glfwTerminate();
}

#pragma region User Commands

void CV::point(float x, float y)
{
	CommandPoint cmd{};
    cmd.x = x;
    cmd.y = y;
    controller.processPoint(cmd);
}

void CV::point(Vector2 pos)
{
    point(pos.x, pos.y);
}

void CV::line(float x1, float y1, float x2, float y2)
{
    CommandLine cmd{};
    if (x1 > x2 && y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    cmd.x1 = x1;
    cmd.y1 = y1;
    cmd.x2 = x2;
    cmd.y2 = y2;
    controller.processLine(cmd);
}

void CV::line(Vector2 p1, Vector2 p2)
{
    line(p1.x, p1.y, p2.x, p2.y);
}

void CV::rect(float x1, float y1, float x2, float y2)
{
    CommandRect cmd{};
    if (x1 > x2 && y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    cmd.x1 = x1;
    cmd.y1 = y1;
    cmd.x2 = x2;
    cmd.y2 = y2;
    cmd.fill = false;
    controller.processRect(cmd);
}

void CV::rect(Vector2 p1, Vector2 p2)
{
    rect(p1.x, p1.y, p2.x, p2.y);
}

void CV::rectFill(float x1, float y1, float x2, float y2)
{
    CommandRect cmd{};
    if (x1 > x2 && y1 > y2) {
        std::swap(x1, x2);
        std::swap(y1, y2);
    }
    cmd.x1 = x1;
    cmd.y1 = y1;
    cmd.x2 = x2;
    cmd.y2 = y2;
    cmd.fill = true;
	controller.processRect(cmd);
}

void CV::rectFill(Vector2 p1, Vector2 p2)
{
    rectFill(p1.x, p1.y, p2.x, p2.y);
}

void CV::polygon(float vx[], float vy[], int n_elems)
{
    CommandPolygon cmd{};
    cmd.vx = vx;
    cmd.vy = vy;
    cmd.n_elems = n_elems;
    cmd.fill = false;
    controller.processPoly(cmd);
}

void CV::polygonFill(float vx[], float vy[], int n_elems)
{
     CommandPolygon cmd{};
    cmd.vx = vx;
    cmd.vy = vy;
    cmd.n_elems = n_elems;
    cmd.fill = true;
    controller.processPoly(cmd);
}

void CV::circle(float x, float y, float radius, int div)
{
    CommandCircle cmd{};
    cmd.x = x;
    cmd.y = y;
    cmd.radius = radius;
    cmd.div = div;
    cmd.fill = false;
    controller.processCircle(cmd);
}

void CV::circle(Vector2 pos, float radius, int div)
{
    circle(pos.x, pos.y, radius, div);
}

void CV::circleFill(float x, float y, float radius, int div)
{
    CommandCircle cmd{};
    cmd.x = x;
    cmd.y = y;
    cmd.radius = radius;
    cmd.div = div;
    cmd.fill = true;
    controller.processCircle(cmd);
}

void CV::circleFill(Vector2 pos, float radius, int div)
{
    circleFill(pos.x, pos.y, radius, div);
}

void CV::color(float r, float g, float b, float alpha)
{
    CommandColor cmd{};
    cmd.g = g;
    cmd.r = r;
    cmd.b = b;
    cmd.alpha = alpha;
    controller.processColor(cmd);
}

void CV::color(float r, float g, float b)
{
    color(r, g, b, 1);
}

void CV::color(CV::Color color)
{
	const int index = static_cast<int>(color);
    CV::color(Colors[index][0], Colors[index][1], Colors[index][2]);
}

void CV::clear(float r, float g, float b)
{
    CommandClear cmd{};
    cmd.r = r;
    cmd.g = g;
    cmd.b = b;
    controller.processClear(cmd);
}

void CV::text(float x, float y, const char* t, float fontSize)
{
    CommandText cmd{};
    cmd.x = x;
    cmd.y = y;
    cmd.t = t;
    cmd.fontSize = fontSize;
    controller.processText(cmd);
}

void CV::text(Vector2 pos, const char* t, float fontSize)
{
    text(pos.x, pos.y, t, fontSize);
}

void CV::text(Vector2 pos, int valor, float fontSize)
{
    text(pos.x, pos.y, std::to_string(valor).c_str(), fontSize);
}

void CV::text(Vector2 pos, float valor, float fontSize)
{
    text(pos.x, pos.y, std::to_string(valor).c_str(), fontSize);
}

void CV::translate(float x, float y)
{
    CommandTranslate cmd{};
    cmd.x = x;
    cmd.y = y;
    controller.processTranslate(cmd);
}

void CV::translate(Vector2 pos)
{
    translate(pos.x, pos.y);
}

#pragma endregion
