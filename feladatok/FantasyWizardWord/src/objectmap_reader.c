#include "objectmap_reader.h"
#include "ext/stb_image.h"
#include <stdlib.h>
#include <stdio.h>

static BiomeCode rgb_to_object_code(unsigned char r, unsigned char g, unsigned char b) {
    if (r == 0   && g == 100  && b == 0)   return BIOME_PLAINS;
    if (r == 0   && g == 50 && b == 0)   return BIOME_FOREST;
    if (r == 150 && g == 0 && b == 50)  return BIOME_MAGIC_FOREST;
    if (r == 255 && g == 100   && b == 0)  return BIOME_ROCKY_TERRAIN;
    return BIOME_UNKNOWN;
}

BiomeCode* read_objectmap(const char* filename, int expected_width, int expected_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 3);
    printf("[DEBUG] Trying to load objectmap: %s\n", filename);

    if (!data) {
        printf("[ERROR] Could not load objectmap: %s\n", filename);
        return NULL;
    }

    if (width != expected_width || height != expected_height) {
        printf("[ERROR] Objectmap size mismatch (%d×%d expected %d×%d)\n",
               width, height, expected_width, expected_height);
        stbi_image_free(data);
        return NULL;
    }

    BiomeCode* result = (BiomeCode*)malloc(width * height * sizeof(BiomeCode));
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
            result[y * width + x] = rgb_to_object_code(r, g, b);
        }
    }

    stbi_image_free(data);
    return result;
}
