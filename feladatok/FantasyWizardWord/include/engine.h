#ifndef ENGINE_H
#define ENGINE_H

#include <stdbool.h>

// Engine elindítása, true ha sikeres
bool engine_init(const char* title, int width, int height);

// A fő futtató ciklus
void engine_run(void);

// Leállítás, memória felszabadítás
void engine_shutdown(void);

#endif // ENGINE_H
