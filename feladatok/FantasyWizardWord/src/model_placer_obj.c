#include "model_placer_obj.h"
#include "model_registry.h"
#include "model_instance.h"
#include "multi_model_instance.h"
#include "vector.h"
#include "random_generator.h"
#include "texturemap_reader.h"
#include <stdio.h>

#define ROCK_MODEL_COUNT 3
#define ROCK_TEXTURE_COUNT 7

#define BOULDER_MODEL_COUNT 5
#define BOULDER_TEXTURE_COUNT 7

#define TREE_MODEL_COUNT 5

#define CRYSTAL_MODEL_COUNT 11
#define CRYSTAL_COLOR_COUNT 7

#define BUSH_TEXTURE_COUNT 3

#define DECORATION_SIMPLE_COUNT 8
#define DECORATION_MULTI_COUNT 4

#define MODEL_BASE "assets/models/InteractiveStatics/"
#define TEXTURE_BASE "assets/textures/Environment"
#define DECORATION_MODEL_BASE "assets/models/Decorations/"


static Model* rock_models[ROCK_MODEL_COUNT][ROCK_TEXTURE_COUNT];
static Model* boulder_models[BOULDER_MODEL_COUNT][BOULDER_TEXTURE_COUNT];
static MultiModel* dead_tree_models[TREE_MODEL_COUNT];
static MultiModel* pine_models[TREE_MODEL_COUNT];
static MultiModel* tree_models[TREE_MODEL_COUNT];
static MultiModel* twisted_tree_models[TREE_MODEL_COUNT];
static MultiModel* decoration_models_multi[DECORATION_MULTI_COUNT];
static Model* crystal_models[CRYSTAL_MODEL_COUNT][CRYSTAL_COLOR_COUNT];
static Model* bush_models[BUSH_TEXTURE_COUNT];
static Model* decoration_models[DECORATION_SIMPLE_COUNT];
static MultiModel* decoration_models_multi[DECORATION_MULTI_COUNT];

static const float crystal_colors[CRYSTAL_COLOR_COUNT][3] = {
    {1.0f, 0.0f, 0.0f}, // piros
    {1.0f, 1.0f, 0.0f}, // sárga
    {0.0f, 1.0f, 0.0f}, // zöld
    {0.0f, 0.6f, 1.0f}, // kék
    {0.6f, 0.0f, 1.0f}, // lila
    {0.1f, 0.1f, 0.1f}, // fekete (szürkés)
    {1.0f, 1.0f, 1.0f}  // fehér
};

static const char* bush_textures[BUSH_TEXTURE_COUNT] = {
    TEXTURE_BASE "/Leaves_NormalTree_C.png",
    TEXTURE_BASE "/Leaves_TwistedTree_C.png",
    TEXTURE_BASE "/Leaf_Pine_C.png"
};

static const char* decoration_simple_names[DECORATION_SIMPLE_COUNT] = {
    "Clover_Many",
    "Clover",
    "Fern",
    "Grass_Wispy_1",
    "Grass_Wispy_2",
    "Grass",
    "Plant_Big",
    "Tall_Grass"
};
static const char* decoration_simple_textures[DECORATION_SIMPLE_COUNT] = {
    TEXTURE_BASE "/Leaves.png",           // Clover_Many
    TEXTURE_BASE "/Leaves.png",           // Clover
    TEXTURE_BASE "/Leaves.png",           // Fern
    TEXTURE_BASE "/Grass.png",            // Grass_Wispy_1
    TEXTURE_BASE "/Grass.png",            // Grass_Wispy_2
    TEXTURE_BASE "/Grass.png",            // Grass
    TEXTURE_BASE "/Leaves.png",           // Plant_Big
    TEXTURE_BASE "/Leaves.png"            // Tall_Grass
};

static const char* decoration_multi_names[DECORATION_MULTI_COUNT] = {
    "Flower_Group_Pink",
    "Flower_Group_Yellow",
    "Flower_Single_Pink",
    "Flower_Single_Yellow"

};


