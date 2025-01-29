from libcpp cimport bool

cimport numpy as np
import numpy as np


DEF SCREEN_SIZE = 128


cdef extern from "environment.h":
    void init()

    void step(unsigned short action)

    void* save()
    void load(const void* savestate)

    size_t get_state_size()
    void free_state(void* savestate)

    cdef struct PlayerState:
        float spd_x, spd_y

        float x, y

        int grace, jbuffer, djump, dash_time

        bint was_on_ground

        int deaths, fruits
        short minutes
        int seconds, frames

    cdef struct RoomState:
        int x
        int y

    PlayerState get_player_state()

    unsigned char get_room()

    void get_screen(unsigned char input_screen[SCREEN_SIZE][SCREEN_SIZE][3])

    void set_render_enabled(bool enabled)


cdef np.ndarray get_screen_as_np():
    cdef unsigned char screen[SCREEN_SIZE][SCREEN_SIZE][3]
    get_screen(screen)

    screen_np = np.empty((SCREEN_SIZE, SCREEN_SIZE, 3), dtype=np.uint8)
    cdef unsigned char [:,:,:] screen_np_view = screen_np

    cdef int i, j, k
    for i in range(SCREEN_SIZE):
        for j in range(SCREEN_SIZE):
            for k in range(3):
                screen_np_view[i, j, k] = screen[i][j][k]

    return screen_np


cdef bytes save_bytes():
    cdef void* savestate = save()
    if savestate is NULL:
        raise MemoryError("Failed to create savestate")
    
    # Convert void* to Python bytes for safe storage
    cdef size_t state_size = get_state_size()
    py_savestate = (<char*>savestate)[:state_size]
    free_state(savestate)
    return py_savestate


cdef void load_bytes(bytes savestate):
    # Ensure savestate is the correct size
    cdef size_t expected_size = get_state_size()
    if len(savestate) != expected_size:
        raise ValueError(f"Savestate must be exactly {expected_size} bytes")

    load(<const void*>(<const char*>savestate))


cdef class Celeste:
    def __cinit__(self):
        init()

    def step(self, unsigned short action):
        step(action)

    def get_info(self):
        cdef PlayerState p = get_player_state()
        cdef unsigned char room = get_room()

        return {
            "spd_x": p.spd_x,
            "spd_y": p.spd_y,
            "x": p.x,
            "y": p.y,
            "grace": p.grace,
            "jbuffer": p.jbuffer,
            "djump": p.djump,
            "dash_time": p.dash_time,
            "room": room,
            "deaths": p.deaths,
            "fruits": p.fruits,
            "minutes": p.minutes,
            "seconds": p.seconds + (1 / 30) * p.frames
        }

    def save(self):
        return save_bytes()

    def load(self, savestate: bytes):
        load_bytes(savestate)

    def get_state_size(self):
        return get_state_size()

    def get_screen(self) -> np.ndarray:
        return get_screen_as_np()

    def set_render_enabled(self, bool enabled):
        set_render_enabled(enabled)
