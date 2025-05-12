#include "window.h"

int main() {
    Window window;
    if (!window.init())
        return -1;
    window.mainLoop();
    return 0;
}
