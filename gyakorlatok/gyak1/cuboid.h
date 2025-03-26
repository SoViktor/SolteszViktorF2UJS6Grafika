#ifndef CUBOID_H
#define CUBOID_H

/**
 * Cuboid 3 oldala
 */
typedef struct Cuboid
{
	double x;
	double y;
	double z;
} Cuboid;

/**
 * Set the data of the cuboid
 */
void set_size(Cuboid* Cuboid, double x, double y, double z);

/**
 * Calculate the area of the cuboid.
 */
double calc_volum(const Cuboid* cuboid);

double calc_surface(const Cuboid* cuboid);

int have_square(const Cuboid*  cuboid);

#endif // CUBOID_H
