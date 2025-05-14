#include "terrain_mesh_generator.h"
#include "heightmap_reader.h"
#include "texturemap_reader.h"
#include "tile_types.h"
#include "renderer.h"
#include <stdio.h>
#include <stdlib.h>

#define TILE_ATLAS_COLS 3
#define TILE_ATLAS_ROWS 2

void generate_terrain_mesh(const char* heightmap_path, const char* texturemap_path) {
    printf("[TERRAIN] Loading heightmap: %s\n", heightmap_path);
    int width, height;
    float* height_data = read_heightmap(heightmap_path, &width, &height);
    if (!height_data) {
        printf("[ERROR] Failed to load heightmap.\n");
        return;
    }

    printf("[TERRAIN] Loading texturemap: %s\n", texturemap_path);
    TileType* tile_data = read_texturemap(texturemap_path, width, height);
    if (!tile_data) {
        printf("[ERROR] Failed to load texturemap.\n");
        free(height_data);
        return;
    }

    int vertex_count = width * height;
    int quad_count = (width - 1) * (height - 1);
    int index_count = quad_count * 6;

    float* vertices = (float*)malloc(sizeof(float) * vertex_count * 5); // 3 pos + 2 uv
    unsigned int* indices = (unsigned int*)malloc(sizeof(unsigned int) * index_count);

    if (!vertices || !indices) {
        printf("[ERROR] Memory allocation failed for terrain mesh.\n");
        free(height_data);
        free(tile_data);
        if (vertices) free(vertices);
        if (indices) free(indices);
        return;
    }

    float x_offset = width / 2.0f;
    float z_offset = height / 2.0f;

    float tile_w = 1.0f / TILE_ATLAS_COLS;
    float tile_h = 1.0f / TILE_ATLAS_ROWS;

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            int i = z * width + x;
            float y = height_data[i];
            TileType tile = tile_data[i];

            float atlas_u = (tile % TILE_ATLAS_COLS) * tile_w;
            float atlas_v = (tile / TILE_ATLAS_COLS) * tile_h;

            float local_u = (x % 4) / 4.0f;
            float local_v = (z % 4) / 4.0f;

            float u = atlas_u + local_u * tile_w;
            float v = atlas_v + local_v * tile_h;

            vertices[i * 5 + 0] = (float)x - x_offset; // X középre igazítva
            vertices[i * 5 + 1] = y;                  // Y (magasság)
            vertices[i * 5 + 2] = (float)z - z_offset; // Z középre igazítva
            vertices[i * 5 + 3] = u;                  // U
            vertices[i * 5 + 4] = v;                  // V
        }
    }

    // Index generálás
    int idx = 0;
    for (int z = 0; z < height - 1; ++z) {
        for (int x = 0; x < width - 1; ++x) {
            int i0 = z * width + x;
            int i1 = z * width + (x + 1);
            int i2 = (z + 1) * width + (x + 1);
            int i3 = (z + 1) * width + x;

            indices[idx++] = i0;
            indices[idx++] = i1;
            indices[idx++] = i2;

            indices[idx++] = i0;
            indices[idx++] = i2;
            indices[idx++] = i3;
        }
    }

    renderer_upload_mesh(vertices, vertex_count * 5, indices, index_count);

    printf("[TERRAIN] Mesh uploaded: %d vertices, %d indices\n", vertex_count, index_count);

    free(vertices);
    free(indices);
    free(height_data);
    free(tile_data);
}
