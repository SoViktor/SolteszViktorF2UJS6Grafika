#ifndef VECTOR_H
#define VECTOR_H

typedef struct {
    float x, y;
} vec2;

typedef struct {
    float x, y, z;
} vec3;

vec3 vec3_create(float x, float y, float z);
vec3 vec3_add(vec3 a, vec3 b);
vec3 vec3_sub(vec3 a, vec3 b);
vec3 vec3_scale(vec3 v, float s);
float vec3_length(vec3 v);
vec3 vec3_normalize(vec3 v);
float vec3_dot(vec3 a, vec3 b);
vec3 vec3_cross(vec3 a, vec3 b);
vec2 vec2_create(float x, float y);


#endif
