""" ---------- Dummy pod helper functions-------------------- 
Author: Trent Smith
    Date: 3/19/14
    
 All the functionality of the on-board LEDS
    
""" 
from capnet_leds import *

# Globals  
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

#------------  Path Functions  ----------------
def go():
    global speed_set, ticks
    speed_set = speed_set+1
    if speed_set >=2: # pod advances in position every 3 seconds
        speed_set=0
        ticks=ticks-1
        pathing()
        printGlobals()
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
# ----------  Helper functions ----------------

    
def fixed_pt_str(val, n):
    """Return str(val) with fixed decimal pt at position n"""
    # (432,3) => 0.432   (3210,3) => 3.210  (23,1) => 2.3
    s = str(val)
    i = len(s)
    pfx = "0.0000000"
    return pfx[:n+2-i] + s if i <= n else s[:-n] + '.' + s[-n:]       

# ----------  LED Helper functions ----------------
    
# LED pattern: Columns are LEDs, rows comprise sequence
led_p1 = (
    #   0   1   2   3   4   5   6   7
    "\x80\x00\x00\x00\x00\x00\x00\x00",
    "\x00\x80\x00\x00\x00\x00\x00\x00",
    "\x00\x00\x80\x00\x00\x00\x00\x00",
    "\x00\x00\x00\x80\x00\x00\x00\x00",
    "\x00\x00\x00\x00\x80\x00\x00\x00",
    "\x00\x00\x00\x00\x00\x80\x00\x00",
    "\x00\x00\x00\x00\x00\x00\x80\x00",
    "\x00\x00\x00\x00\x00\x00\x00\x80",
    "\x00\x00\x00\x00\x00\x00\x00\x00",
)

led_blink = (
    #   0   1   2   3   4   5   6   7
    "\x80\x80\x80\x80\x80\x80\x80\x80",
    "\x00\x00\x00\x00\x00\x00\x00\x00",
    "\x00\x00\x00\x00\x00\x00\x00\x00",
    "\x00\x00\x00\x00\x00\x00\x00\x00",
)

led_pattern = led_p1
i_ledpat = 0
    
def set_led_pat(pattern, repeat):
    global led_pattern, led_repeat, i_ledpat
    
    led_pattern = pattern
    led_repeat = repeat
    i_ledpat = 0
    
def run_led_pattern():
    global i_ledpat, led_pattern
    
    if not led_pattern:
        return
    
    pat = led_pattern[i_ledpat]
    set_leds_bulk(pat)

    i_ledpat = i_ledpat + 1
    if i_ledpat == len(led_pattern):
        i_ledpat = 0