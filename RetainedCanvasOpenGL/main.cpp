#include "Canvas2D/gl_canvas2d.h"

#include <vector>
#include <chrono>

int screenWidth, screenHeight;


std::chrono::high_resolution_clock::time_point lastTime;
std::vector<float> deltas;

int offset = 0;
void render() {
    // get time
    auto currentTime = std::chrono::high_resolution_clock::now();

    CV::translate(0, 0);
    CV::clear(1, 1, 1);
    offset++;
    const int count = 10'000;
    for (int i = 0; i < count; i++) {
        float r = ((i + offset) % 255) / 255.0f;
        float g = ((i + offset +64) % 255) / 255.0f;
        float b = ((i + offset +128) % 255) / 255.0f;
        CV::color(r, g, b);

        // x,y,w,h in function of i
        float x = (i + offset) % screenWidth;
        float y = (i + offset + 64) % screenHeight;
        float w = (i + offset + 128) % 100;
        float h = (i + offset + 192) % 100;
        //CV::rectFill(x, y, x + w, y + h);
        //CV::rect(x, y, x + w, y + h);
        CV::circleFill(x, y, w, h);
        //CV::circle(x, y, w, h);
        //CV::line(x, y, x + w, y + h);
        //CV::line(x, y, x + w, y + h);
    }

    // capturar uma amostra
    if (offset % 10 == 0) {
        std::chrono::duration<float> delta = currentTime - lastTime;
		float deltaFloat = delta.count();
		deltas.push_back(deltaFloat);
        // calcular delta medio
		float sum = 0;
        for (auto d : deltas) {
            sum += d;
        }
		float avg = sum / deltas.size();
		float fps = 1.0f / avg;
		printf("Delta: %f; FPS: %f\n", avg, fps);
    }

    //CV::text(20, 100, "AaBb", 300);
    lastTime = currentTime;
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
    CV::init(800, 600, "Canvas2D");
    CV::run();
}