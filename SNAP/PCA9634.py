# (c) Copyright, 2013, Synapse Wireless Inc.
"""Definitions and functions for the NXP PCA9634 8-bit Fm+ I2C LED Driver"""


PCA9634_REG_MODE1 = '\x00'
PCA9634_REG_MODE2 = '\x01'
PCA9634_REG_PWM0 = '\x02'
PCA9634_REG_PWM1 = '\x03'
PCA9634_REG_PWM2 = '\x04'
PCA9634_REG_PWM3 = '\x05'
PCA9634_REG_PWM4 = '\x06'
PCA9634_REG_PWM5 = '\x07'
PCA9634_REG_PWM6 = '\x08'
PCA9634_REG_PWM7 = '\x09'
PCA9634_REG_GRPPWM = '\x0A'
PCA9634_REG_GRPFREQ = '\x0B'
PCA9634_REG_LEDOUT0 = '\x0C'
PCA9634_REG_LEDOUT1 = '\x0D'
PCA9634_REG_SUBADR1 = '\x0E'
PCA9634_REG_SUBADR2 = '\x0F'
PCA9634_REG_SUBADR3 = '\x10'
PCA9634_REG_ALLCALLADR = '\x11'


def pca9634_reset():
    i2cWrite("\x06" + "\xa5\x5a", 1, False)

def pca9634_enable_normal_mode():
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_MODE1 + "\xEF", 1, False)  # Tell it to get out of sleep state

def pca9634_enable_sleep_mode():
    """Reduces power consumption from ~3mA to ~3uA"""
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_MODE1 + "\xFF", 1, False)

def pca9634_enable_individual_control():
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_LEDOUT0 + "\xFF", 1, False)
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_LEDOUT1 + "\xFF", 1, False)

def pca9634_set_brightness(led_num, brightness):
    if led_num <= 7 and led_num >= 0 and brightness >= 0 and brightness <= 255:
        i2cWrite(LED_I2C_ADDR + chr(led_num+2) + chr(brightness), 1, False)

def pca9634_set_all_indiv_brightness(brightness_string):
    i2cWrite(LED_I2C_ADDR + "\xA2" + brightness_string, 1, False)

def pca9634_enable_blinking():
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_MODE2 + "\x25", 1, False)

def pca9634_disable_blinking():
    i2cWrite(LED_I2C_ADDR + PCA9634_REG_MODE2 + "\x05", 1, False)

def pca9634_blink_frequency(rate):
    if rate >= 0 and rate <= 255:
        i2cWrite(LED_I2C_ADDR + PCA9634_REG_GRPFREQ + chr(rate), 1, False)

def pca9634_duty_cycle(rate):
    if rate >= 0 and rate <= 255:
        i2cWrite(LED_I2C_ADDR + PCA9634_REG_GRPPWM + chr(rate), 1, False)
