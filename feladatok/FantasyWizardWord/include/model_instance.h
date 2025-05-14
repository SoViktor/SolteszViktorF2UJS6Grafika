#ifndef MODEL_INSTANCE_H
#define MODEL_INSTANCE_H

#include "model.h"
#include "vector.h" // vec3

#ifdef __cplusplus
extern "C" {
#endif

// Egyetlen modellpéldány (model + transformációk)
typedef struct {
    Model* model;
    vec3 position;
    float scale;
    float rotation_deg; // Y tengely körüli forgatás
} ModelInstance;

/**
 * Hozzáad egy új példányt a jelenethez.
 */
void model_instance_add(Model* model, vec3 position, float scale, float rotation_deg);

/**
 * Minden példány kirajzolása a világban.
 */
void model_instance_render_all(const float view[16], const float proj[16]);

/**
 * Törli az összes példányt (pl. új világ generálásnál).
 */
void model_instance_clear_all(void);

#ifdef __cplusplus
}
#endif

#endif // MODEL_INSTANCE_H
