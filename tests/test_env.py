import pytest

from celeste import CelesteEnv

from gymnasium.utils.env_checker import check_env


SEED = 42


@pytest.fixture
def env():
    yield CelesteEnv()


def test_spec(env):
    check_env(env)


def test_observation_spaces(env: CelesteEnv):
    env.obs_type = "rgb_array"
    obs, _ = env.reset()
    assert obs.shape == (128, 128,3)

    obs_type = "ram"
    env.obs_type = obs_type
    obs, _ = env.reset()
    assert len(obs) == len(env.save_state)

def test_reward_calculation(env: CelesteEnv):
    _, info = env.reset(seed=SEED)
    env._last_info = info.copy()

    env._last_info.update({"fruits": 0, "deaths": 0})
    env._last_room = 5

    info.update({"fruits": 1, "deaths": 0})
    current_room = 6

    reward = env._reward(info, current_room)
    assert reward > 0

    info.update({"fruits": 0, "deaths": 1})
    reward_with_death = env._reward(info, current_room)
    assert reward_with_death < 0

def test_tas(env: CelesteEnv):
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
