/*
 * Copyright (c) 2011-2013 Sebastian Himmler
 * All rights reserved.
 *
 * Permission to use, copy, modify, and distribute this software for any
 * purpose with or without fee is hereby granted, provided that the above
 * copyright notice and this permission notice appear in all copies.
 *
 * THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES
 * WITH REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF
 * MERCHANTABILITY AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR
 * ANY SPECIAL, DIRECT, INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES
 * WHATSOEVER RESULTING FROM LOSS OF MIND, USE, DATA OR PROFITS, WHETHER IN
 * AN ACTION OF CONTRACT, NEGLIGENCE OR OTHER TORTIOUS ACTION, ARISING OUT
 * OF OR IN CONNECTION WITH THE USE OR PERFORMANCE OF THIS SOFTWARE.
 */

#include <avr/io.h>
#include <avr/pgmspace.h>

#include "uart.h"

void uartInit(uint32_t baudrate)
{
	// Set uart baudrate register
	uint8_t ubrr  = ((F_CPU + baudrate * 8L) / (baudrate * 16L) - 1);
	UBRR0H = (unsigned int)(ubrr >> 8);
	UBRR0L = (unsigned int)(ubrr);

	// enable rx and tx for uart
	UCSR0B |= (1 << RXEN0) | (1 << TXEN0);

	// set mode asynch 8n1
	UCSR0C = (3 << UCSZ00);
}

unsigned char uartReceiveByte(void)
{
	while (!(UCSR0A & (1 << RXC0)));
	return UDR0;
}

void uartTransmitByte(unsigned char data)
{
	while (!(UCSR0A&(1 << UDRE0)));
	UDR0 = data;
}

// easy method for sending strings
void uartPutString(const char *str)
{
	unsigned char c;

	while ((c = *str++))
		uartTransmitByte(c);
}

// wirte data from programm memmory address
void uartPutString_P(const char *addr)
{
	unsigned char c;

	while((c = pgm_read_byte(addr++)))
		uartTransmitByte(c);
}
