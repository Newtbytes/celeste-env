import gymnasium as gym
import numpy as np

from .ccleste import Celeste


def uint_limit(bits):
    return 2 ** bits - 1

def level_index(x, y):
	return x%8+y*8


# TODO: implement single room goal
# TODO: implement reward for achieving new maximum height in current room, max height is reset once room changes
# TODO: implement rendering
# once image rendering is implemented, implement human render mode with pygame or something
class CelesteEnv(gym.Env):
    def __init__(self) -> None:
        super().__init__()

        self.celeste = Celeste()
        self.initial_state = self.celeste.save()
        self.save_state = self.celeste.save()
        self.last_room = None

        self.observation_space = gym.spaces.Box(
            low=0,
            high=255,
            shape=(len(self.save_state),),
            dtype=np.uint8,
        )
        self.action_space = gym.spaces.Discrete(uint_limit(6) + 1)

    def _obs(self):
        return np.frombuffer(self.save_state, dtype=np.uint8)
    
    def _reward(self, room):
        reward = 0
        if room > self.last_room:
            reward += room - self.last_room

        return reward

    def reset(self, seed=None, **kwargs):
        super().reset(seed=seed)

        self.load(self.initial_state)
        self.save_state = self.celeste.save()

        info = self.celeste.get_info()
        self.last_room = level_index(info["room_x"], info["room_y"])

        return self._obs(), info

    def step(self, action: int):
        self.celeste.step(action)
        self.save_state = self.celeste.save()

        info = self.celeste.get_info()

        room = level_index(info["room_x"], info["room_y"])

        reward = self._reward(room)
        terminated = room == 30 # TODO: verify this is the index of the final room

        self.last_room = room

        return self._obs(), reward, terminated, False, info

    def save(self):
        return self.save_state[:]
    
    def load(self, savestate):
        self.celeste.load(savestate)


if __name__ == "__main__":
    from gymnasium.utils.performance import benchmark_step

    env = CelesteEnv()
    print(benchmark_step(env, target_duration=15), "steps per second")

    env.close()
