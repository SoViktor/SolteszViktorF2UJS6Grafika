#ifndef MODEL_REGISTRY_H
#define MODEL_REGISTRY_H

#include "model.h"
#include "vector.h"
#include <stdbool.h>

typedef enum {
    MODEL_WIZARD,
    MODEL_CRYSTAL1,
    MODEL_CRYSTAL2,
    MODEL_CRYSTAL3,
    MODEL_CRYSTAL4,
    MODEL_CRYSTAL5,
    MODEL_CRYSTAL6,
    MODEL_CRYSTAL7,
    MODEL_CRYSTAL8,
    MODEL_CRYSTAL9,
    MODEL_CRYSTAL10,
    MODEL_CRYSTAL11,
    MODEL_CRYSTAL12,
    MODEL_COUNT
} ModelType;


typedef struct {
    ModelType type;
    vec3 position;
    float scale;
    unsigned int texture_id;
} ModelInstance;


bool model_registry_init(void);
void model_registry_shutdown(void);

Model* model_registry_get(ModelType type);

#endif
