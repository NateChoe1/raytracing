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
#include <unistd.h>
#include <stdlib.h>

#include "fb.h"
#include "vectors.h"
#include "raytrace.h"

int32_t computeColor(Vector v, Point start) {
//TODO: Add scene data to function args
//For now there is just a sphere of radius 1 at (0, 5, 0)
	Point center = (Point) {.x = 2, .y = 10, .z = 0};
	Point light = (Point) {.x = -1, .y = 5, .z = 2};
	Vector toSphere = PQ(start, center);
	//NOTE: this is not very well optimized

	Vector toClosest = proj(v, toSphere);
	Point closest = end(start, toClosest);
	float radius = 1;
	if (d2(closest, center) < radius * radius) {
		//the distance between a point on a line and another point create a quadratic. Converting
		//a vector to parametric form and graphing the relationship between t and the distance
		//from the solution for that t and a point produces a quadratic which can be solved with
		//the quadratic formula.
		float cxd = start.x - center.x;
		float cyd = start.y - center.y;
		float czd = start.z - center.z;
		float a = (v.x * v.x + v.y * v.y + v.z * v.z);
		float b = 2 * (
				v.x * cxd +
				v.y * cyd +
				v.z * czd
		);
		float c = cxd * cxd + cyd * cyd + czd * czd - radius * radius;

		float discriminant = (b * b - 4 * a * c);
		if (discriminant < 0)
			return RGB(255, 255, 255);
		float t = (-b + sqrt(discriminant)) / (2 * a);
		if (t < 0)
			return RGB(255, 255, 255);

		Point intersection = end(start, mult(t, v));
		Vector norm = PQ(center, intersection);
		Vector bounce = reflection(v, norm);
		Vector toLight = PQ(intersection, light);

		float theta = angle(bounce, toLight);
		float diff = M_PI - fabs(theta - M_PI) - 2;
		int col = (int) (diff * 200);

		return RGB(col, col, col);
	}
	return RGB(0, 0, 0);
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
