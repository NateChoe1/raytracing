#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#include "fb.h"
#include "vectors.h"
#include "raytrace.h"

int main() {
	initFramebuffer(getenv("FRAMEBUFFER"));

	Point camera = (Point) {.x = 0, .y = 0, .z = 0};
	Vector direction = (Vector) {.x = 0, .y = 1, .z = 0};
	float tilt = 0;

	Scene scene;
	scene.scene = malloc(sizeof(Object) * 2);
	scene.light = malloc(sizeof(LightSource) * 2);
	scene.objects = 2;
	scene.scene[0]= (Object) {
		.type = SPHERE,
		.brightness = 1,
		.sphere = (Sphere) {
			.center = (Point){
				.x = 0,
				.y = 10,
				.z = 0,
			},
			.radius = 1,
		},
	};
	scene.scene[1]= (Object) {
		.type = SPHERE,
		.brightness = 1,
		.sphere = (Sphere) {
			.center = (Point){
				.x = 1,
				.y = 10,
				.z = 0,
			},
			.radius = 1,
		},
	};
	scene.sources = 1;
	scene.light[0]= (LightSource) {
		.location = (Point) {
			.x = 0,
			.y = 0,
			.z = 0,
		},
		.strength = 25,
	};

	for (;;)
		redraw(direction, camera, tilt, scene);
	exit(EXIT_SUCCESS);
}
