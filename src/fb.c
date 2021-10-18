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

#include <fcntl.h>
#include <stdint.h>
#include <assert.h>
#include <unistd.h>
#include <linux/fb.h>
#include <sys/mman.h>
#include <sys/ioctl.h>

#include "fb.h"

int xres, yres;

static int32_t *fb;
static int fbfd;
static struct fb_var_screeninfo info;

void initFramebuffer(char *path) {
	if (path == NULL)
		path = "/dev/fb0";

	fbfd = open(path, O_RDWR);
	assert(fbfd > 0);

	assert(ioctl(fbfd, FBIOGET_VSCREENINFO, &info) == 0);

	xres = info.xres;
	yres = info.yres;
	fb = mmap(NULL, xres * yres * info.bits_per_pixel / 8,
			PROT_READ | PROT_WRITE, MAP_SHARED, fbfd, 0);
	assert(fb != NULL);
}

void putPixel(int x, int y, int32_t color) {
	fb[info.xres * y + x] = color;
}
