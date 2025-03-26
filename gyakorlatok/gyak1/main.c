#include "cuboid.h"

#include <stdio.h>

int main(int argc, char* argv[])
{
	Cuboid cuboid;
	double area;
	double volum;
	int square;

	set_size(&cuboid, 7, 6, 10);
	area = calc_surface(&cuboid);
	volum = calc_volum(&cuboid);
	square = have_square(&cuboid);

	printf("Cuboid volum: %lf\n", volum);
	printf("Cuboid surface: %lf\n", area);
	if (square == 1){
        printf("Cuboid have square?: Yes\n");
	}
	else{
        printf("Cuboid have square?:  No\n");
	}

	return 0;
}
