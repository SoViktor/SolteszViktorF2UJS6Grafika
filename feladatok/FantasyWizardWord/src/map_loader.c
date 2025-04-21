#include "map_loader.h"
#include "texture_registry.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

bool map_loader_load(const char* path, ModelInstance** out_instances, int* out_count) {
    FILE* file = fopen(path, "r");
    if (!file) {
        fprintf(stderr, "Failed to open map file: %s\n", path);
        return false;
    }

    int max_instances = 256;
    ModelInstance* list = malloc(sizeof(ModelInstance) * max_instances);
    int count = 0;

    int z = 0;
    char line[1024];
    while (fgets(line, sizeof(line), file)) {
        int x = 0;
        char* ptr = line;

        while (*ptr) {
            int type;
            char texname[128] = {0};

            if (sscanf(ptr, "%d:%127s", &type, texname) >= 1) {
                ModelType model_type;
                bool valid = true;

                switch (type) {
                    case 1: model_type = MODEL_WIZARD; break;
                    case 2: model_type = MODEL_CRYSTAL1; break;
                    case 3: model_type = MODEL_CRYSTAL2; break;
                    case 4: model_type = MODEL_CRYSTAL3; break;
                    case 5: model_type = MODEL_CRYSTAL4; break;
                    case 6: model_type = MODEL_CRYSTAL5; break;
                    case 7: model_type = MODEL_CRYSTAL6; break;
                    case 8: model_type = MODEL_CRYSTAL7; break;
                    case 9: model_type = MODEL_CRYSTAL8; break;
                    case 10: model_type = MODEL_CRYSTAL9; break;
                    case 11: model_type = MODEL_CRYSTAL10; break;
                    case 12: model_type = MODEL_CRYSTAL11; break;
                    case 13: model_type = MODEL_CRYSTAL12; break;

                    default:
                        valid = false;
                        break;
                }

                if (valid) {
                    if (count >= max_instances) {
                        max_instances *= 2;
                        list = realloc(list, sizeof(ModelInstance) * max_instances);
                    }

                    float scale = (model_type == MODEL_CRYSTAL1) ? 0.5f : 1.0f;

                    // --- Textúra útvonal összeállítása ---
                    char texture_path[256] = {0};

                    if (strlen(texname) > 0) {
                        const char* folder = NULL;
                        switch (type) {
                            case 2: case 3: case 4: case 5: case 6:
                            case 7: case 8: case 9: case 10:
                            case 11: case 12: case 13:
                                folder = "Crystals"; break;
                            case 1: folder = "Wizard"; break;
                        }

                        if (folder) {
                            snprintf(texture_path, sizeof(texture_path), "assets/textures/%s/%s", folder, texname);
                        }
                    }

                    GLuint texture_id = 0;
                    if (texture_path[0]) {
                        texture_id = texture_registry_get_or_load(texture_path);
                    }

                    list[count++] = (ModelInstance){
                        model_type,
                        { (float)x, 0.0f, (float)-z },
                        scale,
                        texture_id
                    };

                    printf("[MAP_LOADER] Added model %d at (%.2f, %.2f, %.2f), texture: %s\n",
                           model_type, (float)x, 0.0f, (float)-z, texture_path[0] ? texture_path : "(default)");
                }
            }

            while (*ptr && *ptr != ' ' && *ptr != '\t') ptr++;
            while (*ptr == ' ' || *ptr == '\t') ptr++;
            x++;
        }
        z++;
    }

    fclose(file);

    *out_instances = list;
    *out_count = count;

    return true;
}

void map_loader_free(ModelInstance* instances) {
    if (instances) {
        free(instances);
    }
}
