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

#define SPHERE 0

typedef struct {
	Point location;
	float strength;
} LightSource;

typedef struct {
	Point center;
	float radius;
} Sphere;

typedef struct {
	int type;
	float brightness;
	union {
		Sphere sphere;
	};
} Object;

typedef struct {
	int objects;
	Object *scene;
	int sources;
	LightSource *light;
} Scene;

bool sphereCollision(Point start, Vector v, Point *collisionReturn, Vector *normalVector, Sphere sphere);
int sceneCollision(Point start, Vector v, Point *collisionReturn, Vector *normalVector, Scene scene);
int32_t computeColor(Vector v, Point start, Scene scene);
void redraw(Vector direction, Point camera, float tilt, Scene scene);
