#include "camera.h"
#include <math.h>

#define DEG2RAD (3.14159f / 180.0f)

static vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

static vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_create(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

static vec3 vec3_normalize(vec3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0.0f) return vec3_create(0.0f, 0.0f, 0.0f);
    return vec3_create(v.x / len, v.y / len, v.z / len);
}

static float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

static void look_at(vec3 eye, vec3 center, vec3 up, float matrix[16]) {
    vec3 f = vec3_normalize(vec3_sub(center, eye));
    vec3 s = vec3_normalize(vec3_cross(f, up));
    vec3 u = vec3_cross(s, f);

    matrix[0] = s.x;  matrix[1] = u.x;  matrix[2] = -f.x;  matrix[3] = 0.0f;
    matrix[4] = s.y;  matrix[5] = u.y;  matrix[6] = -f.y;  matrix[7] = 0.0f;
    matrix[8] = s.z;  matrix[9] = u.z;  matrix[10] = -f.z; matrix[11] = 0.0f;
    matrix[12] = -vec3_dot(s, eye);
    matrix[13] = -vec3_dot(u, eye);
    matrix[14] = vec3_dot(f, eye);
    matrix[15] = 1.0f;
}

void camera_init(Camera* cam, vec3 position, vec3 target, vec3 up) {
    cam->up = up;
    camera_reset(cam, target);
}

void camera_update(Camera* cam, vec3 player_position) {
    float x = cam->distance * cosf(cam->elevation) * sinf(cam->azimuth);
    float y = cam->distance * sinf(cam->elevation);
    float z = cam->distance * cosf(cam->elevation) * cosf(cam->azimuth);

    cam->position = vec3_create(player_position.x + x, player_position.y + y, player_position.z + z);
    cam->target = player_position;
}

void camera_get_view_matrix(Camera* cam, float view_matrix[16]) {
    look_at(cam->position, cam->target, cam->up, view_matrix);
}

void camera_orbit(Camera* cam, float delta_azimuth, float delta_elevation) {
    cam->azimuth += delta_azimuth;
    cam->elevation += delta_elevation;

    float limit = DEG2RAD * 89.0f;
    if (cam->elevation > limit) cam->elevation = limit;
    if (cam->elevation < -limit) cam->elevation = -limit;
}

void camera_zoom(Camera* cam, float delta) {
    cam->distance += delta;
    if (cam->distance < 1.0f) cam->distance = 1.0f;
    if (cam->distance > 20.0f) cam->distance = 20.0f;
}

void camera_reset(Camera* cam, vec3 player_position) {
    cam->distance = 5.0f;
    cam->azimuth = DEG2RAD * 180.0f;
    cam->elevation = DEG2RAD * 15.0f;
    camera_update(cam, player_position);
}

vec3 camera_get_position(Camera* cam) {
    return cam->position;
}