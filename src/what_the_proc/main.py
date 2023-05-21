
from loader import (
    is_c_extension_loaded,
    procer_get_name
)

if __name__ == '__main__':
    if is_c_extension_loaded():
        name = procer_get_name()
        print(f"The current process name is {name}")
    else:
        print("Unable to get the current process name!!!")