#include <avr/io.h>

void initUSART() {

   UCSR0A = 0;
   UCSR0B = (1 << TXEN0);
   UCSR0C = (1 << UCSZ01)|(1 << UCSZ00);

   UBRR0H = 0;
   UBRR0L = 8;

}

void USART_Write(char a) {
   
   while(!(UCSR0A & (1 << UDRE0)));
   
   UDR0 = a;

}

void USART_WriteStr(char* str) {

   while(*str) {
      USART_Write(*str);
      str++;
   }
}

void printHex8(unsigned char a) {

   char lookup[] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

   USART_Write(lookup[a >> 4]);
   USART_Write(lookup[a & 0xF]);
}

void printHex16(unsigned int a) {

   printHex8(a >> 8);
   printHex8(a & 0xFF);

}

void printEndl() {
   USART_Write('\n');
   USART_Write('\r');
}
