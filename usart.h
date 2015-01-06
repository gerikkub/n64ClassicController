#ifndef _USART_H_
#define _USART_H_

void initUSART();
void USART_Write(char);
void USART_WriteStr(char*);
void printHex8(unsigned char a);
void printHex16(unsigned int a);
void printEndl();

#endif