void load_tree_group(MultiModel** target_array, const char* model_prefix, const char* name) {
    for (int i = 0; i < TREE_MODEL_COUNT; ++i) {
        char obj_path[256];
        snprintf(obj_path, sizeof(obj_path), MODEL_BASE "Trees/%s%d.obj", model_prefix, i + 1);

        ModelParams tree = {
            .name = name,
            .model_path = obj_path,
            .texture_path = TEXTURE_BASE,
            .format = MODEL_FORMAT_OBJ,
            .visual = MODEL_TYPE_MULTI_TEXTURED,
            .use_lighting = true,
            .alpha = 1.0f
        };
 
        target_array[i] = model_registry_load_multi(&tree);
    }
}


void model_placer_obj_init(){
    for (int i = 0; i < ROCK_MODEL_COUNT; ++i) {
        for (int j = 0; j < ROCK_TEXTURE_COUNT; ++j) {
            char obj_path[256];
            char tex_path[256];

            snprintf(obj_path, sizeof(obj_path), MODEL_BASE "Rocks/Rock%d.obj", i + 1);
            snprintf(tex_path, sizeof(tex_path), TEXTURE_BASE "/Rocks_Diffuse%d.png", j + 1);

            ModelParams rock = {
                .name = "Rock",
                .model_path = obj_path,
                .texture_path = tex_path,
                .format = MODEL_FORMAT_OBJ,
                .visual = MODEL_TYPE_TEXTURED,
                .use_lighting = true,
                .alpha = 1.0f
            };

            rock_models[i][j] = model_registry_load(&rock);
        }
    }
    for (int i = 0; i < BOULDER_MODEL_COUNT; ++i) {
        for (int j = 0; j < BOULDER_TEXTURE_COUNT; ++j) {
            char obj_path[256];
            char tex_path[256];

            snprintf(obj_path, sizeof(obj_path), MODEL_BASE "Rocks/Boulder%d.obj", i + 1);
            snprintf(tex_path, sizeof(tex_path), TEXTURE_BASE "/PathRocks_Diffuse%d.png", j + 1);

            ModelParams boulder = {
                .name = "Boulder",
                .model_path = obj_path,
                .texture_path = tex_path,
                .format = MODEL_FORMAT_OBJ,
                .visual = MODEL_TYPE_TEXTURED,
                .use_lighting = true,
                .alpha = 1.0f
            };

            boulder_models[i][j] = model_registry_load(&boulder);
        }
    }
    load_tree_group(dead_tree_models, "Dead_Tree", "Dead_Tree");
    load_tree_group(pine_models, "Pine", "Pine");
    load_tree_group(tree_models, "Tree", "Tree");
    load_tree_group(twisted_tree_models, "Twisted_Tree", "Twisted_Tree");
    
    for (int i = 0; i < BUSH_TEXTURE_COUNT; ++i) {
        ModelParams bush = {
            .name = "Bush",
            .model_path = MODEL_BASE "Bushes/Bush.obj",
            .texture_path = bush_textures[i],
            .format = MODEL_FORMAT_OBJ,
            .visual = MODEL_TYPE_TEXTURED,
            .use_lighting = true,
            .alpha = 1.0f
        };
    
        bush_models[i] = model_registry_load(&bush);
    }
    

    for (int i = 0; i < CRYSTAL_MODEL_COUNT; ++i) {
        for (int j = 0; j < CRYSTAL_COLOR_COUNT; ++j) {
            char obj_path[256];
            snprintf(obj_path, sizeof(obj_path), MODEL_BASE "Crystals/Crystal%d.obj", i + 1);
    
            ModelParams crystal = {
                .name = "Crystal",
                .model_path = obj_path,
                .format = MODEL_FORMAT_OBJ,
                .visual = MODEL_TYPE_COLORED,
                .color = {crystal_colors[j][0], crystal_colors[j][1], crystal_colors[j][2]},
                .alpha = 0.6f,
                .use_lighting = true
            };
    
            crystal_models[i][j] = model_registry_load(&crystal);
        }
    }

    for (int i = 0; i < DECORATION_SIMPLE_COUNT; ++i) {
        char obj_path[256];
        snprintf(obj_path, sizeof(obj_path), DECORATION_MODEL_BASE "%s.obj", decoration_simple_names[i]);
    
        ModelParams params = {
            .name = decoration_simple_names[i],
            .model_path = obj_path,
            .texture_path = decoration_simple_textures[i],
            .format = MODEL_FORMAT_OBJ,
            .visual = MODEL_TYPE_TEXTURED,
            .use_lighting = true,
            .alpha = 1.0f
        };
    
        decoration_models[i] = model_registry_load(&params);
    }

    for (int i = 0; i < DECORATION_MULTI_COUNT; ++i) {
        char obj_path[256];
        snprintf(obj_path, sizeof(obj_path), DECORATION_MODEL_BASE "%s.obj", decoration_multi_names[i]);
    
        ModelParams params = {
            .name = decoration_multi_names[i],
            .model_path = obj_path,
            .texture_path = TEXTURE_BASE,
            .format = MODEL_FORMAT_OBJ,
            .visual = MODEL_TYPE_MULTI_TEXTURED,
            .use_lighting = true,
            .alpha = 1.0f
        };
    
        decoration_models_multi[i] = model_registry_load_multi(&params);
    }
    
}

