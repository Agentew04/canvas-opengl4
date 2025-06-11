#include "Canvas2D/gl_canvas2d.h"

#include <vector>
#include <chrono>

int screenWidth, screenHeight;


std::chrono::high_resolution_clock::time_point lastTime;
std::vector<float> deltas;

int offset = 0;
void render() {
    CV::clear(1, 0.2, 0.2);
    CV::color(1, 1, 1);
	CV::rect(100, 100, 200, 200);
    CV::text(300, 300, "Hello, World!", 50);
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