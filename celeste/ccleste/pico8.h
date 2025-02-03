#pragma once

#define SCREEN_SIZE 128
#define SPR_SIZE 8


const int palette[16] = {
    0x000000,
    0x1D2B53,
    0x7E2553,
    0x008751,
    0xAB5236,
    0x5F574F,
    0xC2C3C7,
    0xFFF1E8,
    0xFF004D,
    0xFFA300,
    0xFFEC27,
    0x00E436,
    0x29ADFF,
    0x83769C,
    0xFF77A8,
    0xFFCCAA
};

typedef void (*spr_renderer)(int x, int y);

void p8_init();
void p8_get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3]);
void p8_spr(int spr, int x, int y, bool flipx, bool flipy);
void p8_print(const char* str, int x, int y, int col);
void p8_rectfill(int x0, int y0, int x1, int y1, int col);
