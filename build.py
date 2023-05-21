"""Build script for c extension building and installation."""

from setuptools import Extension

ext_modules = [

    Extension(
        "what_the_proc.ext.procer",
        sources=["./src/what_the_proc/ext/procer.c"],
        include_dirs=["./src/what_the_proc/ext/include"],
        extra_compile_args=["-O3"],
        optional=False,
    )
]

def build(setup_kwargs):
    """Build extension."""
    setup_kwargs.update({"ext_modules": ext_modules})