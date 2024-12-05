from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step
from gymnasium.utils.env_checker import check_env

env = CelesteEnv()

print(benchmark_step(env), "fps")

check_env(env)


env.reset()

env.action_space.seed(42)

for f in range(200):
    state, reward, terminated, truncated, info = env.step(env.action_space.sample())

    if reward != 0:
        print("frame:", f)
        print("reward:", reward)
        print("info:", info)
        print()

    if terminated or truncated:
        env.reset()
