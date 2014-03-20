"""CapNet base hardware definitions"""

# Enable light and battery sensors - active low
IO_SNS_EN = 31

# Light sensor ADC input
ADC_PHOTO_SNS = 4
IO_PHOTO_SNS = 28

# Battery monitor ADC input
ADC_BATT_SNS = 5
IO_BATT_SNS = 29

# Accelerometer interrupt outputs
IO_MOTION_IRQ1 = 10
IO_MOTION_IRQ2 = 23

# Enable LED controller - active low
IO_LED_EN = 37
LED_I2C_ADDR = "\xE2"

# Temperature sensor interrupt output
IO_TMP_IRQ = 21

# Motor driver enable - active high
IO_MOT_OFF = 15

# Motor driver PWM inputs (H-bridge)
IO_MOT_IN1 = 19    # OC3A
IO_MOT_IN2 = 20    # OC3B

# Global ADC variables
batt_level = 0
light_level = 0

def init_hw_io():
    """Initialize CapNet hardware to sane defaults"""
    setPinDir(IO_SNS_EN, True)
    setPinDir(IO_PHOTO_SNS, False)
    setPinDir(IO_BATT_SNS, False)
    setPinDir(IO_MOTION_IRQ1, False)
    setPinDir(IO_MOTION_IRQ2, False)
    setPinDir(IO_LED_EN, True)
    setPinDir(IO_TMP_IRQ, False)
    setPinDir(IO_MOT_OFF, True)
    setPinDir(IO_MOT_IN1, True)
    setPinDir(IO_MOT_IN2, True)
    
    writePin(IO_SNS_EN, True)
    writePin(IO_LED_EN, True)
    writePin(IO_MOT_OFF, False)
    writePin(IO_MOT_IN1, False)
    writePin(IO_MOT_IN2, False)
    
    i2cInit(True)
    read_batt_light_levels()
    
def read_batt_light_levels():
    """Read battery monitor and light sensor; update global variables"""
    global batt_level, light_level
    # Enable sensor inputs
    writePin(IO_SNS_EN, False)
    # Precharge ADC, and allow time to stabilize
    readAdc(ADC_BATT_SNS)
    readAdc(ADC_PHOTO_SNS)
    # Read ADC values into globals
    batt_level = avgAdc(ADC_BATT_SNS, 10)
    light_level = avgAdc(ADC_PHOTO_SNS, 10)
    # Disable sensor inputs
    writePin(IO_SNS_EN, True)

def avgAdc(ch, n):
    sum = 0
    i = 0
    while i < n:
        sum += readAdc(ch)
        i += 1
        
    return sum / n
    
def batt_mv():
    """Convert battery ADC reading to millivolts"""
    # ADC reference is 1.6v. Voltage divider gives 1023 => 3.2v; volts = batt_level * 3.2/1023
    batt_cnt_scale32 = batt_level * 32
    batt_decivolts = batt_cnt_scale32 / 1023
    batt_millivolts = ((batt_cnt_scale32 % 1023) * 32) / 330
    batt_millivolts += batt_decivolts * 100
    return batt_millivolts

def led_en(doEnable):
    writePin(IO_LED_EN, not doEnable)

