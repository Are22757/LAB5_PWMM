
#define F_CPU 16000000UL
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "PWM1/PWM1.h"



volatile uint16_t value     = 0;
volatile uint8_t valoradc = 0;
volatile uint8_t seguidor = 0;


void setupPWM1(void) {
	cli();
	// Configura los pines de los botones como entrada con pull-up
	UCSR0B = 0;
	//Configurando el pin 1 del puerto B como salida. (OC1A)
	DDRB |= (1<<DDB1);
	TCCR1A = 0;
	TCCR1B = 0;

	TCCR1A |= (1<<COM1A1);//Configuramos en modo rápido no invertido.
	// Configuramos el modo Fast para que llegue al OCR1A
	TCCR1A |= (1<<WGM11);//|(1<<WGM10);
	TCCR1B |= (1<<WGM13)|(1<<WGM12);
	
	TCCR1B |= (1<<CS11);
	//Escribo 39999 a ICR1 dividido entre H y L
	ICR1H = 0x9C;
	ICR1L = 0x3F;
	sei();
}

void setupPWM2(void) {
	cli();

	UCSR0B = 0;
	//Pin 1 del puerto B como salida
	DDRB |= (1<<DDB3);
	TCCR2A = 0;
	TCCR2B = 0;
	//Configuramos el modo y el prescaler.
	TCCR2A |= (1<<COM2A1);//Configuramos en modo rápido no invertido.
	
	TCCR2A |= (1<<WGM21)|(1<<WGM20);
	
	TCCR2B |= (1<<CS22)|(1<<CS21)|(1<<CS20);
	sei();
}
	
void InitADC1(void){
	ADMUX |= (1 << REFS0);    
	ADMUX &= ~(1 << REFS1);

	ADMUX |= (1 << ADLAR); 

	DIDR0 |= (1 << ADC0D);

	ADMUX &= ~(1 << MUX0); 
	ADCSRA = 0;

	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}

void InitADC2(void){
	ADMUX |= (1 << REFS0);   
	ADMUX &= ~(1 << REFS1);

	ADMUX |= (1 << ADLAR);

	DIDR0 |= (1 << ADC1D);

	ADMUX |= (1 << MUX0);
	ADCSRA = 0;

	ADCSRA |= (1 << ADIE);
	//Habilitamos el prescaler a 128.
	ADCSRA |= (1 << ADPS2)|(1 << ADPS1)|(1 << ADPS0);
	//Habilitamos el ADC
	ADCSRA |= (1 << ADEN);
	
	
}

int main(void) {
	setupPWM1(); // Inicializa todo.
	setupPWM2();
	InitADC1();
	InitADC2();
	//InitADC();//Configuro mi ADC.
	DDRB = 0xFF;
	ADCSRA |= (1<<ADSC);   //Habilito la interrupcion de ADC.
	while (1) {
		PWM1(1, valoradc);
		_delay_ms(10);
		//ADCSRA |= (1<<ADSC); 
		PWM1(0, valoradc);
		_delay_ms(10);
		
		
	}
	return 0;
}


//***************************************************************************************************************************************************************
// Interrupciones:
//***************************************************************************************************************************************************************

ISR(ADC_vect){
	//uint16_t valor_total;
	valoradc = ADCH;
	ADCSRA |= (1<<ADIF); // Apaga la bandera de interrupción del ADC

}