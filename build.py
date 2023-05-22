"""Build script for c extension building and installation."""
import platform

from setuptools import Extension

ext_modules = [

    Extension(
        name="what_the_proc.ext.procer",
        sources=["./src/what_the_proc/ext/procer.c"],
        include_dirs=["./src/what_the_proc/ext/include"],
        extra_link_args=[] if "Darwin" in platform.system() else ["-shared"],
        extra_compile_args=["-O3"],
        
        optional=False,
    )
]

def build(setup_kwargs):
    """Build extension."""
    print(f"{setup_kwargs}")
    setup_kwargs.update({"ext_modules": ext_modules})