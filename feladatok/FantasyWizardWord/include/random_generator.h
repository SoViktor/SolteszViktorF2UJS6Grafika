#ifndef RANDOM_GENERATOR_H
#define RANDOM_GENERATOR_H

// Seed beállítása
void random_set_seed(unsigned int seed);

// Egész random szám 0..RAND_MAX között
unsigned int random_int(void);

// Lebegőpontos random szám 0.0..1.0 között
float random_float(void);

// Egész random szám adott intervallumban (zárt intervallum)
int random_int_range(int min, int max);

// Lebegőpontos random szám adott intervallumban
float random_float_range(float min, float max);

#endif // RANDOM_GENERATOR_H
