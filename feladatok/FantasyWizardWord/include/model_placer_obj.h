#ifndef MODEL_PLACER_OBJ_H
#define MODEL_PLACER_OBJ_H

#include "biome_codes.h"
#include "tile_types.h"

// Inicializálás: modellek betöltése (fa, bokor, kő, kristály)
void model_placer_obj_init(void);

// Törli az eddigi példányokat, ha újra generáljuk a világot
void model_placer_obj_clear(void);

// Objektumok elhelyezése: beolvasott objectmap + heightmap alapján
void model_placer_obj_place(const BiomeCode* object_data, const TileType* tile_data, const float* height_data, int width, int height);



#endif
