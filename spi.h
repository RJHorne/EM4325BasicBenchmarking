#ifndef SPI_H_
#define SPI_H_

#define bufferSize    8
#define spiDelayCycle 200
#define spiDelayRise 2000

void spiInit();
void spiWrite(unsigned char data);
unsigned char  spiRead();
void spiClkInit();
void writeRF(int wordNo, int data);

#endif
