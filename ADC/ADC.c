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
void printVoltage(char[4], char[4]);

int main(void)
{
	setup_ADC();
	
	x_init();
	
	Serial_open(0, 115200L, SERIAL_8N1);
	char vol[4];
	char hex[4];
	
    while (1) 
    {
		int result = ADC_read(0);		
		itoa(result, vol, 10);
		itoa(result, hex, 16);
		
		printVoltage(vol, hex);
		
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
	ADCSRA = (1<<ADEN)|(1<<ADPS2);  // Enable ADC & Prescale = 16
	ADMUX  = (1<<ADLAR)|(1<<REFS0);  // Left-adjust & REF = AVCC
}

void printVoltage(char voltage[4], char hex[4]) {
	int length = strlen(voltage);
	char *label = "0 volts(0X";
	
	//add decimal
	for(int i = length; i > 0; i--)
		voltage[i + 1] = voltage[i];
	voltage[1] = '.';
	
	//print voltage in decimal
	for(int i = 0; i < strlen(voltage); i++)
		Serial_write(0, voltage[i]);
	//print the voltage label
	for(int i = 0; i < strlen(label); i++)
		Serial_write(0, label[i]);
	//print voltage in hex surrounded by parens
	for(int i = 0; i < strlen(hex); i++)
		Serial_write(0, hex[i]);
	Serial_write(0, ')');
	
	Serial_write(0, '\n');
}
