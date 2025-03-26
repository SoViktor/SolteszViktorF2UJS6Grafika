#include "matrix.h"
#include <stdio.h>


int main(int argc, char* argv[])
{
	float a[3][3] = {
        { 1.0f, -2.0f,  3.0f},
        { 5.0f, -3.0f,  0.0f},
        {-2.0f,  1.0f, -4.0f}
    };
	float b[3][3];
	float c[3][3];
    float d[3][3];
    float scaled[3][3];
    float product[3][3];

    init_zero_matrix(b);
    b[1][1] =  8.0f;
    b[2][0] = -3.0f;
    b[2][2] =  5.0f;

    print_matrix(a);
    print_matrix(b);
    
    add_matrices(a, b, c);

    print_matrix(c);

init_identity_matrix(d);
print_matrix(d);


multiply_matrix_by_scalar(a, 2.0f, scaled);
print_matrix(scaled);

multiply_matrices(a, b, product);
print_matrix(product);

float m[3][3];
init_identity_matrix(m); // identitásmátrix = nincs változás

float p[3] = {2.0f, 3.0f, 1.0f};  // (x, y, 1)
float transformed[3];

transform_point(m, p, transformed);

printf("Transformed point: (%.2f, %.2f, %.2f)\n", transformed[0], transformed[1], transformed[2]);

float t[3][3];
init_identity_matrix(t);
scale(t, 2.0f, 2.0f);         // duplázás
shift(t, 10.0f, 5.0f);        // eltolás
rotate(t, 45.0f);             // 45 fokos forgatás
print_matrix(t);              // az eredő transzformációs mátrix

// Stack part next


MatrixStack stack;
init_matrix_stack(&stack);

float trans[3][3];
init_identity_matrix(trans);

// Push initial state
push_matrix(&stack, trans);

// Apply transformations
scale(trans, 2.0f, 2.0f);
shift(trans, 5.0f, 0.0f);
rotate(trans, 30.0f);

printf("Transformed matrix:\n");
print_matrix(trans);

// Undo
if (pop_matrix(&stack, trans)) {
    printf("After pop (undo):\n");
    print_matrix(trans);
} else {
    printf("Stack is empty!\n");
}


	return 0;
}

