
from what_the_proc.procer import (
    procer_get_name,
    procer_start_process_listener
)

if __name__ == '__main__':
    try:
        name = procer_get_name()
        print(f"The current process name is {name}")
        procer_start_process_listener()
        print(f"exiting...")
    except Exception as e:
        print(f"Exception occured while calling procer_get_name - {e}")