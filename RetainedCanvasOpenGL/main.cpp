#include "Canvas2D/gl_canvas2d.h"

int screenWidth, screenHeight;

void render() {
    CV::clear(0, 0, 0);
	CV::translate(100, 100);
    CV::color(CV::Color::Teal);
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