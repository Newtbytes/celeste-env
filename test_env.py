from celeste import CelesteEnv

from gymnasium.utils.performance import benchmark_step
from gymnasium.utils.env_checker import check_env


tas = [0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,2,2,2,2,2,2,2,2,2,34,2,0,0,0,0,0,0,2,2,2,2,34,34,2,2,2,2,2,2,18,18,2,2,2,2,18,18,0,0,0,0,36,0,0,0,0,0,2,2,2,2,2,2,18,2,2,2,2,2,2,36,36,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,18,2,18,2,2,2,2,18,1,1,1,1,17,2,2,2,2,18,1,1,1,1,17,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,18,1,1,1,1,17,2,2,2,2,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,17,1,1,1,1,1,1,36,1,1,1,0,0,0,0,0,0,0,0,16,0,16,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,2,2,2,2,2,2,2,2,38,2,6,6,6,6,6,22,6,6,6,6,6,6,22,6,6,6,6,22,6,6,6,6,6,6,6,6,22,6,0,0,0,0,0,5,5,5,21,5,5,5,5,0,36,5,5,5,5,5,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,2,2,2,38,0,0,0,0,0,0,0,2,2,2,2,1,1,16,36,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,16,1,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,18,18,2,2,0,0,2,2,0,0,38,0,0,2,2,2,2,2,2,2,2,38,38,0,0,0,1,1,17,17,2,18,16,0,16,0,0,16,0,16,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,2,2,2,2,2,2,2,18,2,0,36,0,0,0,16,0,0,0,0,2,36,36,0,0,0,0,0,2,2,2,2,16,1,1,1,36,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,36,0,0,0,0,0,0,0,0,0,0,0,1,17,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,18,2,2,2,2,2,2,2,2,1,1,37,1,1,1,1,1,1,1,1,1,1,1,1,18,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,18,2,2,2,34,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,1,1,1,1,1,1,1,1,1,1,1,1,0,0,36,1,1,1,1,1,1,1,1,1,1,1,37,1,1,0,0,0,0,0,16,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,18,2,40,0,0,0,0,0,0,0,18,2,2,2,2,2,2,2,2,2,18,2,2,2,2,2,0,0,0,36,36,36,0,1,1,1,1,1,0,1,1,17,1,16,5,1,1,17,17,1,1,1,17,17,1,0,1,36,0,0,0,36,16,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,2,2,2,2,18,2,2,2,2,2,2,2,2,2,2,38,2,2,2,2,18,18,2,2,22,22,2,2,2,18,2,2,2,2,18,2,2,2,2,38,2,2,2,2,2,2,2,2,18,1,1,1,1,1,17,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,2,2,2,18,2,38,2,2,2,2,2,2,2,2,2,2,2,2,16,0,0,0,1,17,1,1,1,1,1,36,0,0,0,0,0,1,1,1,16,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,22,6,6,6,6,6,6,36,0,0,0,0,0,0,0,0,0,2,2,36,2,0,0,0,0,0,0,0,0,0,0,2,18,2,2,2,2,2,2,2,2,2,34,0,2,2,2,2,2,2,2,2,2,2,2,2,2,38,0,0,0,0,2,2,2,2,2,18,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,2,2,2,2,2,2,2,18,2,34,2,2,2,2,2,2,18,2,2,2,2,2,2,2,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,18,2,1,1,1,0,36,0,0,0,0,0,0,0,0,1,1,17,1,1,0,0,16,37,0,1,1,0,0,16,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,2,2,2,18,2,2,38,0,0,0,0,0,0,0,0,0,0,0,2,2,0,0,0,1,1,1,1,1,1,33,33,0,0,0,0,0,0,1,1,17,0,0,36,0,0,0,0,0,0,0,0,16,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,10,10,42,2,2,2,2,18,0,2,2,2,2,2,2,2,2,2,0,17,1,1,1,1,0,0,16,0,0,0,0,36,0,0,0,2,2,2,2,2,2,2,2,2,18,2,2,2,2,2,2,36,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,40,0,0,0,0,0,0,16,0,0,0,0,0,1,1,38,0,0,0,0,0,0,2,2,2,2,2,2,18,2,2,2,2,2,2,2,38,0,0,0,0,0,0,2,2,2,2,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,18,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,38,2,2,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,17,1,1,1,1,1,1,1,37,0,0,0,0,0,0,1,1,1,1,1,1,1,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,2,2,2,2,2,2,18,2,2,2,1,17,1,1,1,1,1,1,1,18,2,2,2,2,2,17,17,0,0,0,0,1,1,1,16,1,0,0,0,0,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,42,10,42,10,10,10,10,2,2,38,6,6,6,6,6,6,6,6,38,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,6,38,38,6,6,6,6,16,5,5,5,5,5,5,37,0,0,0,0,0,1,1,1,1,33,1,1,1,1,1,1,1,1,1,33,1,33,1,1,1,1,1,17,1,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,2,0,2,2,2,0,0,1,18,0,36,0,0,0,0,0,0,0,0,0,2,2,36,0,0,0,16,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,38,2,2,2,2,2,2,2,2,2,2,2,2,34,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,2,2,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,36,0,0,0,0,0,0,38,0,0,0,0,0,0,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,17,1,1,1,1,1,1,17,17,1,1,1,1,37,1,1,1,1,1,1,17,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,18,2,2,2,2,2,2,2,0,36,0,0,0,0,0,0,0,18,2,2,2,2,38,0,0,0,0,0,0,0,0,0,16,0,0,0,0,0,0,1,1,33,0,0,0,0,0,0,1,1,37,0,0,0,0,0,0,16,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,0,0,0,0,0,0,34,0,0,0,0,0,0,34,0,0,0,0,0,0,0,0,0,0,1,1,0,0,1,1,1,1,37,0,0,0,0,0,0,37,1,37,0,0,0,0,0,0,0,0,1,1,1,1,0,0,0,0,36,0,0,0,0,0,0,0,0,0,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,0,16,16,2,34,0,0,0,0,0,0,0,0,0,0,2,36,2,2,2,2,2,0,1,1,1,1,33,1,1,1,1,1,1,17,1,1,1,1,1,1,36,0,0,1,1,1,1,1,1,1,1,1,38,2,2,2,2,2,2,2,2,2,18,18,42,42,42,2,2,0,2,0,38,0,38,38,18,2,18,2,2,2,2,2,2,2,2,2,2,18,18,2,18,18,18,18,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,0,0,0,0,0,0,2,18,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,2,38,0,0,0,0,0,0,18,2,2,2,2,2,2,2,2,36,0,0,0,0,0,0,0,0,0,1,17,0,36,0,0,0,0,0,0,0,1,36,0,0,0,0,0,0,1,1,17,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,26,10,10,42,10,10,10,10,10,10,10,0,0,0,1,17,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,34,0,2,2,2,2,2,42,2,2,2,2,2,2,2,2,2,2,2,0,2,2,2,34,2,2,2,2,18,2,2,2,2,2,2,2,2,2,38,6,0,0,0,0,2,2,16,0,0,36,0,1,1,1,1,1,1,1,1,1,1,1,17,1,17,1,1,1,1,1,36,1,36,0,0,0,0,16,0,36,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,34,2,2,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,18,2,2,38,2,2,2,2,2,2,2,2,36,0,0,0,0,0,0,18,2,2,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,2,2,2,2,2,2,18,2,2,2,2,2,38,0,0,0,0,0,0,0,0,0,37,0,0,0,0,0,0,36,0,0,0,0,0,0,36,0,0,0,0,0,0,16,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,18,2,18,18,2,2,2,2,38,2,2,2,2,2,2,2,2,2,38,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,2,0,0,0,0,0,0,0,0,0,]

env = CelesteEnv()

BENCH_DURATION = 30


def level_index(x, y):
	return x%8+y*8

def test_tas():
    env.reset()

    rooms = set()

    for f, a in enumerate(tas):
        _, _, terminated, truncated, info = env.step(a)
        if (f+1) == 1:
            assert info["room_x"] == 0 and info["room_y"] == 0

        room = level_index(info["room_x"], info["room_y"])

        if room not in rooms:
            print(f, room)

        rooms.add(room)

        if terminated or truncated:
            env.reset()

def run_tests():
    check_env(env)
    test_tas()

    return benchmark_step(env, target_duration=BENCH_DURATION)


if __name__ == "__main__":
    print(run_tests(), "fps")
