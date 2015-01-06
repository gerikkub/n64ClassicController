F_CPU = 16000000

default:
	avr-gcc -mmcu=atmega328p -DF_CPU=$(F_CPU) -o main.elf --std=c99 -O2 main.c commands.s usart.c wiimote.c
	avr-objcopy -O ihex main.elf main.hex
	avr-size main.elf

clean:
	rm -rf *.elf *.o *.hex

d:
	avrdude -p atmega328p -P /dev/ttyACM0 -c avrispv2 -F -u -U flash:w:main.hex
