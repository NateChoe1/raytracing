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
#include <float.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fb.h"
#include "vectors.h"
#include "raytrace.h"

bool sphereCollision(Point start, Vector v,
		Point *collisionReturn, Vector *normalVector,
		Sphere sphere) {
	//the distance between a point on a line and another point create a
	//quadratic. Converting a ray to parametric form and graphing the
	//relationship between t and the distance from the solution for that t
	//and a point produces a quadratic which can be solved with the
	//quadratic formula.
	float cxd = start.x - sphere.center.x;
	float cyd = start.y - sphere.center.y;
	float czd = start.z - sphere.center.z;
	float a = (v.x * v.x + v.y * v.y + v.z * v.z);
	float b = 2 * (
			v.x * cxd +
			v.y * cyd +
			v.z * czd
	);
	float c = cxd * cxd + cyd * cyd + czd * czd - sphere.radius * sphere.radius;

	float discriminant = (b * b) - (4 * a * c);
	if (discriminant < 0)
		return false;
	float t = (-b + sqrt(discriminant)) / (2 * a);
	if (t < 0)
		return false;

	*collisionReturn = end(start, mult(t, v));
	*normalVector = PQ(sphere.center, *collisionReturn);

	return true;
}

bool sceneCollision(Point start, Vector v,
		Point *collisionReturn, Vector *normalVector,
		Scene scene) {
	int bestObject = -1;
	Point bestPoint;
	Vector bestVector;
	float mostDistance = FLT_MAX;
	for (int i = 0; i < scene.objects; i++) {
		Point p;
		Vector v;
		switch (scene.scene[i].type) {
			case SPHERE:
				if (sphereCollision(start, v, &p, &v, scene.scene[i].sphere))
					break;
				continue;
			default:
				return false;
		}
		float curr = mag2(PQ(start, p));
		if (curr < mostDistance) {
			mostDistance = curr;
			bestObject = i;
			bestPoint = p;
			bestVector = v;
		}
	}
	if (bestObject == -1)
		return false;
	*collisionReturn = bestPoint;
	*normalVector = bestVector;
	return true;
}

int32_t computeColor(Vector v, Point start, Scene scene) {
	Point light = (Point) {.x = -1, .y = 5, .z = 2};
	Point intersection;
	Vector normal;
	Sphere sphere = (Sphere) {
		.center = (Point) {.x = 0, .y = 10, .z = 0},
		.radius = 1,
	};


	if (sphereCollision(start, v, &intersection, &normal, sphere)) {
		Vector bounce = reflection(v, normal);
		Vector toLight = PQ(intersection, light);
		float theta = angle(bounce, toLight);
		float diff = M_PI - fabs(theta - M_PI) - 2;
		int col = (int) (diff * 200);

		return RGB(col, col, col);
	}
	return RGB(0, 0, 0);
}

void redraw(Vector direction, Point camera, float tilt, Scene scene) {
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
			putPixel(x, y, computeColor(currentDirection, camera, scene));
		}
	}
}

int main() {
	initFramebuffer(getenv("FRAMEBUFFER"));

	Point camera = (Point) {.x = 0, .y = 0, .z = 0};
	Vector direction = (Vector) {.x = 0, .y = 1, .z = 0};
	float tilt = 0;

	Scene scene;
	scene.scene = malloc(sizeof(Object));
	scene.light = malloc(sizeof(LightSource));
	scene.objects = 1;
	scene.scene[0]= (Object) {
		.type = SPHERE,
		.sphere = (Sphere) {
			.center = (Point){
				.x = 0,
				.y = 10,
				.z = 0,
			},
			.radius = 1,
		},
	};
	scene.sources = 1;
	scene.light[0]= (LightSource) {
		.location = (Point) {
			.x = -2,
			.y = 4,
			.z = -1,
		},
		.strength = 200,
	};

	redraw(direction, camera, tilt, scene);
	sleep(5);
	exit(EXIT_SUCCESS);
}
