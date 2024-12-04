from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step

env = CelesteEnv()

print(benchmark_step(env), "fps")


env.reset()

env.action_space.seed(42)

for i in range(1000):
    _, _, _, _, info = env.step(env.action_space.sample())

    # print last 5 info dicts to show that something is happening
    if i > 995:
        print(info)
