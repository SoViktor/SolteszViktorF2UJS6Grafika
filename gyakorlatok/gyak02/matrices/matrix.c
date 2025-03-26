#include "matrix.h"
#define PI 3.14159265358979323846f


#include <stdio.h>

void init_zero_matrix(float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = 0.0;
        }
    }
}

void print_matrix(const float matrix[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            printf("%4.4f ", matrix[i][j]);
        }
        printf("\n");
    }
}

void add_matrices(const float a[3][3], const float b[3][3], float c[3][3])
{
    int i;
    int j;

    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            c[i][j] = a[i][j] + b[i][j];
        }
    }
}

void init_identity_matrix(float matrix[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            matrix[i][j] = (i == j) ? 1.0f : 0.0f;
        }
    }
}

void multiply_matrix_by_scalar(const float matrix[3][3], float scalar, float result[3][3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = matrix[i][j] * scalar;
        }
    }
}

void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3])
{
    int i, j, k;
    for (i = 0; i < 3; ++i) {
        for (j = 0; j < 3; ++j) {
            result[i][j] = 0.0f;
            for (k = 0; k < 3; ++k) {
                result[i][j] += a[i][k] * b[k][j];
            }
        }
    }
}

void transform_point(const float matrix[3][3], const float point[3], float result[3])
{
    int i, j;
    for (i = 0; i < 3; ++i) {
        result[i] = 0.0f;
        for (j = 0; j < 3; ++j) {
            result[i] += matrix[i][j] * point[j];
        }
    }
}

#include <math.h>

void scale(float m[3][3], float sx, float sy)
{
    float s[3][3] = {
        {sx, 0.0f, 0.0f},
        {0.0f, sy, 0.0f},
        {0.0f, 0.0f, 1.0f}
    };
    float temp[3][3];
    multiply_matrices(s, m, temp);
    // Másoljuk vissza temp-et m-be
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = temp[i][j];
}

void shift(float m[3][3], float tx, float ty)
{
    float t[3][3] = {
        {1.0f, 0.0f, tx},
        {0.0f, 1.0f, ty},
        {0.0f, 0.0f, 1.0f}
    };
    float temp[3][3];
    multiply_matrices(t, m, temp);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = temp[i][j];
}

void rotate(float m[3][3], float angle_degrees)
{
    float angle_radians = angle_degrees * (float)PI / 180.0f;
    float c = cosf(angle_radians);
    float s = sinf(angle_radians);

    float r[3][3] = {
        {c, -s, 0.0f},
        {s,  c, 0.0f},
        {0.0f, 0.0f, 1.0f}
    };
    float temp[3][3];
    multiply_matrices(r, m, temp);
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 3; ++j)
            m[i][j] = temp[i][j];
}

