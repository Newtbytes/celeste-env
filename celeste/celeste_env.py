import gymnasium as gym
import numpy as np

from ccleste import Celeste


def uint_limit(bits):
    return 2 ** bits - 1

def level_index(x, y):
	return x%8+y*8


# TODO: implement binding for load_room
# Level index / room x,y should be converted to something intuitive like 1 for level 1, etc.
# TODO: implement room name in info dict
# TODO: implement single room goal
# TODO: implement rendering
# once image rendering is implemented, implement human render mode with pygame or something
class CelesteEnv(gym.Env):
    def __init__(self, terminate_on_death=False) -> None:
        super().__init__()

        self.terminate_on_death = terminate_on_death

        self.celeste = Celeste()
        self.initial_state = self.celeste.save()
        self.save_state = self.celeste.save()
        
        self._last_room = None
        self._last_info = {}
        self._max_y = float("-inf")

        self.observation_space = gym.spaces.Box(
            low=0,
            high=255,
            shape=(len(self.save_state),),
            dtype=np.uint8,
        )
        self.action_space = gym.spaces.Discrete(uint_limit(6) + 1)

    def _obs(self):
        return np.frombuffer(self.save_state, dtype=np.uint8)
    
    def _reward(self, info, room):
        reward = 5 if room != self._last_room else 0

        reward += (info["fruits"] - self._last_info["fruits"]) * 2
        reward -= (info["deaths"]  - self._last_info["deaths"])

        # TODO: check if this is correct. y axis may be flipped, so this may be incorrect.
        if self._max_y == info["y"] and self._last_info["y"] < self._max_y:
            reward += (info["y"] - self._last_info["y"]) * 0.1

        reward += max(info["spd_y"] - self._last_info["spd_y"], 0)

        return reward

    def _done(self, info, room):
        done = room == 30

        if self.terminate_on_death:
            done = info["deaths"] > 0

        return done

    def reset(self, seed=None, **kwargs):
        super().reset(seed=seed)

        self.load(self.initial_state)
        self.save_state = self.celeste.save()

        info = self.celeste.get_info()
        self._last_room = level_index(info["room_x"], info["room_y"])
        self._last_info = info
        self._max_y = max(self._max_y, info["y"])

        return self._obs(), info

    def step(self, action: int):
        self.celeste.step(action)
        self.save_state = self.celeste.save()

        info = self.celeste.get_info()
        room = level_index(info["room_x"], info["room_y"])

        if room == self._last_room:
            self._max_y = max(self._max_y, info["y"])
        else:
            self._max_y = info["y"]


        reward = self._reward(info, room)
        terminated = self._done(info, room)


        self._last_room = room
        self._last_info = info

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
