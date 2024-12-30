#ifndef CELESTE_H_
#define CELESTE_H_

#ifdef __cplusplus
#define _Bool bool
extern "C" {
#endif

typedef enum {
	CELESTE_P8_MUSIC, CELESTE_P8_SPR, CELESTE_P8_BTN, CELESTE_P8_SFX,
	CELESTE_P8_PAL, CELESTE_P8_PAL_RESET, CELESTE_P8_CIRCFILL, CELESTE_P8_PRINT,
	CELESTE_P8_RECTFILL, CELESTE_P8_LINE, CELESTE_P8_MGET, CELESTE_P8_CAMERA,
	CELESTE_P8_FGET, CELESTE_P8_MAP
} CELESTE_P8_CALLBACK_TYPE;

typedef _Bool Celeste_P8_bool_t;
typedef int (*Celeste_P8_cb_func_t) (CELESTE_P8_CALLBACK_TYPE calltype, ...);


#define bool Celeste_P8_bool_t
#define false 0
#define true 1

extern void Celeste_P8_set_call_func(Celeste_P8_cb_func_t func);
extern void Celeste_P8_set_rndseed(unsigned seed);
extern void Celeste_P8_init(void);
extern void Celeste_P8_update(void);
extern void Celeste_P8_draw(void);

extern void Celeste_P8__DEBUG(void); //debug functionality

//state functionality
size_t Celeste_P8_get_state_size(void);
void Celeste_P8_save_state(void* st);
void Celeste_P8_load_state(const void* st);

#ifdef __cplusplus
} //extern "C"
#endif


#include "fixedp.h"


typedef struct {decimal x,y;} VEC;
typedef struct {int x,y;} VECI;

//with this X macro table thing we can define the properties that each object type has, in the original lua code these properties
//are inferred from the `types` table
#define OBJ_PROP_LIST() \
	/* TYPE        TILE   HAS INIT  HAS UPDATE  HAS DRAW    IF_NOT_FRUIT */\
	X(PLAYER,       -1,     Y,        Y,          Y,            false)\
	X(PLAYER_SPAWN,  1,     Y,        Y,          Y,            false)\
	X(SPRING,       18,     Y,        Y,          N,            false)\
	X(BALLOON,      22,     Y,        Y,          Y,            false)\
	X(SMOKE,        -1,     Y,        Y,          N,            false)\
	X(PLATFORM,     -1,     Y,        Y,          Y,            false)\
	X(FALL_FLOOR,   23,     Y,        Y,          Y,            false)\
	X(FRUIT,        26,     Y,        Y,          N,             true)\
	X(FLY_FRUIT,    28,     Y,        Y,          Y,             true)\
	X(FAKE_WALL,    64,     N,        Y,          Y,             true)\
	X(KEY,           8,     N,        Y,          N,             true)\
	X(CHEST,        20,     Y,        Y,          N,             true)\
	X(LIFEUP,       -1,     Y,        Y,          Y,            false)\
	X(MESSAGE,      86,     N,        N,          Y,            false)\
	X(BIG_CHEST,    96,     Y,        N,          Y,            false)\
	X(ORB,          -1,     Y,        N,          Y,            false)\
	X(FLAG,        118,     Y,        N,          Y,            false)\
	X(ROOM_TITLE,   -1,     Y,        N,          Y,            false)

typedef enum {
	#define X(t,...) OBJ_##t,
	OBJ_PROP_LIST()
	#undef X
	OBJTYPE_COUNT
} OBJTYPE;

typedef struct {
	bool active;
	decimal x,y,s,spd,off,c,h,t;
	VEC spd2; //used by dead particles, moved from spd
} PARTICLE;

typedef struct {int x,y,w,h;} HITBOX;

typedef struct {
	decimal x,y,size;
	bool isLast;
} HAIR;

//OBJECT strucutre
typedef struct {
	bool active;
	short id; //unique identifier for each object, incremented per object

	//inherited
	OBJTYPE type;
	bool collideable, solids;
	decimal spr;
	bool flip_x, flip_y;
	decimal x, y;
	HITBOX hitbox;
	VEC spd;
	VEC rem;

	//player
	bool p_jump, p_dash;
	int grace, jbuffer, djump, dash_time;
	short dash_effect_time; //can underflow in normal gameplay (after 18 minutes)
	VEC dash_target;
	VEC dash_accel;
	decimal spr_off;
	bool was_on_ground;
	HAIR hair[5]; //also player_spawn

	//player_spawn
	int state, delay;
	VEC target;

	//spring
	int hide_in, hide_for;

	//balloon
	int timer;
	decimal offset, start;

	//fruit
	decimal off;

	//fly_fruit
	bool fly;
	decimal step;
	int sfx_delay;

	//lifeup
	int duration;
	decimal flash;

	//platform
	decimal last, dir;

	//message
	const char* text;
	decimal index;
	VECI off2; //changed from off..

	//big chest
	PARTICLE particles[50];
	int particle_count;

	//flag
	int score;
	bool show;
} OBJ;


#ifdef __cplusplus
extern "C" {
#endif

// Game Info

// return pointer to the player object, so it can be accessed by users of celeste.h
OBJ* Celeste_get_player_state(void);
unsigned char Celeste_get_room(void);
void Celeste_get_gameplay_time(short* min, int* sec, int* f);
int Celeste_get_deaths(void);
int Celeste_get_fruits(void);
void Celeste_load_room(unsigned char room_index);

#ifdef __cplusplus
} //extern "C"
#endif

#endif
