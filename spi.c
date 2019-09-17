#include <msp430.h>
#include "spi.h"


volatile unsigned char RXData[bufferSize];
volatile unsigned char rxIncoming =0;
#define EM4325_CS BIT0


void spiInit(){
    P1SEL1 |= BIT6 + BIT7;
    P2SEL1 |= BIT2;
    UCB0CTLW0 |= UCSWRST;
    UCB0CTLW0 |= UCMST+UCSYNC+UCCKPL+UCMSB;
    UCB0CTLW0 |= UCSSEL__ACLK;
    UCB0BR0 = 16;
    UCB0BR1 = 0;
    UCB0CTLW0 &= ~UCSWRST;
    UCB0IE |= UCRXIE;
}

void spiWrite(unsigned char data)
{
        while (!(UCB0IFG&UCTXIFG));
        UCB0TXBUF = data;
}

unsigned char spiRead()
{
    return UCB0RXBUF;
}

void spiClkInit()
{
    PJSEL0 |= BIT4 + BIT5;
    CSCTL0_H = 0xA5;
    CSCTL1 = DCOFSEL_6;
    CSCTL2 = SELA__LFXTCLK | SELS__DCOCLK | SELM__DCOCLK;
    CSCTL3 = DIVA__1 | DIVS__1 | DIVM__1;
    CSCTL4 &= ~LFXTOFF;
    do{
        CSCTL5 &= ~LFXTOFFG;
        SFRIFG1 &= ~OFIFG;
    }
    while (SFRIFG1&OFIFG);
    CSCTL0_H = 0;
}

void writeRF(int wordNo, int data)
{
    __delay_cycles(spiDelayCycle);
    P3OUT &= ~EM4325_CS;
    __delay_cycles(spiDelayCycle);
    spiWrite(0xEF);
    spiWrite(wordNo);
    spiWrite(data);
    spiWrite(data);
    __delay_cycles(spiDelayCycle);
    P3OUT |= EM4325_CS;
    __delay_cycles(spiDelayRise);
}


#if defined(__TI_COMPILER_VERSION__) || defined(__IAR_SYSTEMS_ICC__)
#pragma vector = USCI_B0_VECTOR
__interrupt void USCI_B0_ISR(void)
#elif defined(__GNUC__)
void __attribute__ ((interrupt(USCI_B0_VECTOR))) USCI_B0_ISR (void)
#else
#error Compiler not supported!
#endif
{
    if ((bufferSize - 1) < rxIncoming) rxIncoming = 0;
    RXData[rxIncoming] = UCB0RXBUF;
    rxIncoming++;
}


