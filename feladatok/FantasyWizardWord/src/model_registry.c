#include "model_registry.h"
#include "model_loader_obj_textured.h"
#include "model_loader_obj_colored.h"
#include "model_loader_obj_multi.h"


#include <stdio.h>

Model* model_registry_load(const ModelParams* params) {
    if (!params || !params->model_path) {
        printf("[MODEL REGISTRY] Invalid model parameters.\n");
        return NULL;
    }

    switch (params->format) {
        case MODEL_FORMAT_OBJ:
            switch (params->visual) {
                case MODEL_TYPE_TEXTURED:
                    if (!params->texture_path) {
                        printf("[MODEL REGISTRY] Texture path missing for textured model: %s\n", params->name);
                        return NULL;
                    }
                    return load_obj_textured(params->model_path, NULL, params->texture_path);

                case MODEL_TYPE_COLORED:
                    return load_obj_colored(params->model_path, params->color, params->alpha, params->use_lighting);

                default:
                    printf("[MODEL REGISTRY] Unknown OBJ visual type for: %s\n", params->name);
                    return NULL;
            }

        case MODEL_FORMAT_GLTF:
            printf("[MODEL REGISTRY] GLTF support not yet implemented: %s\n", params->name);
            return NULL;

        default:
            printf("[MODEL REGISTRY] Unknown model format for: %s\n", params->name);
            return NULL;
    }
}

MultiModel* model_registry_load_multi(const ModelParams* params) {
    if (!params || !params->model_path || !params->texture_path) {
        printf("[MODEL REGISTRY] Invalid MULTI model parameters: %s\n", params ? params->name : "NULL");
        return NULL;
    }

    if (params->format != MODEL_FORMAT_OBJ) {
        printf("[MODEL REGISTRY] MULTI model only supports OBJ for now: %s\n", params->name);
        return NULL;
    }

    if (params->visual != MODEL_TYPE_MULTI_TEXTURED) {
        printf("[MODEL REGISTRY] Incorrect visual type for MULTI model: %s\n", params->name);
        return NULL;
    }

    return load_obj_multi(params->model_path, params->texture_path);
}