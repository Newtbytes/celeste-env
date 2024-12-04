import gymnasium as gym
import numpy as np

from .ccleste import Celeste


def uint_limit(bits):
    return 2 ** bits - 1


class CelesteEnv(gym.Env):
    def __init__(self) -> None:
        super().__init__()

        self.celeste = Celeste()
        self.initial_state = self.celeste.save()
        self.save_state = self.celeste.save()

        self.observation_space = gym.spaces.Box(
            low=0,
            high=255,
            shape=(len(self.save_state),),
            dtype=np.uint8,
        )
        self.action_space = gym.spaces.Discrete(uint_limit(16) + 1)

    def reset(self, **_):
        self.load(self.initial_state)
        self.save_state = self.celeste.save()

        return self.save_state, {}

    def step(self, action: int):
        self.celeste.step(action)
        self.save_state = self.celeste.save()

        return np.array(self.save_state), 0, False, False, {}

    def save(self):
        return self.save_state[:]
    
    def load(self, savestate):
        self.celeste.load(savestate)


if __name__ == "__main__":
    from gymnasium.utils.performance import benchmark_step

    env = CelesteEnv()
    print(benchmark_step(env, target_duration=15), "steps per second")

    env.close()
