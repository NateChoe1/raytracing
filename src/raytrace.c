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
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "fb.h"
#include "vectors.h"
#include "raytrace.h"

int32_t computeColor(Vector v, Point start) {
//TODO: Add scene data to function args
//For now there is just a sphere of radius 3 at (1, 5, 2)
	//printf("%f %f %f\n", v.x, v.y, v.z);

	Point center = (Point) {.x = 0, .y = 5, .z = 0};
	Vector toSphere = PQ(start, center);
	//NOTE: this is not very well optimized

	Vector toClosest = proj(v, toSphere);
	Point closest = end(start, toClosest);
	if (d2(closest, center) > 1) {
		return RGB(0, 0, 0);
	}
	return RGB(255, 255, 255);
}

void redraw(Vector direction, Point camera, float tilt) {
	const Vector directUp = (Vector) {.x = 0, .y = 0, .z = 1};
	Vector aheadProj = proj(direction, directUp);

	Vector up = sub(directUp, aheadProj);
	Vector right = cross(up, direction);
	normalize(&up);
	normalize(&right);

	Vector realUp = add(mult(cos(tilt), up), mult(sin(tilt), right));
	Vector realRight = cross(direction, realUp);
	normalize(&realUp);
	normalize(&realRight);
	normalize(&direction);
	
	float dist = 1.0 / xres;
	float ystart = (float) yres / xres / 2;

	for (int x = 0; x < xres; x++) {
		for (int y = 0; y < yres; y++) {
			Vector currentDirection = direction;
			currentDirection = add(mult((x * dist - 0.5), realRight), currentDirection);
			currentDirection = add(mult((y * dist - ystart), realUp), currentDirection);
			putPixel(x, y, computeColor(currentDirection, camera));
		}
	}
}

int main() {
	initFramebuffer(getenv("FRAMEBUFFER"));

	Point camera = (Point) {.x = 0, .y = 0, .z = 0};
	Vector direction = (Vector) {.x = 0, .y = 1, .z = 0};
	float tilt = 0;

	redraw(direction, camera, tilt);
	sleep(5);
	exit(EXIT_SUCCESS);
}
