#include <avr/io.h>
#include <util/delay.h>
#include <util/setbaud.h>

void initUSART(void) {
	UBRR0H = UBRRH_VALUE;					    // BAUDrate setzen, wird via Makefile im
	UBRR0L = UBRRL_VALUE;						// terminal als Argument übergeben 
#if USE_2X
	UCSR0A |= (1 << U2X0);						// Divisor BAUDrate halbieren		
#else
	UCSR0A &= ~(1 << U2X0);
#endif
	UCSR0B = (1 << TXEN0) | (1 << RXEN0);		// Reveiver & Transmitter einschalten
	UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);     // 8 Start-, 1 Stop, kein Paritätsbit
}

void transmitByte(uint8_t data) {
	while ( !( UCSR0A & ( 1 << UDRE0 ) ) ) {}   // Solange Transmitpuffer nicht leer, warten
	UDR0 = data;
}

uint8_t receiveByte(void) {
	while ( !( UCSR0A & ( 1 << RXC0 ) ) ) {}   // Solang Empfang nicht fertig, warte
	return UDR0;
}

void printString(const char myString[]) {
	uint8_t i = 0;
	while (myString[i]) {						// Solange string != \0-character
		transmitByte(myString[i++]);
	}
}

int main(void) {
	char serialCharacter;
	DDRB = 0xff;								// Alle LEDs als Ausgang definieren
	initUSART();
	printString("Hello World!\r\n");

	while (1) {
		serialCharacter = receiveByte();
		transmitByte(serialCharacter);
		PORTB = serialCharacter;
	}
	return 0;
}
