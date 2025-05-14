#include "world_generator.h"
#include "terrain_mesh_generator.h"
#include "random_generator.h" // <-- ezt hozzáadjuk
#include "model.h"
#include "vector.h"
#include "model_registry.h"
#include "model_instance.h"
#include "multi_model_instance.h"
#include "objectmap_reader.h"
#include "model_placer_obj.h"
#include "heightmap_reader.h"
#include "texturemap_reader.h"

#include <stdlib.h>
#include <stdio.h>

void world_set_seed(unsigned int seed) {
    random_set_seed(seed);
}

void world_generate(void) {
    printf("[WORLD] Generating world...\n");

    int width, height;
    float* height_data = read_heightmap("assets/maps/map1/landscape.png", &width, &height);
    if (!height_data) return;

    TileType* tile_data = read_texturemap("assets/maps/map1/terrain.png", width, height);
    if (!tile_data) {
        free(height_data);
        return;
    }

    BiomeCode* biome_data = read_objectmap("assets/maps/map1/model_obj.png", width, height);
    if (!biome_data) {
        free(height_data);
        free(tile_data);
        return;
    }

    generate_terrain_mesh("assets/maps/map1/landscape.png", "assets/maps/map1/terrain.png");

    model_placer_obj_init();
    model_placer_obj_clear();
    model_placer_obj_place(biome_data, tile_data, height_data, width, height);

    free(biome_data);
    free(height_data);
    free(tile_data);
}
