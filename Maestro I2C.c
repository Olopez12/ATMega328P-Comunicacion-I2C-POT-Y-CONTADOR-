#define F_CPU 16000000
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include "LCD/LCD8.h"
#include "I2C/I2C.h"
#include "UART/UART.h"
#include <stdlib.h>

#define slave1 0x30
#define slave2 0x31

uint8_t direccion;
uint8_t temp;
uint8_t bufferI2C;

char CONV[8];
uint8_t bufferI2C;

volatile uint8_t counterValue; // Valor del contador del segundo esclavo

int main(void) {
	I2C_Init();
	sei(); // Habilitar interrupciones globales
	InLCD();

	while (1) {
		
		LCD_cursor(1,1);
		LCD_W_String("Val Pot ");
		LCD_cursor(10,1);
		LCD_W_String("Cont");
		
		I2C_Start();
		I2C_Write(0x10); 
		I2C_Start();
		I2C_Write(0x11); 
		uint8_t adcValue = I2C_Read_NACK();
		I2C_Stop();
		
		float voltaje = adcValue * (5.0 / 255.0); // ADC de 8 bits (0-255)
		
		// Mostrar valor del ADC en LCD
		dtostrf(voltaje, 4, 2, CONV);  // Convertir voltaje a string con 2 decimales
		LCD_cursor(1,2);
		LCD_W_String(CONV);
		
		I2C_Start();
		I2C_Write(0x12);
		I2C_Start();
		I2C_Write(0x13); 
		uint8_t counter_value = I2C_Read_NACK();
		I2C_Stop();
		
		LCD_clr(10,2);
		LCD_clr(11,2);
		LCD_clr(12,2);
		LCD_cursor(10,2);
		
		itoa(counter_value, CONV, 10);
		LCD_W_String(CONV);
		
		_delay_ms(1000); // Esperar antes de la siguiente lectura
	}
}