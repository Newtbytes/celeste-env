from ccleste import Celeste

import numpy as np


def test_save_load():
    savestate = Celeste().save()
    assert len(savestate) > 0

    original_celeste = Celeste()
    loaded_celeste = Celeste()
    loaded_celeste.load(original_celeste.save())

    assert original_celeste.get_info() == loaded_celeste.get_info()

def test_save_load_integration():
    celeste = Celeste()
    init_state = celeste.save()

    for _ in range(10):
        celeste.step(0)

        save_state = celeste.save()

        assert len(init_state) == len(save_state)

        celeste.load(init_state)
        assert celeste.save() == init_state

        celeste.load(save_state)
        assert celeste.save() == save_state

def test_get_screen():
    screen = Celeste().get_screen()
    assert isinstance(screen, np.ndarray)
    assert screen.shape == (128, 128, 3)

def test_get_screen_and_load():
    celeste = Celeste()
    init_state = celeste.save()

    for _ in range(10):
        celeste.step(np.random.randint(0, 256))

    save_state = celeste.save()

    celeste.load(init_state)

    for _ in range(10):
        celeste.step(np.random.randint(0, 256))

    celeste.load(save_state)

    celeste.step(0)

    screen = celeste.get_screen()

    assert np.unique(screen).size > 1
    assert np.max(screen) > 0

def test_first_step_not_black():
    celeste = Celeste()

    screen = celeste.get_screen()

    assert np.unique(screen).size > 1
    assert np.max(screen) > 0

    celeste.step(0)

    screen = celeste.get_screen()

    assert np.unique(screen).size > 1
    assert np.max(screen) > 0


def test_step_action():
    celeste = Celeste()

    original_state = celeste.save()

    celeste.step(0)
    new_state = celeste.save()

    assert len(original_state) == len(new_state)
