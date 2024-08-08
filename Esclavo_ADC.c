/*
 * EsclavoADC.c
 *
 * Created: 1/08/2024 23:59:59
 * Author : Usuario
 */ 

// esclavo ADC
#define F_CPU 16000000
#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <avr/interrupt.h>
#include "ADC/ADC.h"
#include "I2C/I2C.h"

#define SlaveAddress 0x08

volatile uint8_t ValorAdc = 0;


ISR(TWI_vect) {
	switch (TWSR & 0xF8) {
		case 0x60: // Direcci?n recibida
		case 0x70: // Direcci?n general recibida
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xA8: // Solicitud de datos del maestro
		TWDR = ADC_read(0); // Leer el valor
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xC0: // Datos enviados y NACK recibido
		case 0xC8: // ?ltimo byte enviado
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK por defecto
		break;
	}
}


int main(void)

{
	initADC();
	I2C_Slave_Init(0x08);
	SPCR |= (1<<SPIE);
	sei();
	
    /* Replace with your application code */
    while (1) 
    {
		
    }
}

