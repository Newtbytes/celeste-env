#include <cstdio>
#include <string.h>
#include "data/gfx.h"
#include "data/font.h"
#include "pico8.h"


unsigned char screen[SCREEN_SIZE][SCREEN_SIZE][3];


void p8_init() {
    memset(screen, 0, sizeof(screen));
}

void p8_get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3]) {
	for (int i = 0; i < SCREEN_SIZE; i++) {
		for (int j = 0; j < SCREEN_SIZE; j++) {
			for (int k = 0; k < 3; k++) {
				input_screen[i][j][k] = screen[i][j][k];
			}
		}
	}
}

// convert pico8 color to rgb
constexpr void getcolor(char idx, unsigned char* r, unsigned char* g, unsigned char* b) {
	int col = palette[idx%16];
	*r = (col >> 16) & 0xFF;
	*g = (col >> 8) & 0xFF;
	*b = col & 0xFF;
}

inline void setpixel(char col, int x, int y) {
	if (0 <= x && x < SCREEN_SIZE && 0 <= y && y < SCREEN_SIZE)
		getcolor(col, &screen[y][x][0], &screen[y][x][1], &screen[y][x][2]);
}

void p8_spr(int spr, int x, int y, bool flipx, bool flipy) {
	for (int dx = 0; dx < SPR_SIZE; dx++) {
    	for (int dy = 0; dy < SPR_SIZE; dy++) {
            int sx = flipx ? (SPR_SIZE - 1 - dx) : dx;
            int sy = flipy ? (SPR_SIZE - 1 - dy) : dy;

			int col = gfx[spr][sy * 8 + sx];

			if (col != 0)
            	setpixel(col, (x + dx), (y + dy));
        }
    }
}

void p8_print(const char* str, int x, int y, int col) {
	for (char c = *str; c; c = *(++str)) {
		c &= 0x7F;

		for (int dx = 0; dx < SPR_SIZE; dx++) {
			for (int dy = 0; dy < SPR_SIZE; dy++) {
				int col = font[c][dy * 8 + dx];

				if (col != 0)
					setpixel(7, (x + dx), (y + dy));
			}
		}

		x += 4;
	}
}

void clamp(int* x) {
	if (*x > SCREEN_SIZE) {
		*x = SCREEN_SIZE;
	}
	else if (*x < 0) {
		*x = 0;
	}
}

void p8_rectfill(int x0, int y0, int x1, int y1, int col) {
    if (x1 < x0 || y1 < y0) {
        return;
    }

	clamp(&x0);
	clamp(&y0);
	clamp(&x1);
	clamp(&y1);

	for (int x = x0; x < x1 + 1; x++) {
		for (int y = y0; y < y1 + 1; y++) {
			setpixel(col, x, y);
		}
	}
}
