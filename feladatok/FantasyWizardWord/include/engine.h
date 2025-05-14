#pragma once
#include <stdbool.h>

bool engine_init(const char* title, int width, int height);
void engine_run(void);
void engine_shutdown(void);