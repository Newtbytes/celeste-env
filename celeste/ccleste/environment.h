#include <stdlib.h>
#include "celeste.h"


typedef unsigned short Uint16;


void init(void);

void step(Uint16 action);

void* save(void);
void load(const void* savestate);
size_t get_state_size(void);
void free_state(void* savestate);


typedef struct PlayerState {
    VEC spd;
    float x, y;
    int grace, jbuffer, djump, dash_time;
    bool was_on_ground;
    int deaths, fruits;
    short minutes;
    int seconds, frames;
} PlayerState;

typedef struct RoomState {
    int x, y;
} RoomState;


PlayerState get_player_state(void);
RoomState get_room_state(void);