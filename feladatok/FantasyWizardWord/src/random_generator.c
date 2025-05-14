#include "random_generator.h"
#include <stdlib.h>

static unsigned int random_seed = 0;

void random_set_seed(unsigned int seed) {
    random_seed = seed;
    srand(seed);
}

unsigned int random_int(void) {
    return rand();
}

float random_float(void) {
    return (float)rand() / (float)RAND_MAX;
}

int random_int_range(int min, int max) {
    if (max <= min) return min; // biztosítjuk, hogy ne legyen hiba
    return min + rand() % (max - min + 1);
}

float random_float_range(float min, float max) {
    if (max <= min) return min; // biztonsági ellenőrzés
    return min + random_float() * (max - min);
}
