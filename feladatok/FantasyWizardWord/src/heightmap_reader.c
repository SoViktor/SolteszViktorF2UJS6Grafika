#include "ext/stb_image.h"
#include "heightmap_reader.h"
#include <stdlib.h>
#include <stdio.h>

#define HEIGHTMAP_SCALE 50.0f

static float* global_heightmap = NULL;
static int map_width = 0, map_height = 0;

float* read_heightmap(const char* filename, int* out_width, int* out_height) {
    int width, height, channels;
    unsigned char* data = stbi_load(filename, &width, &height, &channels, 1);
    if (!data) {
        printf("[ERROR] Could not load image: %s\n", filename);
        return NULL;
    }

    float* heights = (float*)malloc(sizeof(float) * width * height);
    if (!heights) {
        stbi_image_free(data);
        return NULL;
    }

    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            int index = y * width + x;
            heights[index] = data[index] / 255.0f * HEIGHTMAP_SCALE; // 10.0f = max magasság
        }
    }

    global_heightmap = heights;
    map_width = width;
    map_height = height;

    stbi_image_free(data);
    *out_width = width;
    *out_height = height;
    return heights;
}

float get_height_at(float x, float z) {
    if (!global_heightmap) return 0.0f;

    // A heightmap középre van igazítva, így az x,z 0-nál indulnak a képernyő közepén
    float half_width = map_width / 2.0f;
    float half_height = map_height / 2.0f;

    float fx = x + half_width;
    float fz = z + half_height;

    int ix = (int)fx;
    int iz = (int)fz;

    if (ix < 0 || iz < 0 || ix >= map_width - 1 || iz >= map_height - 1)
        return 0.0f;

    float tx = fx - ix;
    float tz = fz - iz;

    float h00 = global_heightmap[(iz    ) * map_width + (ix    )];
    float h10 = global_heightmap[(iz    ) * map_width + (ix + 1)];
    float h01 = global_heightmap[(iz + 1) * map_width + (ix    )];
    float h11 = global_heightmap[(iz + 1) * map_width + (ix + 1)];

    float h0 = h00 * (1 - tx) + h10 * tx;
    float h1 = h01 * (1 - tx) + h11 * tx;
    float h  = h0 * (1 - tz) + h1 * tz;

    return h;
}

