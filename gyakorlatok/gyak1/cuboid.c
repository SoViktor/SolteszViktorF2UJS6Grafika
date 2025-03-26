#include "cuboid.h"

#include <math.h>

void set_size (Cuboid* cuboid, double x, double y, double z)

{

	if (x > 0.0) {
		cuboid->x = x;
	} else {
		cuboid->x = NAN;
	}
	if (y > 0.0) {
		cuboid->y = y;
	} else {
		cuboid->y = NAN;
	}
	if (z > 0.0) {
		cuboid->z = z;
	} else {
		cuboid->z = NAN;
	}
}

double calc_volum  (const Cuboid* cuboid)
{
	double volum = cuboid->x * cuboid->y * cuboid->z;
	return volum;
}

double calc_surface (const Cuboid* cuboid)
{
    double area = 2 * (cuboid->x * cuboid->y + cuboid->x * cuboid->z + cuboid->y * cuboid->z);
    return area;
}

int have_square(const Cuboid*  cuboid)
{
    if (cuboid->x == cuboid->y || cuboid->x == cuboid->z || cuboid->z == cuboid->y)
    {
       return 1;
    }
    else
    {
        return 0;
    }
}
