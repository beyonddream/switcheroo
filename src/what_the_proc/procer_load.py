import os
import glob
import subprocess

import ctypes
from ctypes import (c_char_p)

import numpy as np
from np.ctypeslib import ndpointer, Structure

import structlog

from typing import Any

__all__ = ["is_c_extension_loaded"]

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

class ProcessInfoNext(ctypes.Structure):
    _fields = [
        ("sle_next", ctypes.POINTER('ProcessInfoNext'))
    ]

class ProcessInfoHead(ctypes.Structure):
    _fields = [
        ("slh_first", ctypes.POINTER(ProcessInfoNext))
    ]

class ProcessInfos(Structure):
    _fields = [
        ("proc_info", ProcessInfoHead)
    ]


# C library for procer
PROCER_LIB: Any

try:
    # load the library using numpy
    PROCER_LIB = np.ctypeslib.load_library(so_file, *so_path)

    # setup argument and response type for each of the public functions
    
    # procer_get_name()
    PROCER_LIB.procer_get_name.argtypes = [] # void
    PROCER_LIB.procer_get_name.restype = c_char_p

    # procer_get_process_info_all()
    PROCER_LIB.procer_get_process_info_all.argtypes = [] # void
    

    C_EXTENSION_LOADED = True
except OSError:
    C_EXTENSION_LOADED = False
    LOG.warn("Unable to load c extension library!!!", loaded=False, so_path=so_path, exc_info=1)