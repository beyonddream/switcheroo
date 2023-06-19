from what_the_proc.procer import procer_get_name, procer_start_process_listener

def test_procer_name():
    actual = procer_get_name()
    expected = "what_the_proc"
    assert actual.decode('utf-8') == expected


def test_procer_start_process_listener():
    procer_start_process_listener()
    