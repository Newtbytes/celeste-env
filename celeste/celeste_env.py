import gymnasium as gym
import numpy as np

from ccleste import Celeste


def uint_limit(bits):
    return 2 ** bits - 1


# Level index / room x,y should be converted to something intuitive like 1 for level 1, etc.
# TODO: implement room name in info dict
# TODO: implement single room goal
# once image rendering is implemented, implement human render mode with pygame or something
class CelesteEnv(gym.Env):
    metadata = {"render_modes": ["rgb_array"], "render_fps": 30}

    def __init__(self, terminate_on_death=False, obs_type="rgb_array", render_mode="rgb_array") -> None:
        super().__init__()

        self.terminate_on_death = terminate_on_death
        self.obs_type = obs_type
        self.render_mode = render_mode

        assert obs_type in {"ram", "rgb_array"}

        self.celeste = Celeste()
        self.initial_state = self.celeste.save()
        self.save_state = self.celeste.save()
        self.screen = None

        self._last_room = None
        self._last_info = {}
        self._max_y = float("-inf")

        if self.obs_type == "ram":
            self.observation_space = gym.spaces.Box(
                low=0,
                high=255,
                shape=(len(self.save_state),),
                dtype=np.uint8,
            )

        elif self.obs_type == "rgb_array":
            self.observation_space = gym.spaces.Box(
                low=0,
                high=255,
                shape=(128,128,3),
                dtype=np.uint8
            )

        self.action_space = gym.spaces.Discrete(uint_limit(6) + 1)

    def _obs(self):
        if self.obs_type == "rgb_array":
            return self.get_screen()
        elif self.obs_type == "ram":
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
        self._last_room = info["room"]
        self._last_info = info
        self._max_y = max(self._max_y, info["y"])

        return self._obs(), info

    def render(self):
        if self.render_mode == "rgb_array":
            return self.get_screen()

    def step(self, action: int):
        self.screen = None

        self.celeste.step(action)
        self.save_state = self.celeste.save()

        info = self.celeste.get_info()
        room = info["room"]

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

    def get_screen(self):
        if self.screen is None:
            screen = self.celeste.get_screen()

            rgb_screen = np.zeros((128, 128, 3), dtype=np.uint8)

            rgb_screen[:, :, 0] = (screen >> 16) & 0xFF  # Red
            rgb_screen[:, :, 1] = (screen >> 8) & 0xFF   # Green
            rgb_screen[:, :, 2] = screen & 0xFF          # Blue

            self.screen = rgb_screen

        return self.screen


if __name__ == "__main__":
    from gymnasium.utils.performance import benchmark_step

    env = CelesteEnv()
    print(benchmark_step(env, target_duration=15), "steps per second")

    env.close()
