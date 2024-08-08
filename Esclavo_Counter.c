/*
 * EsclavoCounter.c
 *
 * Created: 2/08/2024 00:00:24
 * Author : Usuario
 */ 

#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "I2C/I2C.h"

#define SlaveAddress 0x09

volatile uint8_t counter = 0;

void debounce() {
	_delay_ms(50); // Retraso para el antirrebote
}

ISR(PCINT0_vect) {
	if (!(PINB & (1<<PINB0))) { // Si el botón PB0 está presionado
		debounce(); // Antirrebote
		if (!(PINB & (1<<PINB0))) {
			counter = (counter + 1) & 0x0F; // Incrementar contador (4 bits)
		}
	}
	if (!(PINB & (1<<PINB1))) { // Si el botón PB3 está presionado
		debounce(); // Antirrebote
		if (!(PINB & (1<<PINB1))) {
			counter = (counter - 1) & 0x0F; // Decrementar contador (4 bits)
		}
	}
	PORTD = (PORTD & 0xF0) | (counter & 0x0F); // Mostrar valor del contador en PD0-PD3
}


ISR(TWI_vect) {
	switch (TWSR & 0xF8) {
		case 0x60: // Direcci?n recibida y ACK
		case 0x70: // Direcci?n general recibida y ACK
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xA8: 
		TWDR = counter; // Enviar el valor del contador
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xB8: // Datos enviados y ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		case 0xC0: // Datos enviados y NACK recibido
		case 0xC8: // ?ltimo byte enviado y ACK recibido
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK
		break;
		default:
		TWCR |= (1 << TWINT) | (1 << TWEA); // Responder con ACK por defecto
		break;
	}
}






void initCounter() {
	DDRD = 0x0F; // Configurar PD0-PD3 como salida
	PORTD = 0x00; // Apagar los LEDs inicialmente

	DDRB &= ~((1<<PINB0) | (1<<PINB1)); // Configurar PB0 y PB3 como entrada
	PORTB |= (1<<PINB0) | (1<<PINB1); // Habilitar pull-up resistors en PB0 y PB3

	PCICR |= (1<<PCIE0); // Habilitar interrupciones de cambio de pin para PCINT0
	PCMSK0 |= (1<<PCINT0) | (1<<PCINT1); // Habilitar interrupciones para PB0 y PB3
}

int main(void) {
	initCounter();
	
	I2C_Slave_Init(SlaveAddress);
	sei(); // Habilitar interrupciones globales

	while (1) {
		// El ciclo principal no necesita hacer nada, todo está manejado por las ISR
	}
}