import os
import glob
import subprocess

from ctypes import (c_char_p, util)

import numpy as np
import structlog

__all__ = ["is_c_extension_loaded", "procer_get_name"]

LOG = structlog.get_logger()

app_path = subprocess.check_output(['git', 'rev-parse', '--show-toplevel'])
app_path = app_path.decode().strip()
build_rpath = "build"
extension_rpath = "what_the_proc/ext"

so_path = glob.glob(os.path.join(app_path, build_rpath, "*", extension_rpath))
so_file = os.path.basename(*glob.glob(os.path.join(*so_path, '*.so')))

C_EXTENSION_LOADED = False

def is_c_extension_loaded():
    return C_EXTENSION_LOADED

try:
    # load the library using numpy
    PROCER_LIB = np.ctypeslib.load_library(so_file, *so_path)

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