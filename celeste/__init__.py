from .celeste_env import CelesteEnv
from .ccleste import SaveState

from gymnasium.envs.registration import register

__all__ = ["CelesteEnv", "SaveState"]

register(
    id="CelesteClassic-v0",
    entry_point="celeste.celeste_env:CelesteEnv"
)