static Model* model_pick_rock_by_texture(int texture_index) {
    if (texture_index < 0 || texture_index >= ROCK_TEXTURE_COUNT) return NULL;
    int model_index = random_int_range(0, ROCK_MODEL_COUNT - 1);
    return rock_models[model_index][texture_index];
}

static Model* model_pick_boulder_by_texture(int texture_index) {
    if (texture_index < 0 || texture_index >= BOULDER_TEXTURE_COUNT) return NULL;
    int model_index = random_int_range(0, BOULDER_MODEL_COUNT - 1);
    return boulder_models[model_index][texture_index];
}

static Model* model_pick_crystal_by_color(int color_index) {
    if (color_index < 0 || color_index >= CRYSTAL_COLOR_COUNT) return NULL;
    int model_index = random_int_range(0, CRYSTAL_MODEL_COUNT - 1);
    return crystal_models[model_index][color_index];
}

static MultiModel* model_pick_random_dead_tree(void) {
    int i = random_int_range(0, TREE_MODEL_COUNT - 1);
    return dead_tree_models[i];
}

static MultiModel* model_pick_random_pine(void) {
    int i = random_int_range(0, TREE_MODEL_COUNT - 1);
    return pine_models[i];
}

static MultiModel* model_pick_random_tree(void) {
    int i = random_int_range(0, TREE_MODEL_COUNT - 1);
    return tree_models[i];
}

static MultiModel* model_pick_random_twisted_tree(void) {
    int i = random_int_range(0, TREE_MODEL_COUNT - 1);
    return twisted_tree_models[i];
}


static Model* model_pick_random_decoration_simple(void) {
    int i = random_int_range(0, DECORATION_SIMPLE_COUNT - 1);
    return decoration_models[i];
}


static MultiModel* model_pick_random_decoration_multi(void) {
    int i = random_int_range(0, DECORATION_MULTI_COUNT - 1);
    return decoration_models_multi[i];
}

static Model* model_pick_random_bush(void) {
    int i = random_int_range(0, BUSH_TEXTURE_COUNT - 1);
    return bush_models[i];
}


void model_placer_obj_clear(void) {
    model_instance_clear_all();
    multi_model_instance_clear_all();
}

