#ifndef OBJECTMAP_READER_H
#define OBJECTMAP_READER_H

#include "biome_codes.h"

#ifdef __cplusplus
extern "C" {
#endif

// Beolvassa a models_obj.png-t es visszaad egy ObjectCode tombot
// A tomb merete: width * height, row-major sorrendben (y * width + x)
BiomeCode* read_objectmap(const char* filename, int expected_width, int expected_height);

#ifdef __cplusplus
}
#endif

#endif // OBJECTMAP_READER_H
