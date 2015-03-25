//--------------------------------------------------------------------------------------
// File: uart_bluetooth.h
// Author: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------

#include <IOConfig.h>
#include <Structs.h>
#include <XPD.h>

#include "UART.h"
#include "struct_aliases.h"

#define btRx { GPIO_J, io_PJ, 0x8 , Polar_ActiveHigh } // J3
#define btTx { GPIO_J, io_PJ, 0x4 , Polar_ActiveHigh } // J2

static const UART_Pin_Pair btPair = { btTx, btRx , 49152000/9600 };

InputStream btStream;

void bluetooth_init()
{
	globalPin_set_dir(PinDir_Output, &btPair.tx);
	io_set_config(DEFAULT_IO_CFG, btPair.tx.io_port);
	
	globalPin_set_dir(PinDir_Input, &btPair.rx);
	io_set_config(DEFAULT_IO_CFG, btPair.rx.io_port);
	
	IS_Init(&btStream);
}

void uart_write_str(char* str, int len, UART_Pin_Pair const * pinPair)
{
	for(int i = 0; i < len; i++)
	{
		uint16_t input = str[i];
		uart_write_byte(input, pinPair);
	}
}

		// The code below is legacy test code
		// it should not be necessary for our 
		// project, but I want to leave it here
		// in case we have need of it again.
		
		// Code to test GlobalPin declarations
		/* 
		while(1)
		{
			xpd_puts("Writing 0\n");
			globalPin_write(0, &btTx);
			wait_ms(1000);
			xpd_puts("Writing 1\n");
			globalPin_write(1, &btTx);
			wait_ms(1000);
		}
		
		// Old Loop to test bluetooth functionality
		xpd_puts("Beginning Continuous Loop.\n");
		while(1)
		{	

			
			xpd_puts("Putting some bytes.\n");
			for( char c = 'a'; c <= 'z'; c++)
			{
				uart_write_byte(c, &btPair);
				wait_ms(10);
			}
			xpd_puts("Reading a byte.\n");
			char in = uart_read_byte(&btPair);
			xpd_puts("Echoing the value.\n");
			xpd_putc(in);
			xpd_puts("\n");
		}
		*/