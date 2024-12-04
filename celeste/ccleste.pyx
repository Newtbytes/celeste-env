cdef extern from "environment.h":
    void init()

    void step(unsigned short action)

    void* save()
    void load(const void* savestate)

    size_t get_state_size()
    void free_state(void* savestate)

    cdef struct VEC:
        float x
        float y

    cdef struct PlayerState:
        VEC spd

        float x
        float y

        int grace
        int jbuffer
        int djump
        int dash_time

        bint was_on_ground

    cdef struct RoomState:
        int x
        int y

    PlayerState get_player_state()
    RoomState get_room_state()


cdef class Celeste:
    def __cinit__(self):
        init()

    def step(self, unsigned short action):
        step(action)

    def get_info(self):
        cdef PlayerState p = get_player_state()
        cdef RoomState r = get_room_state()

        return {
            "spd_x": p.spd.x,
            "spd_y": p.spd.y,
            "x": p.x,
            "y": p.y,
            "grace": p.grace,
            "jbuffer": p.jbuffer,
            "djump": p.djump,
            "dash_time": p.dash_time,
            "room_x": r.x,
            "room_y": r.y
        }

    def save(self):
        cdef void* savestate = save()
        if savestate is NULL:
            raise MemoryError("Failed to create savestate")
        
        # Convert void* to Python bytes for safe storage
        cdef size_t state_size = get_state_size()
        py_savestate = (<char*>savestate)[:state_size]
        free_state(savestate)
        return py_savestate

    def load(self, savestate: bytes):
        # Ensure savestate is the correct size
        cdef size_t expected_size = get_state_size()
        if len(savestate) != expected_size:
            raise ValueError(f"Savestate must be exactly {expected_size} bytes")
        
        load(<const void*>(<const char*>savestate))

    def get_state_size(self):
        return get_state_size()
