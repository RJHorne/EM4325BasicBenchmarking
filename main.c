#include <msp430.h> 
#include <intrinsics.h>
#include "spi.h"

#define EM4325_CS BIT0
extern volatile unsigned char rxIncoming;
unsigned char counterData = 0;
unsigned char toggle = 0;
unsigned char frame =0;

int main(void)
{
    extern volatile unsigned char RXData[bufferSize];
    extern volatile unsigned char rxIncoming;

	WDTCTL = WDTPW | WDTHOLD;
	PM5CTL0 &= ~LOCKLPM5;

	spiClkInit();
	spiInit();

	P1SEL1 &= (~BIT3);
	__bis_SR_register(GIE);
	P3DIR |= EM4325_CS;
	P1DIR &= (~BIT3);
	
	for(;;){
	    if((P1IN & BIT3) != 0x00)
	    {
	        toggle = 0;
	        __delay_cycles(1000);
	    }

	    if((P1IN & BIT3) == 0x00 && toggle == 0)
	    {
	        while(frame < 8)
	        {
	            writeRF(frame,counterData);
	            frame++;
	            counterData++;
	        }
	    toggle = 1;
	  //  counterData++;
	    frame = 0;
	    }
	    else
	    {
	        __delay_cycles(1000);
	    }
	}
}



