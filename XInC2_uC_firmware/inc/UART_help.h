//--------------------------------------------------------------------------------------
// File: UART_help.h
// Author: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------

#include "UART.h"

/* This function takes in a string, and outputs it to the
*  Uart buffer. It assumes that you have a standard null-
*  terminated C-style string.
*/
void uart_write_str(char* str, UART_Pin_Pair const * pinPair)
{
	int i = 0;
	while( str[i] != 0 )
	{
		uint16_t input = str[i];
		uart_write_byte(input, pinPair);
	}
}