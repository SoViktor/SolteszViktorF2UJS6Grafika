#ifndef MULTI_MODEL_INSTANCE_H
#define MULTI_MODEL_INSTANCE_H

#include "model.h"
#include "vector.h"

#ifdef __cplusplus
extern "C" {
#endif

void multi_model_instance_add(MultiModel* model, vec3 position, float scale, float rotation_deg);
void multi_model_instance_render_all(const float view[16], const float proj[16]);
void multi_model_instance_clear_all(void);

#ifdef __cplusplus
}
#endif

#endif // MULTI_MODEL_INSTANCE_H
