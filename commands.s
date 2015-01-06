

DDRB = 0x4
PORTB = 0x5
PINB = 0x3

DDRD = 0xA
PORTD = 0xB
PIND = 0x9
PD3 = 3

DDRB = 0x4
PINB = 0x3
PB1 = 2
PB2 = 4

__SREG__ = 0x3f
__SP_H__ = 0x3e
__SP_L__ = 0x3d
__CCP__  = 0x34
__tmp_reg__ = 0
__zero_reg__ = 1

#n64DataStart = 0x200
.extern n64DataStart

.global inputDataBytes

inputDataBytes:
	.space 4

;Assume Z already PORTD, 2 cycles
.macro outputHigh
	ldi r25,0
	out DDRB,r25
.endm

;3 cycles
.macro outputLow
	ldi r25,2
	out DDRB,r25
.endm

;1 Cycle
.macro readData
	in r25,PINB 
.endm
	

;16 cycles
.macro delayMicro 
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
.endm

;8 cycles
.macro delayHalf 
	nop
	nop
	nop
	nop
	nop
	nop
	nop
	nop
.endm


;Waits until the n64 is finished sending a command
.global sync 
.global _Z4syncv

_Z4syncv:
sync:
		push r16
		;Should wait for 6 microseconds
		outputLow
		ldi	r16,0x5A	

syncLoop:
		;Check the signal line		
		readData
		andi	r25,PB1
		;if the signal line is low then jump back to sync
		breq	sync	

		;decrement the count
		dec	r16
		brne	syncLoop

		pop r16
		ret	


;reads 4 Bytes from the signal line
;Bytes stored in inputDataBytes
.global readConsoleData

readConsoleData:

		ldi r24,0x0
      ldi r27,0x8

readConsoleDataLoop:
      ldi   r26,64
readConsoleDataDown:
		readData
      dec r26
      breq readConsoleDataError
		andi	r25,PB1
		breq	readConsoleDataDown

		;Wait for the signal line to drop
      ldi r26,200 
readConsoleDataWaitUp:
		readData
      dec r26
      breq readConsoleDataError2
		andi	r25,PB1
		brne	readConsoleDataWaitUp

      

		;Delay 2.5 microseconds to align with the data
		delayMicro
		delayMicro
	

		readData			
		lsr		r25	
		andi	r25,0x1	
		lsl		r24
		or		r24,r25

		;Check for the end
		;3
		dec		r27		
		brne	readConsoleDataLoop

		ret

readConsoleDataError:
   #Impossible value to read
   ldi r24, 0x7D 
   ret

readConsoleDataError2:
   ldi r24, 0x7D
   ret

.global detectFallingEdge

detectFallingEdge:

detectFallingEdgeUp:
	readData
	andi	r25,0x2
	breq 	detectFallingEdgeUp

detectFallingEdgeDown:
	readData
	andi	r25,0x2
	brne	detectFallingEdgeDown

	ret


;Sends One byte to the n64 Console
;Byte stored in r24
.global sendByte
.global sendByteConsecutive

sendByte:

	outputHigh

	;Be sure that the signal is high
	delayMicro
	delayMicro

sendByteConsecutive:

	ldi r19,0x8
   mov r18,r24

sendByteLoop:
	andi 	r18,0x80	;1
	brne	sendByteOne	;1/2
	sendByteZero:
		;10 in
		nop
		nop
		nop
		outputLow

		delayMicro
		delayMicro
		delayHalf

		nop		;9
		nop		;10
		nop		;11
		nop		;12
		nop		;13

		outputHigh
		jmp	sendByteFinish	;2

	sendByteOne:
		;11 in
		nop
		nop
		outputLow
		delayHalf

		nop		;9
		nop		;10
		nop		;11
		nop		;12
		nop		;13

		outputHigh

		delayMicro
		delayMicro
		nop
		nop
		nop

	sendByteFinish:
	lsl	r24					;1
   mov r18,r24

	dec 	r19					;1
	breq	sendByteNoJmpBack	;1
	sendByteJmpBack:
	
	jmp sendByteLoop			;2
	
	sendByteNoJmpBack:

	;Technically not the end, but I wanted to leave some
	;leeway for future calls
	ret


.global sendStopBit

sendStopBit:

	outputLow
	delayMicro
	delayHalf
	nop
	nop
	nop
	nop
	nop
	outputHigh

	ret

.global sendFourBytes
sendFourBytes:

   push r18
   push r20
   push r22
   
   call sendByte
   
   pop r24
   call sendByteConsecutive
   
   pop r24
   call sendByteConsecutive

   pop r24
   call sendByteConsecutive

   call sendStopBit

   ret

.global getControllerData

getControllerData:

   #ldi r24, 0x01
   #call sendByte
   #call sendStopBit

   call readConsoleData
   push r24
   call readConsoleData
   push r24
   call readConsoleData
   push r24
   call readConsoleData
   mov r25, r24 
   pop r24
   pop r23
   pop r22

   ret
