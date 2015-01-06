#ifndef pindef_h

#define twi_port PORTC
#define twi_ddr DDRC
#define twi_scl_pin 5
#define twi_sda_pin 4

#define dev_detect_port PORTD
#define dev_detect_ddr DDRD
#define dev_detect_pin 4

#define uart_port PORTD
#define uart_rx_pin 0

#define LED_pin 0
#define LED_port PORTB
#define LED_ddr DDRB

#define power_detect_pin 2
#define power_detect_port PORTD
#define power_detect_ddr DDRD
#define power_detect_input PIND

#define adc_port PORTC
#define adc_ddr DDRC

// for the bit numbers, go to
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

#define drum_port PORTC
#define drum_ddr DDRC
#define drum_in_reg PINC

#define bass_port PORTB
#define bass_ddr DDRB
#define bass_in_reg PINB

#define green_bit 4
#define green_pin 3

#define red_bit 6
#define red_pin 2

#define yellow_bit 5
#define yellow_pin 1

#define blue_bit 3
#define blue_pin 0

#define orange_bit 7
#define orange_pin 4

#define bass_bit 2

#define bass1_pin 1
#define bass2_pin 2

#define plusminus_port PORTA
#define plusminus_ddr DDRA
#define plusminus_in_reg PINA

#define stick_in_reg PINA
#define stick_port PORTA
#define stick_ddr DDRA

#define plus_bit 2
#define plus_pin 6

#define minus_bit 4
#define minus_pin 7

#define up_stick_pin 6
#define down_stick_pin 6
#define left_stick_pin 6
#define right_stick_pin 6

#define pindef_h
#endif
