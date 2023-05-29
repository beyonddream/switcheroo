from what_the_proc.procer import procer_get_name

def test_procer_name():
    actual = procer_get_name()
    expected = "what_the_proc"
    assert actual.decode('utf-8') == expected