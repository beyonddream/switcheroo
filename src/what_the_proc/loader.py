import os

from ctypes import c_char_p

import numpy as np
import structlog

__all__ = ["is_c_extension_loaded", "procer_get_name"]

LOG = structlog.get_logger()

extension_rpath = "what_the_proc/ext"
so_path = os.path.dirname(os.path.abspath(__file__))+"/../../build/lib.macosx-10.9-x86_64-cpython-39/"+extension_rpath

C_EXTENSION_LOADED = False

def is_c_extension_loaded():
    return C_EXTENSION_LOADED

try:
    # load the library using numpy
    PROCER_LIB = np.ctypeslib.load_library("procer.cpython-39-darwin.so", so_path)

    # setup argument and response time for each of the public functions
    PROCER_LIB.procer_get_name.argtypes = [] # void
    PROCER_LIB.procer_get_name.restype = c_char_p

    def procer_get_name():
        name = PROCER_LIB.procer_get_name()
        if not name:
            raise Exception("procer_get_name returned empty process name!!!")
        return name

    C_EXTENSION_LOADED = True
except OSError:
    C_EXTENSION_LOADED = False
    LOG.warn("Unable to load c extension library!!!", loaded=False, so_path=so_path, exc_info=1)