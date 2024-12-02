#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

#include "celeste.h"
#include "tilemap.h"
#include "environment.h"


static unsigned short buttons_state = 0;
static void* initial_game_state = NULL;


static int gettileflag(int tile, int flag) {
	return tile < sizeof(tile_flags)/sizeof(*tile_flags) && (tile_flags[tile] & (1 << flag)) != 0;
}

int pico8emu(CELESTE_P8_CALLBACK_TYPE call, ...) {
	static int camera_x = 0, camera_y = 0;

	va_list args;
	int ret = 0;
	va_start(args, call);
	
	#define   INT_ARG() va_arg(args, int)
	#define  BOOL_ARG() (Celeste_P8_bool_t)va_arg(args, int)
	#define RET_INT(_i)   do {ret = (_i); goto end;} while (0)
	#define RET_BOOL(_b) RET_INT(!!(_b))

	switch (call) {
		case CELESTE_P8_BTN: { //btn(b)
			int b = INT_ARG();
			assert(b >= 0 && b <= 5); 
			RET_BOOL(buttons_state & (1 << b));
		} break;
		case CELESTE_P8_MGET: { //mget(tx,ty)
			int tx = INT_ARG();
			int ty = INT_ARG();

			RET_INT(tilemap_data[tx+ty*128]);
		} break;
		case CELESTE_P8_FGET: { //fget(tile,flag)
			int tile = INT_ARG();
			int flag = INT_ARG();

			RET_INT(gettileflag(tile, flag));
		} break;

        default: break;
	}

	end:
	va_end(args);
	return ret;
}

void init(void) {
    Celeste_P8_set_call_func(pico8emu);

    reset();

	initial_game_state = save();
}

void reset(void) {
    // TODO: support seeding env
    Celeste_P8_set_rndseed(8);

    Celeste_P8_init();

    if (initial_game_state) Celeste_P8_load_state(initial_game_state);
}

void step(unsigned short action) {
    buttons_state = action;
    Celeste_P8_update();
}

void* save(void) {
    void* savestate = malloc(get_state_size());
    if (savestate) Celeste_P8_save_state(savestate);

    return savestate;
}

void load(const void* savestate) {
    Celeste_P8_load_state(savestate);
}

size_t get_state_size(void) {
    return Celeste_P8_get_state_size();
}

void close(void) {
    free(initial_game_state);
}
