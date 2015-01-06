#ifndef wiimote_h

#include <string.h>
#include <avr/io.h>
#include <util/delay.h>
#include <util/twi.h>
#include <avr/interrupt.h>

#define twi_port PORTC
#define twi_ddr DDRC
#define twi_scl_pin 5
#define twi_sda_pin 4

#define dev_detect_port PORTD
#define dev_detect_ddr DDRD
#define dev_detect_pin 4

// initialize wiimote interface with id, starting data, and calibration data
void wm_init(unsigned char *, unsigned char *, unsigned char *, void (*)(void));

// set button data
void wm_newaction(unsigned char *);

#define wiimote_h
#endif
