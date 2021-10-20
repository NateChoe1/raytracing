/*
fbtrace: a raytracer for the linux framebuffer
Copyright (C) 2021  Nate Choe

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <https://www.gnu.org/licenses/>.

Contact me at natechoe1@gmail.com
*/

#include <math.h>
#include <stdlib.h>

#include "vectors.h"

#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wstrict-aliasing"
float q_rsqrt(float n) {
	int32_t i;
	float x2, y;

	x2 = n * 0.5;
	y = n;
	i = * (int32_t * ) &y;
	i = 0x5f3759df - (i >> 1);
	y = * (float *) &i;

	y = y * (1.5 - (x2 * y * y));

	return y;
}
#pragma GCC diagnostic pop
//I'm using a linux tty, so I can't copy paste here, hopefully that's right!

float mag2(Vector v) {
	return v.x * v.x + v.y * v.y + v.z * v.z;
}

float d2(Point p1, Point p2) {
	register float dx = p2.x - p1.x;
	register float dy = p2.y - p1.y;
	register float dz = p2.z - p1.z;
	return dx * dx + dy * dy + dz * dz;
}

void normalize(Vector *v) {
	float total = mag2(*v);
	total = q_rsqrt(total);
	v->x *= total;
	v->y *= total;
	v->z *= total;
}

Vector PQ(Point P, Point Q) {
	return (Vector) {
		.x = Q.x - P.x,
		.y = Q.y - P.y,
		.z = Q.z - P.z,
	};
}

float dot(Vector u, Vector v) {
	return u.x * v.x + u.y * v.y + u.z * v.z;
}

Vector cross(Vector u, Vector v) {
	return (Vector) {
		.x = u.y * v.z - u.z * v.y, 
		.y = u.x * v.z - u.z * v.x,
		.z = u.x * v.y - u.y * v.x,
	};
}

Vector mult(float scalar, Vector v) {
	return (Vector) {
		.x = v.x * scalar,
		.y = v.y * scalar,
		.z = v.z * scalar,
	};
}

Vector proj(Vector v, Vector u) {
	return mult(dot(u, v) / dot(v, v), v);
}

Point end(Point start, Vector v) {
	return (Point) {
		.x = start.x + v.x,
		.y = start.y + v.y,
		.z = start.z + v.z,
	};
}

Vector sub(Vector v, Vector u) {
	return (Vector) {
		.x = v.x - u.x,
		.y = v.y - u.y,
		.z = v.z - u.z,
	};
}

Vector reflection(Vector v, Vector norm) {
//given the normal vector of some plane, find the reflection of that plane onto v.
	norm = mult(2, proj(norm, v));
	return add(v, norm);
}

float angle(Vector u, Vector v) {
	return acos(dot(u, v) / sqrt(mag2(u) * mag2(v)));
}
