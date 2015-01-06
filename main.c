/*
 * Code originally from Frank Zhao's wii drum controller project. This file was heavily modified.
 */

#include "main.h"
#include <util/delay.h>

#include "usart.h"

#define N64_BUT_A 0x80
#define N64_BUT_B 0x40
#define N64_BUT_Z 0x20
#define N64_BUT_START 0x10
#define N64_BUT_L 0x2000
#define N64_BUT_R 0x1000

#define N64_PAD_UP 0x8
#define N64_PAD_DOWN 0x4
#define N64_PAD_LEFT 0x2
#define N64_PAD_RIGHT 0x1

#define N64_CAM_UP 0x800
#define N64_CAM_DOWN 0x400
#define N64_CAM_LEFT 0x200
#define N64_CAM_RIGHT 0x100

#define CLASSIC_BUT_A 0x10
#define CLASSIC_BUT_B 0x40
#define CLASSIC_BUT_X 0x08
#define CLASSIC_BUT_Y 0x20
#define CLASSIC_BUT_RT 0x02
#define CLASSIC_BUT_LT 0x20
#define CLASSIC_BUT_ZR 0x04
#define CLASSIC_BUT_ZL 0x80
#define CLASSIC_BUT_PLUS 0x04
#define CLASSIC_BUT_MINUS 0x10

#define CLASSIC_PAD_UP 0x01
#define CLASSIC_PAD_DOWN 0x40
#define CLASSIC_PAD_LEFT 0x02
#define CLASSIC_PAD_RIGHT 0x80

#define BUTTON_MAP(n64But, classicBut, classicByte) \
if(n64Data & (n64But)) { \
   classicData[(classicByte)] &= ~(classicBut); \
} else { \
   classicData[(classicByte)] |= (classicBut); \
}

// most of this data is found on
// http://wiibrew.org/wiki/Wiimote/Extension_Controllers

const unsigned char drum_id[6] = {0x00, 0x00, 0xA4, 0x20, 0x01, 0x01};

// calibration data
const unsigned char cal_data[32] = {
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00,
	0x00, 0x00, 0x00, 0x00
};

unsigned long int getControllerData();
unsigned char readConsoleData();
void sendData(unsigned char);
void sendStopBit();
volatile int n64DataInterrupted;

void dummyFunc() {};

void convertN64ToClassic(unsigned long int n64Data, unsigned char* classicData) {
   
   BUTTON_MAP(N64_BUT_A, CLASSIC_BUT_A, 5);
   BUTTON_MAP(N64_BUT_B, CLASSIC_BUT_X, 5);
   BUTTON_MAP(N64_BUT_R, CLASSIC_BUT_RT, 4);
   BUTTON_MAP(N64_BUT_Z, CLASSIC_BUT_ZL, 5);
   BUTTON_MAP(N64_BUT_START, CLASSIC_BUT_PLUS, 4);
   BUTTON_MAP(N64_BUT_L, CLASSIC_PAD_RIGHT, 4);
   BUTTON_MAP(N64_CAM_DOWN, CLASSIC_BUT_B, 5);
   BUTTON_MAP(N64_CAM_LEFT, CLASSIC_BUT_Y, 5);

   unsigned char analog = ((n64Data >> 16) + 0x40);
   analog = (analog > 0xE0) ? 0 : ((analog > 0x7F) ? 0x7F : analog);
   analog >>= 1;
   classicData[0] = analog;

   analog = ((n64Data >> 24) + 0x40);
   analog = (analog > 0xE0) ? 8 : ((analog > 0x7F) ? 0x78 : analog);
   analog >>= 1;
   classicData[1] = analog;

   classicData[2] = 0x10;
   classicData[0] |= 0x80;
}

void convertN64ToClassicSmash(unsigned long int n64Data, unsigned char* classicData) {
   
   BUTTON_MAP(N64_BUT_A, CLASSIC_BUT_A, 5);
   BUTTON_MAP(N64_BUT_B, CLASSIC_BUT_B, 5);
   BUTTON_MAP(N64_BUT_R, CLASSIC_BUT_ZR, 5);
   BUTTON_MAP(N64_BUT_Z, CLASSIC_BUT_RT, 4);
   BUTTON_MAP(N64_BUT_START, CLASSIC_BUT_PLUS, 4);

   if(n64Data & (N64_CAM_UP | N64_CAM_DOWN | N64_CAM_RIGHT | N64_CAM_LEFT)) {
      classicData[5] &= ~(CLASSIC_BUT_X);
   } else {
      classicData[5] |= CLASSIC_BUT_X;
   }

   unsigned char analog = ((n64Data >> 16) + 0x40);
   analog = (analog > 0xE0) ? 0 : ((analog > 0x7F) ? 0x7F : analog);
   analog >>= 1;
   classicData[0] = analog;

   analog = ((n64Data >> 24) + 0x40);
   analog = (analog > 0xE0) ? 8 : ((analog > 0x7F) ? 0x78 : analog);
   analog >>= 1;
   classicData[1] = analog;

   //classicData[0] = (n64Data >> 18) & 0x3F;
   //classicData[1] = (n64Data >> 26) & 0x3F;

}

int main()
{
   volatile char reason = MCUSR;
   MCUSR = 0;
   initUSART();

   DDRD |= (1 << 5);
   DDRD &= ~(1 << 7);
   PORTD |= (1 << 7);
   DDRD &= (1 << 5);

   unsigned char classicData[6] = {0,0,0,0,0xFF,0xFF};

	// make wiimote think this is a classic controller
	wm_init(drum_id, classicData, cal_data, dummyFunc);

   unsigned int i = 0;

   USART_WriteStr("Start: ");
   printEndl();

   while(1) {

      sendByte(0x01);
      sendStopBit();
      unsigned long int n64Data = getControllerData();
      int valid = 1;
      
      for(i = 0; i < 4; i++) {
         if(((n64Data >> (i * 8)) & 0xFF) == 0x7D) {
            valid = 0;
         }
      }
      if(valid) {
         printHex8((n64Data >> 0) & 0xFF);
         printHex8((n64Data >> 8) & 0xFF);
         printHex8((n64Data >> 16) & 0xFF);
         printHex8((n64Data >> 24) & 0xFF);
         printEndl();
         if(PIND & (1 << 7)) {
            convertN64ToClassicSmash(n64Data, classicData);
         } else {
            convertN64ToClassic(n64Data, classicData);
         }
      }

      wm_newaction(classicData);

      _delay_ms(12);

   }

	return 0;
}


#ifdef trig_on_fall
#ifdef trig_on_rise
#error "cannot define both trig_on_fall and trig_on_rise"
#endif
#endif
