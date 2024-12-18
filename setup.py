import os
import subprocess
from setuptools import setup, find_packages, Extension
from Cython.Build import cythonize


os.environ["CC"] = "clang++"

BASE_COMPILE_ARGS = ["-Ofast", "-march=native", "-fPIC", "-ftree-vectorize", "-fno-semantic-interposition", "-flto"]
BASE_LINK_ARGS = ["-flto"]
PROF_ARGS = ["-fprofile-generate"]
PGO_ARGS = PROF_ARGS + ["-fprofile-use"]


def get_extensions(profiling: bool):
    # a lot of this is probably very unnecessary 
    args = PROF_ARGS if profiling else PGO_ARGS
    extra_compile_args = BASE_COMPILE_ARGS + args
    extra_link_args = BASE_LINK_ARGS + args

    if not profiling:
        for arg in PROF_ARGS:
            extra_compile_args.remove(arg)
            extra_link_args.remove(arg)

    os.environ["CFLAGS"] = ' '.join(extra_compile_args)
    os.environ["CXXFLAGS"] = ' '.join(extra_compile_args)

    return [
        Extension(
            "ccleste", 
            sources=["celeste/ccleste.pyx", "celeste/ccleste/environment.c", "celeste/ccleste/celeste.c"],
            include_dirs=[".", "celeste/ccleste/"],
            define_macros=[("CELESTE_P8_FIXEDP", 1), ("CELESTE_P8_HACKED_BALLOONS", 1)],
            extra_compiler_args=BASE_COMPILE_ARGS + ["-fprofile-generate" if profiling else "-fprofile-use"],
            extra_link_args=BASE_LINK_ARGS + ["-fprofile-generate" if profiling else "-fprofile-use"],
            language="C++"
        )
    ]

def run_setup(extensions):
    setup(
        name="ccleste",
        packages=find_packages(),
        install_requires=["gymnasium"],
        ext_modules=cythonize(extensions, compiler_directives={"language_level": "3"}, force=True),
    )


if __name__ == "__main__":
    # compile with profiling enabled
    run_setup(get_extensions(profiling=True))

    # run tests with profiling, gathering information for PGO
    os.environ["LLVM_PROFILE_FILE"] = "default.profraw"

    from test_env import run_tests
    run_tests()

    subprocess.call(["llvm-profdata", "merge", "-output=default.profdata", "default.profraw"])

    # compile with profile guided optimization
    run_setup(get_extensions(profiling=False))
