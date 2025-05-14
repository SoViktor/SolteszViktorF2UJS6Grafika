#include "engine.h"

int main(int argc, char* argv[]) {
    (void)argc;
    (void)argv;
    if (!engine_init("Wizard Game", 800, 600)) {
        return 1;
    }

    engine_run();
    engine_shutdown();
    return 0;
}