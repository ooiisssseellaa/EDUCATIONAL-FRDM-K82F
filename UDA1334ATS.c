/*
 * UDA1334.c
 *
 *  Created on: 13 mag 2019
 *      Author: Alessio
 */

#include <UDA1334ATS.h>
#include "time.h"

#include "math.h"

float pi = 3.141592653589;


void UDA1334_init(void)
{
	SIM->SCGC6 |= (1 << 15); // Enable clock to I2S module

	PORTA->PCR[5] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX BCLK (transmitter bit clock)
	PORTA->PCR[12] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TXD (data out)
	PORTA->PCR[13] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX FS (transmitter frame sync, 10pF capacitor to ground)

//	PORTA->PCR[14] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S RX BCLK (receiver bit clock) (not used)
//	PORTA->PCR[15] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX RXD (data in) (not used)
//	PORTA->PCR[16] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for I2S TX RX FS (receiver frame sync) (not used)
//
//	PORTA->PCR[17] |= (1 << 10) | (1 << 9); // Set alternative 6 mode pin mux for Master Clock (not used, it's generated by IC)

	I2S0->TMR = 0; // enable all words in a frame

	I2S0->TCR2 &= ~((1 << 31) | (1 << 30)); // set asynchronous mode (TX is independent by RX, another SAI transmitter or receiver)
	I2S0->TCR2 |= (1 << 24) | (1 << 4); // Bit Clock internally generated so clock is an output, bus clock divided by? ---> for 44100 KHz Sample Rate of the signal input, 24 bit 44.1 kHz word, 2 channel, so Bit Clock must be 2116800 MHz: 75000000 / 2116800 = 35 , so division value = (DIV + 1) * 2 )

	I2S0->TCR3 |= (1 << 16); // Enable TX channel 1

	I2S0->TCR4 |= (1 << 0); // Frame Sync internally generated (Master Mode)
	I2S0->TCR4 |= (1 << 12) | (1 << 11) | (1 << 4) | (1 << 3) | (1 << 2) | (1 << 0); // 24 bit clocks frame sync length, MSB first, FSync internally generated
	I2S0->TCR4 |= (1 << 16); // Set frame with two data words (bit field is number of words - 1),
	I2S0->TCR4 |= (1 << 28); // FIFO continue on error

	I2S0->TCR5 |=  (1 << 28) | (1 << 27); // Length of word set as 24 bit (bit field is number of bits - 1)
	I2S0->TCR5 |= (1 << 20) | (1 << 19); // Length of word 0 set as 24 bit (bit field is number of bits - 1)
	I2S0->TCR5 |= (1 << 12) | (1 << 11); // First bit start at 24 index (MSB first transmitted)

	I2S0->TCSR |= (1 << 31) | (1 << 29) | (1 << 28); // Enable TX, TX enable in stop mode, TX is enable also in debug, enable bit clock
}

void UDA1334_sendData(musicalNote note, uint16_t numberOfSampleInASecond, uint32_t amplitude, uint16_t hold, waveForm wave)
{
	uint16_t freq = note;
	int32_t data[numberOfSampleInASecond];

	switch (wave)
	{
		case sinusoidal:

			for(uint16_t s = 0; s < (numberOfSampleInASecond); s++)
			{
				data[s] = (int32_t)((amplitude) * (sin((2 * pi * s * freq) / (numberOfSampleInASecond)) ) );
			}

			break;

		case square:

			for(uint16_t s = 0; s < (numberOfSampleInASecond); s++)
			{

			}

			break;

		case triangular:

			for(uint16_t s = 0; s < (numberOfSampleInASecond); s++)
			{

			}

			break;
	}

	for(uint16_t i = 0; i < hold; i++)
    {
    	while(!(I2S0->TCSR & (1 << 17)));

    	for(uint16_t s = 0; s < (numberOfSampleInASecond); s++)
    	{
    		for(int i = 0; i < 300; i++); // delay generated by dummy processor operations... otherwise, us interrupt on timer degrade transmission.

    		I2S0->TDR[0] = data[s] << 9;
    		I2S0->TDR[0] = data[s] << 9;
    	}
    }
}







