#include <stdlib.h>
#include "celeste.h"


typedef unsigned short Uint16;
typedef unsigned int Uint32;


#define SCREEN_SIZE 128
#define RENDER_CALLS CELESTE_P8_SPR | CELESTE_P8_RECTFILL | CELESTE_P8_MAP


void init(void);

void step(Uint16 action);

void* save(void);
void load(const void* savestate);
size_t get_state_size(void);
void free_state(void* savestate);


typedef struct PlayerState {
    float spd_x, spd_y;
    float x, y;
    int grace, jbuffer, djump, dash_time;
    bool was_on_ground;
    int deaths, fruits;
    short minutes;
    int seconds, frames;
} PlayerState;


PlayerState get_player_state(void);

unsigned char get_room(void);

void get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3]);

void set_render_enabled(bool enabled);