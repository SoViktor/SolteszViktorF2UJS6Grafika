#include "vector.h"
#include <math.h>

vec3 vec3_create(float x, float y, float z) {
    vec3 v = { x, y, z };
    return v;
}

vec2 vec2_create(float x, float y) {
    vec2 v = { x, y };
    return v;
}

vec3 vec3_add(vec3 a, vec3 b) {
    return vec3_create(a.x + b.x, a.y + b.y, a.z + b.z);
}

vec3 vec3_sub(vec3 a, vec3 b) {
    return vec3_create(a.x - b.x, a.y - b.y, a.z - b.z);
}

vec3 vec3_scale(vec3 v, float s) {
    return vec3_create(v.x * s, v.y * s, v.z * s);
}

vec3 vec3_normalize(vec3 v) {
    float len = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
    if (len == 0.0f) return vec3_create(0.0f, 0.0f, 0.0f);
    return vec3_scale(v, 1.0f / len);
}

vec3 vec3_cross(vec3 a, vec3 b) {
    return vec3_create(
        a.y * b.z - a.z * b.y,
        a.z * b.x - a.x * b.z,
        a.x * b.y - a.y * b.x
    );
}

float vec3_dot(vec3 a, vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

float vec3_length(vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}