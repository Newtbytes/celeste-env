import os
from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize


os.environ['CC'] = 'g++'


extensions = [
    Extension(
        "ccleste", 
        sources=["celeste/ccleste.pyx", "celeste/ccleste/environment.c", "celeste/ccleste/celeste.c"],
        include_dirs=[".", "celeste/ccleste/"],
        define_macros=[("CELESTE_P8_FIXEDP", 1), ("CELESTE_P8_HACKED_BALLOONS", 1)],
        extra_compile_args=["-Ofast"]
    )
]

setup(
    name="ccleste",
    packages=find_packages(),
    install_requires=["gymnasium"],
    ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}),
)