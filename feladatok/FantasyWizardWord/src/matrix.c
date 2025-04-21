#include "matrix.h"
#include <math.h>
#include <string.h>

void mat4_perspective(float matrix[16], float fov_rad, float aspect, float near, float far) {
    float f = 1.0f / tanf(fov_rad / 2.0f);
    memset(matrix, 0, sizeof(float) * 16);

    matrix[0] = f / aspect;
    matrix[5] = f;
    matrix[10] = (far + near) / (near - far);
    matrix[11] = -1.0f;
    matrix[14] = (2.0f * far * near) / (near - far);
}
