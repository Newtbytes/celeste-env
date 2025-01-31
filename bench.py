from celeste import CelesteEnv

from time import time

import gymnasium as gym


BENCH_DURATION = 30
SEED = 42

env = CelesteEnv(render_mode=None, obs_type="ram")


# modified version of benchmark step that doesn't include env.action_space.sample() in the timing
def benchmark_step(env: gym.Env, target_duration: int = 5, seed=None) -> float:
    """A benchmark to measure the runtime performance of step for an environment.

    example usage:
        ```py
        env_old = ...
        old_throughput = benchmark_step(env_old)
        env_new = ...
        new_throughput = benchmark_step(env_old)
        slowdown = old_throughput / new_throughput
        ```

    Args:
        env: the environment to benchmarked.
        target_duration: the duration of the benchmark in seconds (note: it will go slightly over it).
        seed: seeds the environment and action sampled.

    Returns: the average steps per second.
    """
    start_t = time()
    total_t = 0
    steps = 0
    env.reset(seed=seed)
    env.action_space.sample()

    while True:
        action = env.action_space.sample()

        t0 = time()
        _, _, terminal, truncated, _ = env.step(action)
        t1 = time()

        total_t += t1 - t0
        steps += 1

        if terminal or truncated:
            env.reset()

        if time() - start_t > target_duration:
            break

    return steps / total_t


def bench_env():
    return benchmark_step(env, target_duration=BENCH_DURATION, seed=SEED)


if __name__ == "__main__":
    print(bench_env(), "fps")
