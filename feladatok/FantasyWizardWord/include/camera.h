#ifndef CAMERA_H
#define CAMERA_H

#include "vector.h"


typedef struct {
    vec3 position;
    vec3 target;
    vec3 up;

    float distance;
    float azimuth;
    float elevation;
} Camera;

void camera_init(Camera* cam, vec3 position, vec3 target, vec3 up);
void camera_update(Camera* cam, vec3 player_position);
void camera_get_view_matrix(Camera* cam, float view_matrix[16]);

void camera_orbit(Camera* cam, float delta_azimuth, float delta_elevation);
void camera_zoom(Camera* cam, float delta);
void camera_reset(Camera* cam, vec3 player_position);

vec3 camera_get_position(Camera* cam);


#endif // CAMERA_H
