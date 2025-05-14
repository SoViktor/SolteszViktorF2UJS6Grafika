#pragma once
#include "tile_types.h"

TileType* read_texturemap(const char* filename, int expected_width, int expected_height);

// Új lekérdezők
TileType get_terrain(int x, int z);
