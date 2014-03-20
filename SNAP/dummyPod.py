""" ---------- CapNet Lesson 3-------------------- 
    All the functionality of the many on-board sensors and actuators
    
    Tap the hat to turn on all LEDs and run the motor for 2 seconds
    Intercept node output to view print statements about the system.
    
"""    
from synapse.switchboard import *
from capnet_hw import *
from capnet_leds import *
from PodDummy_helpers import *
#from pod_func import *              #includes all the pods comands.

#global variables
PORTAL_ADDRESS = "\x00\x00\x01" # For immediate debug/stdout
NET_ID = "\x01\x90"
NET_GROUP_ALL = "\xFF\xFF"

# globals   
led_toggle = False    # Track the current LED state
speed_set = 0         # used for limiting virtual speed
          
loca = 0              # store the pods current location
side = 0              # for left and right merge
data =  0             # store the info about that location

ticks = 0             # used to determine when new nodes are entered

path1 = (
    #Name info
    "S1,\x02",
    "M1R,\x0F",
    "F2,\x0F",
    "M2L,\x0F",
    "F3,\x0F", 
    "M3L,\x0A", 
    "F4,\x0F", 
    "M4L,\x0F", 
    "F1,\x0F", 
    "M1L,\x0A",
)

path2 =(
    #Name info
    "S2,\x02",
    "M3R,\x0F",
    "F4,\x0F",
    "M4R,\x0A",
    "F1,\x0F", 
    "M1L,\x0A", 
    "F2,\x0F", 
    "M2R,\x0A", 
    "F3,\x0F", 
    "M3L,\x0A",
)

dummy_path = 0
i_path = 0
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
    global speed_set, ticks
    speed_set = speed_set+1
    if speed_set >=2: # pod advances in position every 3 seconds
        speed_set=0
        ticks=ticks-1
        pathing()
        printGlobals()
@setHook(HOOK_100MS)
def tick100ms():
    """Event Handler: 100 millisecond tick"""
    pass
def setPath(select):
    global dummy_path
    if select == 1: #set to path 1
        dummy_path = path1
    elif select ==2:    #set to path 2
        dummy_path = path2
    if dummy_path:  #if a path was assigned
        info = dummy_path[i_path]   #get next direction instructions
        parsePath(info)             #parse the data and update globals
def parsePath(info):#called when a new node is entered
    n = len(info)
    global data, loca, side,ticks
    print "info:",info[0:2]
    if n>4:
        loca=info[0:2]
        side=info[2]
        data=ord(info[4])
    else:
        loca=info[0:2]
        side = 0
        data=ord(info[3])
    ticks=data
def pathing():
    global ticks, dummy_path, i_path
    if not(dummy_path):     #check if it has been assigned a path
        print "no path selected"
        return
          
    if ticks <= 0: #you're on a new track node
        i_path = i_path + 1
        
        if i_path == len(dummy_path):
            i_path = 2
            
        info = dummy_path[i_path]
        parsePath(info) 
        run_led_pattern()
def printGlobals():
    print "loca: ",loca
    print "side: ",side
    print "data: ",data
    print "ticks: ",ticks
    print "i_path: ",i_path