#pragma once

void mat4_perspective(float matrix[16], float fov_rad, float aspect, float near, float far);
void mat4_identity(float* m);
void mat4_translate(float* m, float x, float y, float z);
void mat4_rotate_y(float* m, float angle_deg);
void mat4_scale_uniform(float* m, float scale);
void mat4_mul(float out[16], const float a[16], const float b[16]);
