#include "texturemap_reader.h"
#include "ext/stb_image.h"
#include <stdlib.h>
#include <stdio.h>

static TileType* global_tilemap = NULL;
static int tilemap_width = 0, tilemap_height = 0;

static TileType rgb_to_tile_type(unsigned char r, unsigned char g, unsigned char b) {
    if (r == 0   && g == 255 && b == 0)   return TILE_GRASS;
    if (r == 70  && g == 70  && b == 70)  return TILE_STONE;
    if (r == 255 && g == 0   && b == 0)   return TILE_LAVA;
    if (r == 230 && g == 210 && b == 80)  return TILE_SAND;
    if (r == 255 && g == 255 && b == 255) return TILE_SNOW;
    if (r == 150 && g == 150 && b == 150) return TILE_ROAD;
    return TILE_UNKNOWN;
}

TileType* read_texturemap(const char* filename, int expected_width, int expected_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 3);
    if (!data) {
        printf("[ERROR] Could not load texturemap: %s\n", filename);
        return NULL;
    }

    if (width != expected_width || height != expected_height) {
        printf("[ERROR] Texturemap size mismatch (%d×%d expected %d×%d)\n",
               width, height, expected_width, expected_height);
        stbi_image_free(data);
        return NULL;
    }

    TileType* result = (TileType*)malloc(width * height * sizeof(TileType));
    if (!result) {
        stbi_image_free(data);
        return NULL;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = (y * width + x) * 3;
            unsigned char r = data[index];
            unsigned char g = data[index + 1];
            unsigned char b = data[index + 2];
            result[y * width + x] = rgb_to_tile_type(r, g, b);
        }
    }

    global_tilemap = result;
    tilemap_width = width;
    tilemap_height = height;

    stbi_image_free(data);
    return result;
}

TileType get_terrain(int x, int z) {
    if (!global_tilemap || x < 0 || z < 0 || x >= tilemap_width || z >= tilemap_height) return TILE_UNKNOWN;
    return global_tilemap[z * tilemap_width + x];
}