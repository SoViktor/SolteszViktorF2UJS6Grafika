#include "light.h"

void light_init(Light* light, LightType type, vec3 position, vec3 direction, vec3 color, float intensity) {
    light->type = type;
    light->position = position;
    light->direction = direction;
    light->color = color;
    light->intensity = intensity;
    light->enabled = true;
}

void light_enable(Light* light, bool enable) {
    light->enabled = enable;
}

void light_move_to(Light* light, vec3 new_position) {
    light->position = new_position;
}

void light_set_color(Light* light, vec3 new_color) {
    light->color = new_color;
}

void light_set_intensity(Light* light, float intensity) {
    light->intensity = intensity;
}
