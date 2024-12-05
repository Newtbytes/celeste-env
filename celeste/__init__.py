from .celeste_env import CelesteEnv

from gymnasium.envs.registration import register

__all__ = [CelesteEnv]

register(id="CelesteClassic-v0", entry_point="celeste_env:CelesteEnv")