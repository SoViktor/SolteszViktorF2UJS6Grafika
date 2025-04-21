#ifndef LIGHT_H
#define LIGHT_H

#include "vector.h"
#include <stdbool.h>

typedef enum {
    LIGHT_POINT,
    LIGHT_DIRECTIONAL,
    LIGHT_SPOT
} LightType;

typedef struct {
    LightType type;
    vec3 position;
    vec3 direction;
    vec3 color;
    float intensity;
    bool enabled;
} Light;

#define MAX_LIGHTS 8

void light_init(Light* light, LightType type, vec3 position, vec3 direction, vec3 color, float intensity);
void light_enable(Light* light, bool enable);
void light_move_to(Light* light, vec3 new_position);
void light_set_color(Light* light, vec3 new_color);
void light_set_intensity(Light* light, float intensity);

#endif // LIGHT_H
