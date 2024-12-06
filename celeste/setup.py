import os
from setuptools import setup, Extension
from Cython.Build import cythonize


os.environ['CC'] = 'g++'


extensions = [
    Extension(
        "ccleste", 
        sources=["ccleste.pyx", "ccleste/environment.c", "ccleste/celeste.c"],
        include_dirs=[".", "ccleste/"],
        define_macros=[("CELESTE_P8_FIXEDP", 1), ("CELESTE_P8_HACKED_BALLOONS", 1)],
        extra_compile_args=["-Ofast"]
    )
]

setup(
    name="ccleste",
    ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}),
)