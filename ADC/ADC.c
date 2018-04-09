/*
 * ADC.c
 *
 * Created: 4/9/2018 3:10:44 PM
 * Author : andy
 */ 

#include <avr/io.h>
#include <stdlib.h>
#include <string.h>
#include "acx.h"
#include "acxserial.h"

int ADC_read(byte channel);
void setup_ADC();

int main(void)
{
	setup_ADC();
	char str[4];
	
	Serial_open(0, 115200L, SERIAL_8N1);
	
    while (1) 
    {
		itoa(ADC_read(0), str, 10);
		
		for(int i = 0; i < strlen(str); i++)
			Serial_write(0, str[i]);
		
		x_delay(200);
    }
}

int ADC_read(byte channel) {
	// Single-ended on ADC0 is default
	int num = 0;
	for(int i = 4; i > 0; i--) {
		ADCSRA |= (1<<ADSC); //start conversion
		num = ADCH;
		x_delay(5);
	}
	num >>= 2;  // number / 4
	return num;
}

void setup_ADC() {
	// Enable ADC. Prescale=16
	ADCSRA = (1<<ADEN)|(1<<ADPS2);
	ADMUX  = (1<<ADLAR)|(1<<REFS0);  // Left-adjust and REF = AVCC
}


