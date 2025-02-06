import os

from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize
import numpy as np


os.environ["CC"] = "clang++"
os.environ["CXX"] = "clang++"


# TODO: manually compile cython extension
# TODO: reimplement PGO


def get_extensions():
    return [
        Extension(
            "celeste.ccleste.ccleste",
            sources=[
                "celeste/ccleste/ccleste.pyx",
                "celeste/ccleste/celeste.cpp",
                "celeste/ccleste/pico8.cpp",
                "celeste/ccleste/environment.cpp",
            ],
            include_dirs=[".", "celeste/ccleste/", "celeste/ccleste/data/", np.get_include()],
            define_macros=[("CELESTE_P8_FIXEDP", 1), ("CELESTE_P8_HACKED_BALLOONS", 1)],
            extra_compile_args=["-O3", "-march=native", "-fPIC"],
            extra_link_args=["-flto"],
            language="c++"
        )
    ]


setup(
    name="celeste",
    packages=find_packages(),
    install_requires=["Cython", "gymnasium", "numpy"],
    ext_modules=cythonize(get_extensions(), compiler_directives={"language_level": "3"}),
    package_data={
        "celeste": ["*.pyx", "*.pxd", "ccleste/*.h", "ccleste/*.c", "ccleste/*.cpp"],
    },
    language="c++",
)
