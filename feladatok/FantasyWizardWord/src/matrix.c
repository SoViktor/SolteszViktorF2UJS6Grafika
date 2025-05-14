#include "matrix.h"
#include <math.h>
#include <string.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void mat4_perspective(float matrix[16], float fov_rad, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov_rad / 2.0f);
    memset(matrix, 0, sizeof(float) * 16);
    matrix[0] = f / aspect;
    matrix[5] = f;
    matrix[10] = (far + near) / (near - far);
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * far * near) / (near - far);
}

void mat4_identity(float m[16]) {
    memset(m, 0, sizeof(float) * 16);
    m[0] = m[5] = m[10] = m[15] = 1.0f;
}

void mat4_translate(float m[16], float x, float y, float z) {
    mat4_identity(m);
    m[12] = x;
    m[13] = y;
    m[14] = z;
}

void mat4_rotate_y(float m[16], float angle_deg) {
    float angle_rad = angle_deg * (float)M_PI / 180.0f;
    float c = cosf(angle_rad);
    float s = sinf(angle_rad);
    mat4_identity(m);
    m[0] = c;
    m[2] = -s;
    m[8] = s;
    m[10] = c;
}

void mat4_scale_uniform(float m[16], float scale) {
    mat4_identity(m);
    m[0] = m[5] = m[10] = scale;
}

void mat4_mul(float out[16], const float a[16], const float b[16]) {
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            out[row * 4 + col] =
                a[row * 4 + 0] * b[0 * 4 + col] +
                a[row * 4 + 1] * b[1 * 4 + col] +
                a[row * 4 + 2] * b[2 * 4 + col] +
                a[row * 4 + 3] * b[3 * 4 + col];
        }
    }
}
