/*
 * PWM1.c
 *
 * Created: 15/04/2024 01:44:55
 *  Author: lisare
 */ 
#include "PWM1.h"

int PWM1(uint8_t numeropot, uint8_t valoradc){
	uint16_t valortotal;
	if (numeropot == 1)
	{
		ADMUX = (ADMUX & 0xF0) | numeropot;
		ADCSRA |= (1<<ADSC);
		valortotal = (14.12*valoradc)+1198;//mapeo(valoradc, 0, 255, 19, 39); // Establece el ancho de pulso del servo
		OCR1AH = (valortotal & 0xFF00) >> 8;
		OCR1AL |= valortotal & 0x00FF;
	}
	else if (numeropot == 0)
	{
		ADMUX = (ADMUX & 0xF0) | numeropot;
		ADCSRA |= (1<<ADSC);
		OCR2A = (0.094*valoradc)+6.7;
		//while (ADCSRA & (1 << ADSC));
	}
	while (ADCSRA & (1 << ADSC));
	return ADC;
}