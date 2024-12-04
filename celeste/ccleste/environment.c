#include <stdarg.h>
#include <stdio.h>
#include <assert.h>

#include "celeste.h"
#include "tilemap.h"
#include "environment.h"


static Uint16 buttons_state = 0;


static int gettileflag(int tile, int flag) {
	return tile < sizeof(tile_flags)/sizeof(*tile_flags) && (tile_flags[tile] & (1 << flag)) != 0;
}

int pico8emu(CELESTE_P8_CALLBACK_TYPE call, ...) {
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

    Celeste_P8_set_rndseed(8);
    Celeste_P8_init();
}

void step(Uint16 action) {
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

void free_state(void* savestate) {
    free(savestate);
}

PlayerState get_player_state(void) {
	OBJ* player_state = Celeste_get_player_state();

	if (player_state != NULL) {
		#define ATTR(n) player_state->n,
		return (PlayerState){
			ATTR(spd)
			ATTR(grace) ATTR(jbuffer) ATTR(djump) ATTR(dash_time)
			ATTR(was_on_ground)
		};
		#undef ATTR
	}
	else {
		return (PlayerState){
			(VEC){0, 0},
			0, 0, 0, 0,
			false
		};
	}
}

RoomState get_room_state(void) {
	VECI* room = Celeste_get_room_state();
	return (RoomState){
		room->x, room->y
	};
}