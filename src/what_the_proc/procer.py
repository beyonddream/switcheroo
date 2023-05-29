from what_the_proc.procer_load import (
    is_c_extension_loaded, PROCER_LIB)

if is_c_extension_loaded():

    def procer_get_name():
        name = PROCER_LIB.procer_get_name()
        return name
    
else:
     def procer_get_name():
          raise Exception("Unable to get process name!!!")