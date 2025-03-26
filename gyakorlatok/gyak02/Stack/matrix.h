#ifndef MATRIX_H
#define MATRIX_H
#define MATRIX_STACK_SIZE 10

/**
 * Initializes all elements of the matrix to zero.
 */
void init_zero_matrix(float matrix[3][3]);

/**
 * Print the elements of the matrix.
 */
void print_matrix(const float matrix[3][3]);

/**
 * Add matrices.
 */
void add_matrices(const float a[3][3], const float b[3][3], float c[3][3]);

/**
 * Initializes the matrix as identity matrix.
 */
void init_identity_matrix(float matrix[3][3]);

/**
 * Multiplies a matrix by a scalar.
 */
void multiply_matrix_by_scalar(const float matrix[3][3], float scalar, float result[3][3]);

/**
 * Multiplies two 3x3 matrices.
 */
void multiply_matrices(const float a[3][3], const float b[3][3], float result[3][3]);

/**
 * Transforms a 2D point (with homogeneous coordinates) using a transformation matrix.
 */
void transform_point(const float matrix[3][3], const float point[3], float result[3]);

/**
 * Applies scaling to the transformation matrix (around origin).
 */
void scale(float m[3][3], float sx, float sy);

/**
 * Applies translation (shift) to the transformation matrix.
 */
void shift(float m[3][3], float tx, float ty);

/**
 * Applies rotation to the transformation matrix (around origin, in degrees).
 */
void rotate(float m[3][3], float angle_degrees);

/**
 * Stack structure to store transformation matrices.
 */
typedef struct {
    float stack[MATRIX_STACK_SIZE][3][3];
    int top;
} MatrixStack;

/**
 * Initializes the matrix stack.
 */
void init_matrix_stack(MatrixStack* ms);

/**
 * Pushes the given matrix onto the stack.
 */
int push_matrix(MatrixStack* ms, const float matrix[3][3]);

/**
 * Pops the top matrix from the stack into `matrix`.
 */
int pop_matrix(MatrixStack* ms, float matrix[3][3]);


#endif // MATRIX_H

