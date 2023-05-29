from what_the_proc.procer_load import is_c_extension_loaded


def test_procer_load():
    assert is_c_extension_loaded() == True
