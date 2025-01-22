from celeste import CelesteEnv

from gymnasium.utils.env_checker import check_env


SEED = 42


env = CelesteEnv()


def test_spec():
    check_env(env)

def test_tas():
    env.reset()

    with open("tests/test-tas.txt") as f:
        tas = f.read().split(',')
        tas = [int(a) for a in tas if len(a) > 0]

    rooms = set()

    for f, a in enumerate(tas):
        _, _, terminated, truncated, info = env.step(a)
        if (f+1) == 1:
            assert info["room"] == 0

        room = info["room"]

        rooms.add(room)

        if terminated or truncated:
            env.reset()

    env.close()

    assert(len(rooms) == 31)
    assert(rooms == set(i for i in range(31)))
