cdef extern from "environment.h":
    void init()
    void step(unsigned short action)
    void* save()
    void load(const void* savestate)
    size_t get_state_size()
    void free_state(void* savestate)


cdef class Celeste:
    def __cinit__(self):
        init()

    def step(self, unsigned short action):
        step(action)

    def save(self):
        cdef void* savestate = save()
        if savestate is NULL:
            raise MemoryError("Failed to create savestate")
        
        # Convert void* to Python bytes for safe storage
        cdef size_t state_size = get_state_size()
        py_savestate = (<char*>savestate)[:state_size]
        free_state(savestate)
        return py_savestate

    def load(self, savestate):
        # Ensure savestate is the correct size
        cdef size_t expected_size = get_state_size()
        if len(savestate) != expected_size:
            raise ValueError(f"Savestate must be exactly {expected_size} bytes")
        
        load(<const void*>savestate)

    def get_state_size(self):
        return get_state_size()
