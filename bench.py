from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step


BENCH_DURATION = 30
SEED = 42

env = CelesteEnv()


def bench_env():
    return benchmark_step(env, target_duration=BENCH_DURATION, seed=SEED)


if __name__ == "__main__":
    print(bench_env(), "fps")
