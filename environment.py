import ctypes

celeste = ctypes.CDLL('./environment.so')

celeste.init.restype = None
celeste.reset.restype = None
celeste.step.argtypes = [ctypes.c_ushort]
celeste.step.restype = None

celeste.save.restype = ctypes.POINTER(ctypes.c_char)
celeste.load.argtypes = [ctypes.POINTER(ctypes.c_char)]
celeste.load.restype = None

celeste.get_state_size.restype = ctypes.c_size_t

celeste.free_state.argtypes = [ctypes.POINTER(ctypes.c_char)]
celeste.free_state.restype = None

celeste.close.restype = None

class Environment:
    def __init__(self):
        celeste.init()

    def reset(self):
        celeste.reset()

    def step(self, action: int):
        celeste.step(action)

    def save(self) -> bytes:
        size = celeste.get_state_size()
        raw_state = celeste.save()
        state = ctypes.string_at(raw_state, size)
        celeste.free_state(raw_state)
        return state

    def load(self, state: bytes):
        size = len(state)
        buf = ctypes.create_string_buffer(state, size)
        celeste.load(buf)

    def get_state_size(self) -> int:
        return celeste.get_state_size()

    def close(self):
        celeste.close()

# test
if __name__ == "__main__":
    env = Environment()
    env.reset()
    env.step(3)
    state = env.save()
    print(f"State saved: {state[:10]}")
    env.load(state)
    env.close()
