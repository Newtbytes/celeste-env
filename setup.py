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
            "ccleste",
            sources=["celeste/ccleste.pyx", "celeste/ccleste/environment.cpp", "celeste/ccleste/celeste.cpp"],
            include_dirs=[".", "celeste/ccleste/", np.get_include()],
            define_macros=[("CELESTE_P8_FIXEDP", 1), ("CELESTE_P8_HACKED_BALLOONS", 1)],
            extra_compiler_args=["-O3", "-march=native", "-fPIC"],
            extra_linker_args=["-flto"],
            language="c++"
        )
    ]


setup(
    name="ccleste",
    packages=find_packages(),
    install_requires=["Cython", "gymnasium"],
    ext_modules=cythonize(get_extensions(), compiler_directives={"language_level": "3"}),
    package_data={
        "ccleste": ["*.pyx", "*.pxd", "ccleste/*.h", "ccleste/*.c", "ccleste/*.cpp"],
    },
    language="c++",
)
