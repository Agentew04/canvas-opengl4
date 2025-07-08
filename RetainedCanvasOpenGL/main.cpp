#include "Canvas2D/gl_canvas2d.h"

#include <vector>
#include <chrono>
#define GLEW_STATIC
#include <GL/glew.h>
#include <glm/glm.hpp>

int screenWidth, screenHeight;

void render() {
    CV::clear(1, 0, 0);
	CV::translate(100, 100);
    CV::color(1, 1, 1);
    CV::rectFill(100, 100, 200, 200);
    CV::color(0, 1, 1);
	CV::text(150, 150, "Hello, Canvas2D!", 20);
}

void keyboard(int key) {
}

void keyboardUp(int key) {
    //printf("Liberou a tecla: %c\n", key);
}

void mouse(int button, int state, int wheel, int direction, int x, int y)
{
    //printf("Mouse button: %d, state: %d, wheel: %d, direction: %d, x: %d, y: %d\n", button, state, wheel, direction, x, y);
}

int main(void)
{
    CV::init(800, 600, "Canvas2D", true);
    CV::run();
}