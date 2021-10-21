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

typedef struct {
	float x;
	float y;
	float z;
} ThreeDimensions;
//right handed coordinates because I'm not a monster.

typedef ThreeDimensions Point;
typedef ThreeDimensions Vector;

float q_rsqrt(float n);
float mag2(Vector v);
float d2(Point p1, Point p2);
void normalize(Vector *v);
Vector PQ(Point P, Point Q);
float dot(Vector u, Vector v);
Vector cross(Vector u, Vector v);
Vector mult(float scalar, Vector v);
Vector proj(Vector v, Vector u);
Vector end(Point start, Vector v);
#define add(v, u) end(v, u)
//I don't feel like making the same thing twice, ok?
Vector sub(Vector v, Vector u);
Vector reflection(Vector v, Vector norm);
float angle(Vector v, Vector u);
void printVector(Vector v);
