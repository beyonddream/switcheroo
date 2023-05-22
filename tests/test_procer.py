
from what_the_proc.procer_load import (
    is_c_extension_loaded,
    procer_get_name
)

def test_procer_load():
    assert is_c_extension_loaded() == True

def test_procer_name():
    assert procer_get_name() == b"what_the_proc"