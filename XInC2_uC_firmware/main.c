//--------------------------------------------------------------------------------------
// File: main.c
// Author: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------
#include <IOConfig.h>
#include <Structs.h>
#include <XPD.h>

#include "time_funcs.h"
#include "struct_aliases.h"
#include "UART_help.h"
#include "string_defines.h"
#include "InputStream.h"

#define btRx { GPIO_J, io_PJ, 0x8 , Polar_ActiveHigh } // J3
#define btTx { GPIO_J, io_PJ, 0x4 , Polar_ActiveHigh } // J2

static const UART_Pin_Pair btPair = { btTx, btRx , 49152000/9600 };

InputStream btStream;

void init()
{
	init_clock();

	globalPin_set_dir(PinDir_Output, &btPair.tx);
	io_set_config(DEFAULT_IO_CFG, btPair.tx.io_port);
	
	globalPin_set_dir(PinDir_Input, &btPair.rx);
	io_set_config(DEFAULT_IO_CFG, btPair.rx.io_port);
	
	IS_Init(&btStream);
	
	xpd_puts(STRING_INIT_COMPLETE);
}

int main(void)
{
	init();
	
	// New Loop to actually parse the data from the app
	while(1)
	{
		// We read 4 bytes at a time, because
		// I'm lazy and don't want to read 
		// through the multi-threading example
		char in = uart_read_byte(&btPair);
		IS_Insert( in , &btStream );
		in = uart_read_byte(&btPair);
		IS_Insert( in , &btStream );
		in = uart_read_byte(&btPair);
		IS_Insert( in , &btStream );
		in = uart_read_byte(&btPair);
		IS_Insert( in , &btStream );
		
		// upon string completion, check for a command
		// if a command is found, perform the action
		// return the same input string back to the bt
		// module as confirmation of command success
		if( in == STR_TERMINAL ) 
		{
			xpd_puts("Command Identified.\n");
			
			if( IS_ContainsString(DELETE_A, &btStream) )
			{
				xpd_puts("Deleting A\n");
				// DELETE A ACTION
				uart_write_str(DELETE_A,4,&btPair);
				xpd_puts("A Has Been Deleted.");
			}
			else if( IS_ContainsString(DELETE_B, &btStream) )
			{
				xpd_puts("Deleting B\n");
				// DELETE B ACTION
				uart_write_str(DELETE_B,4,&btPair);
				xpd_puts("B Has Been Deleted.");
			}
			else if( IS_ContainsString(CONSOL_A, &btStream) )
			{
				xpd_puts("Consolidating A\n");
				// CONSOLIDATE A ACTION
				uart_write_str(CONSOL_A,4,&btPair);
				xpd_puts("Consolidated to A.\n");
			}
			else if( IS_ContainsString(CONSOL_B, &btStream) )
			{
				xpd_puts("Consolidating B\n");
				// CONSOLIDATE B ACTION
				uart_write_str(CONSOL_B,4,&btPair);
				xpd_puts("Consolidated to B.\n");
			}
			else if( IS_ContainsString(SWAP, &btStream) )
			{
				xpd_puts("Swapping\n");
				// SWAP ACTION
				uart_write_str(SWAP,4,&btPair);
				xpd_puts("Swap complete.\n");
			}
			else if( IS_ContainsString(REFRESH, &btStream) )
			{
				xpd_puts("Refreshing\n");
				// REFRESH ACTION
				uart_write_str(REFRESH,4,&btPair);
				xpd_puts("Refresh complete.\n");
			}
			
			IS_Clear(&btStream);
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
		
	}
	return 0;
}