static Model* model_pick_bush_by_texture(int texture_index) {
    if (texture_index < 0 || texture_index >= BUSH_TEXTURE_COUNT) return NULL;
    return bush_models[texture_index];
}
void model_placer_obj_place(const BiomeCode* object_data, const TileType* tile_data, const float* height_data, int width, int height) {

    for (int z = 0; z < height; ++z) {
        for (int x = 0; x < width; ++x) {
            int index = z * width + x;
            BiomeCode biome = object_data[index];
            TileType tile = tile_data[index];
            float y = height_data[index];
            vec3 pos = vec3_create(x - width / 2.0f, y, z - height / 2.0f);

            // Alapesély a modellre
            float base_chance = 0.01f; // 1%
            if (tile == TILE_LAVA || tile == TILE_SAND) base_chance = 0.001f;
            else if (tile == TILE_SNOW) base_chance = 0.005f;

            if (random_float() > base_chance) continue;

            float r = random_float();
            float rot =1.0f;
            float scale = 1.0f;

            switch (biome) 
            {
                case BIOME_PLAINS:
                    switch (tile)
                    {   
                        case TILE_GRASS:
                        if (r < 0.05f) {
                            Model* model=model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                model_instance_add((model), pos, scale, rot);
                            }  
                        } else if (r < 0.30f) {
                            MultiModel* multi = model_pick_random_decoration_multi();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.55f) {
                            Model* model = model_pick_random_decoration_simple();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.60f) {
                            MultiModel* multi = model_pick_random_tree();
                            if (multi)
                            {
                               multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.85f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.90f) {
                            Model* model =  model_pick_random_bush();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            } 
                        } else if (r < 0.95f) { 
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                               model_instance_add(model, pos, scale, rot);
                            }   
                        } else {
                            Model* model =model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                 model_instance_add(model, pos, scale, rot);
                            }
                        }
                        break;
                    case TILE_STONE:
                        if (r < 0.20f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                 model_instance_add(model, pos, scale, rot);
                            }   
                        } else if (r < 0.30f) {
                            MultiModel* multi = model_pick_random_decoration_multi();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }  
                        } else if (r < 0.40f) {
                            Model* model = model_pick_random_decoration_simple();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else if (r < 0.50f) {
                            MultiModel* multi = model_pick_random_pine();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.85f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
    
                        } else {
                            Model* model =model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        }
                        break;
                    case TILE_LAVA:
                        if (r < 0.10f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                               model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.55f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        }
                        break;
                    case TILE_SAND:
                        if (r < 0.20f) {
                            Model* model =model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_dead_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.80f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            } 
                        } else if (r < 0.90f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        }
                        break;
                    case TILE_SNOW:
                        if (r < 0.10f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else if (r < 0.50f) {
                            MultiModel* multi = model_pick_random_pine();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                                
                        } else if (r < 0.85f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                            
                            
                        }
                        break;
                    default:
                        break;
                    }
                break; 
                
                case BIOME_FOREST:
                    switch (tile)
                    {   
                        case TILE_GRASS:
                        if (r < 0.05f) {
                            Model* model=model_pick_random_bush();
                            if (model)
                            {
                                model_instance_add((model), pos, scale, rot);
                            }  
                        } else if (r < 0.30f) {
                            MultiModel* multi = model_pick_random_decoration_multi();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.55f) {
                            Model* model = model_pick_random_decoration_simple();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.80f) {
                            MultiModel* multi = model_pick_random_tree();
                            if (multi)
                            {
                               multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.85f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.90f) {
                            Model* model =  model_pick_random_bush();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            } 
                        } else if (r < 0.95f) { 
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                               model_instance_add(model, pos, scale, rot);
                            }   
                        } else {
                            Model* model =model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                 model_instance_add(model, pos, scale, rot);
                            }
                        }
                        break;
                    case TILE_STONE:
                        if (r < 0.05f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                 model_instance_add(model, pos, scale, rot);
                            }
                            } 
                        else if (r < 0.20f) {
                            Model* model = model_pick_random_bush();
                            if (model)
                            {
                                 model_instance_add(model, pos, scale, rot);
                            }   
                        } else if (r < 0.30f) {
                            MultiModel* multi = model_pick_random_decoration_multi();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }  
                        } else if (r < 0.40f) {
                            Model* model = model_pick_random_decoration_simple();
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else if (r < 0.60f) {
                            MultiModel* multi = model_pick_random_pine();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.85f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
    
                        } else {
                            Model* model =model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        }
                        break;
                    case TILE_LAVA:
                        if (r < 0.10f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                               model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.55f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        }
                        break;
                    case TILE_SAND:
                        if (r < 0.05f) {
                            Model* model =model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_dead_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                        } else if (r < 0.75f) {
                            MultiModel* multi = model_pick_random_twisted_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            } 
                        } else if (r < 0.90f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        }
                        break;
                    case TILE_SNOW:
                        if (r < 0.05f) {
                            Model* model = model_pick_crystal_by_color(random_int_range(0, CRYSTAL_COLOR_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else if (r < 0.50f) {
                            MultiModel* multi = model_pick_random_pine();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }    
                        } else if (r < 0.70f) {
                            MultiModel* multi = model_pick_random_dead_tree();
                            if (multi)
                            {
                                multi_model_instance_add(multi, pos, scale, rot);
                            }
                                
                        } else if (r < 0.85f) {
                            Model* model = model_pick_rock_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }    
                        } else {
                            Model* model = model_pick_boulder_by_texture(random_int_range(0, ROCK_TEXTURE_COUNT -1));
                            if (model)
                            {
                                model_instance_add(model, pos, scale, rot);
                            }
                            
                            
                        }
                        break;

                    default:
                        break;
                    }
                break; 
                default:
                    break;
            }
            
        }
    }
}


