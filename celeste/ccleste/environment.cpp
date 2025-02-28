#include <stdarg.h>
#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "celeste.h"
#include "data/tilemap.h"
#include "environment.h"
#include "pico8.h"


static Uint16 buttons_state = 0;


bool handle_render_cb = true;


static inline int gettileflag(int tile, int flag) {
	return tile < sizeof(tile_flags)/sizeof(*tile_flags) && (tile_flags[tile] & (1 << flag)) != 0;
}

// TODO: implement image observations
// write drawing operations to buffer and convert buffer to numpy array in ccleste.pyx
int pico8emu(CELESTE_P8_CALLBACK_TYPE call, ...) {
	static int camera_x = 0, camera_y = 0;
	camera_x = camera_y = 0;


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

		// rendering callbacks
		case CELESTE_P8_SPR: { //spr(sprite,x,y,cols,rows,flipx,flipy)
			if (!handle_render_cb)
				goto end;

			int spr = INT_ARG();
			int x = INT_ARG();
			int y = INT_ARG();
			int cols = INT_ARG();
			int rows = INT_ARG();
			int flipx = BOOL_ARG();
			int flipy = BOOL_ARG();

			assert(rows == 1 && cols == 1);

			p8_spr(spr, x, y, flipx, flipy);
		} break;
		case CELESTE_P8_PRINT: { //print(str,x,y,col)
			if (!handle_render_cb)
				goto end;

			const char* str = va_arg(args, const char*);
			int x = INT_ARG() - camera_x;
			int y = INT_ARG() - camera_y;
			int col = INT_ARG() % 16;

			p8_print(str,x,y,col);
		} break;
		case CELESTE_P8_RECTFILL: { //rectfill(x0,y0,x1,y1,col)
			if (!handle_render_cb)
				goto end;

			int x0 = INT_ARG() - camera_x;
			int y0 = INT_ARG() - camera_y;
			int x1 = INT_ARG() - camera_x;
			int y1 = INT_ARG() - camera_y;
			int col = INT_ARG();

			p8_rectfill(x0,y0,x1,y1,col);
		} break;
		case CELESTE_P8_MAP: { //map(mx,my,tx,ty,mw,mh,mask)
			if (!handle_render_cb)
				goto end;

			int mx = INT_ARG(), my = INT_ARG();
			int tx = INT_ARG(), ty = INT_ARG();
			int mw = INT_ARG(), mh = INT_ARG();
			int mask = INT_ARG();
			
			for (int x = 0; x < mw; x++) {
				for (int y = 0; y < mh; y++) {
					int tile = tilemap_data[x + mx + (y + my)*128];
					//hack
					if (mask == 0 || (mask == 4 && tile_flags[tile] == 4) || gettileflag(tile, mask != 4 ? mask-1 : mask)) {
						p8_spr(tile, (tx+x*8), (ty+y*8), false, false);
					}
				}
			}
		} break;

        default: break;
	}

	end:
	va_end(args);
	return ret;
}

void init(void) {
    Celeste_P8_set_call_func(pico8emu);

	p8_init();

    Celeste_P8_set_rndseed(8);
    Celeste_P8_init();


	for (int t = 1; t <= 80; t++) {
		if (t==1) buttons_state = 1<<4;
		else buttons_state = 0;

		Celeste_P8_update();
		Celeste_P8_draw();
	}
}

void step(Uint16 action) {
    buttons_state = action;
    Celeste_P8_update();
	Celeste_P8_draw();
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

void free_state(void* savestate) {
    free(savestate);
}

PlayerState get_player_state(void) {
	OBJ* player_state = Celeste_get_player_state();

	short minutes = 0;
	int seconds, frames = 0;
	Celeste_get_gameplay_time(&minutes, &seconds, &frames);

	int deaths = Celeste_get_deaths();
	int fruits = Celeste_get_fruits();

	if (player_state != NULL) {
		#define ATTR(n) player_state->n,
		return (PlayerState){
			(float)ATTR(spd.x) (float)ATTR(spd.y)
			(float)ATTR(x) (float)ATTR(y)
			ATTR(grace) ATTR(jbuffer) ATTR(djump) ATTR(dash_time)
			ATTR(was_on_ground)
			deaths, fruits,
			minutes, seconds, frames,
		};
		#undef ATTR
	}
	else {
		return (PlayerState){
			0, 0,
			0, 0,
			0, 0, 0, 0,
			false,
			0, 0,
			0, 0, 0
		};
	}
}

unsigned char get_room(void) {
	return Celeste_get_room();
}

void get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3]) {
	return p8_get_screen(input_screen);
}

void set_render_enabled(bool enabled) {
	Celeste_set_render_enabled(enabled);
	handle_render_cb = enabled;
}
