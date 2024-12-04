from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step
from gymnasium.utils.env_checker import check_env

env = CelesteEnv()

print(benchmark_step(env), "fps")

check_env(env)


env.reset()

env.action_space.seed(42)

for i in range(1000):
    state, reward, terminated, _, info = env.step(env.action_space.sample())

    # print last 4 info dicts to show that something is happening
    if i > 995:
        print(reward, terminated)
        print(info)
        print(state)
