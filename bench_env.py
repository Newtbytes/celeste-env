from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step

env = CelesteEnv()
print(benchmark_step(env), "fps")
