""" ---------- Dummy pod-------------------- 
Author: Trent Smith
    Date: 3/19/14

    
"""    
from synapse.switchboard import *
from capnet_hw import *
from capnet_leds import *
from pod_dum_func import *
#from pod_func import *              #includes all the pods comands.

#global variables
PORTAL_ADDRESS = "\x00\x00\x01" # For immediate debug/stdout
NET_ID = "\x01\x90"
NET_GROUP_ALL = "\xFF\xFF"


# ---------- SNAP Hook Handlers ------------------------

@setHook(HOOK_STARTUP)
def start():
    crossConnect(DS_STDIO, DS_TRANSPARENT)
    ucastSerial(PORTAL_ADDRESS)
    setRate(3)
    init_hw_io()
    led_en(True)
    enable_leds()
    setPath(1)      #change value to one or two  to choose a path


@setHook(HOOK_1S)
def tick1s():
    """Event Handler: One second tick"""
    go()

@setHook(HOOK_100MS)
def tick100ms():
    """Event Handler: 100 millisecond tick"""
    pass
