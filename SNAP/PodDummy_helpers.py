""" ---------- CapNet Lesson 3 (helper functions)-------------------- 
    All the functionality of the on-board sensors and actuators
    
    Tap the hat to turn on all LEDs and run the motor for 2 seconds
    Intercept node output to view print statements about the system.
    
""" 
from capnet_leds import *

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