#ifndef macros_h

/* old macros */

// Macros
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit)) // clear bit
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit)) // set bit
#define tog(sfr, bit) (_SFR_BYTE(sfr) ^= _BV(bit)) // toggle bit
#define wdr() __asm__ __volatile__ ("wdr") // watchdog reset
#define nop() __asm__ __volatile__ ("nop") // no operation

#define macros_h
#endif
