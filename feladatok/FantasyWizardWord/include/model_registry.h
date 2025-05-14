#ifndef MODEL_REGISTRY_H
#define MODEL_REGISTRY_H

#include "model.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MODEL_FORMAT_OBJ,
    MODEL_FORMAT_GLTF
} ModelFormat;

typedef enum {
    MODEL_TYPE_TEXTURED,
    MODEL_TYPE_COLORED,
    MODEL_TYPE_MULTI_TEXTURED 
} ModelVisualType;

typedef struct {
    const char* name;
    const char* model_path;
    const char* texture_path;
    float color[3];
    float alpha;
    bool use_lighting;
    ModelFormat format;
    ModelVisualType visual;
} ModelParams;

Model* model_registry_load(const ModelParams* params);
MultiModel* model_registry_load_multi(const ModelParams* params);

#ifdef __cplusplus
}
#endif

#endif // MODEL_REGISTRY_H
