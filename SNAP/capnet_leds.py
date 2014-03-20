# (c) Copyright, 2013, Synapse Wireless Inc.


from PCA9634 import *


led_indiv_enabled = False
led_blink_enabled = False


def enable_leds():
    global led_indiv_enabled

    pca9634_reset()
    pca9634_enable_normal_mode()
    pca9634_enable_individual_control()
    led_indiv_enabled = True

def set_led(led_num, pwm_level):
    global led_indiv_enabled

    if not led_indiv_enabled:
        pca9634_enable_individual_control()
        led_indiv_enabled = True
    pca9634_set_brightness(led_num, pwm_level)

def set_leds(level0, level1, level2, level3, level4, level5, level6, level7):
    global led_indiv_enabled

    if not led_indiv_enabled:
        pca9634_enable_individual_control()
        led_indiv_enabled = True
    pca9634_set_brightness(0, level0)
    pca9634_set_brightness(1, level1)
    pca9634_set_brightness(2, level2)
    pca9634_set_brightness(3, level3)
    pca9634_set_brightness(4, level4)
    pca9634_set_brightness(5, level5)
    pca9634_set_brightness(6, level6)
    pca9634_set_brightness(7, level7)

def blink_leds(freq, duty_cycle):
    global led_blink_enabled

    if not led_blink_enabled:
        pca9634_enable_blinking()
        led_blink_enabled = True

    if freq is None and duty_cycle is None:
        pca9634_disable_blinking
    else:
        pca9634_blink_frequency(freq)
        pca9634_duty_cycle(duty_cycle)

def set_all_leds(level):
    global led_blink_enabled

    if led_blink_enabled:
        pca9634_disable_blinking()
        led_blink_enabled = False

    pca9634_duty_cycle(level)

def set_leds_bulk(level_string):
    pca9634_set_all_indiv_brightness(level_string)
