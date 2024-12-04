from setuptools import setup, Extension
from Cython.Build import cythonize

extensions = [
    Extension(
        "ccleste", 
        sources=["ccleste.pyx", "ccleste/environment.c", "ccleste/celeste.c"],
        include_dirs=[".", "ccleste/"],
        extra_compile_args=["-Ofast"]
    )
]

setup(
    name="ccleste",
    ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}),
)