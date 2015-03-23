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

static const GlobalPin btTx =  { GPIO_J, io_PJ, 0x8 ,  Polar_ActiveHigh }; // J3
static const GlobalPin btRx = { GPIO_J, io_PJ, 0x4 , Polar_ActiveHigh }; // J2

static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh },  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }, 9600 };

InputStream btStream;

void init()
{

	globalPin_set_dir(PinDir_Output, &btTx);
	io_set_config(DEFAULT_IO_CFG, btTx.io_port);
	
	globalPin_set_dir(PinDir_Input, &btRx);
	io_set_config(DEFAULT_IO_CFG, btRx.io_port);
	
	IS_Init(&btStream);
	
	xpd_puts(STRING_INIT_COMPLETE);
}

int main(void)
{
	init();
	
	
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
		char message[] = "abcd";
		
		xpd_puts("Putting a byte.\n");
		uart_write_byte('1', &btPair);
		uart_write_str(message,&btPair);
		xpd_puts("Reading a byte.\n");
		uint16_t in = uart_read_byte(&btPair);
		xpd_puts("Echoing the value.\n");
		xpd_echo_int(in, XPD_Flag_SignedDecimal);

	}
	
	// New Loop to actually parse the data from the app
	while(1)
	{
		char in = uart_read_byte(&btPair);
		IS_Insert( in , &btStream );
		
		// upon string completion, check for a command
		// if a command is found, perform the action
		// return the same input string back to the bt
		// module as confirmation of command success
		if( in == 0 ) 
		{
			xpd_puts("Command Identified.\n");
			
			if( IS_ContainsString(DELETE_A, &btStream) )
			{
				xpd_puts("Deleting A\n");
				// DELETE A ACTION
				uart_write_str(DELETE_A,&btPair);
			}
			else if( IS_ContainsString(DELETE_B, &btStream) )
			{
				xpd_puts("Deleting B\n");
				// DELETE B ACTION
				uart_write_str(DELETE_B,&btPair);
			}
			else if( IS_ContainsString(CONSOL_A, &btStream) )
			{
				xpd_puts("Consolidating A\n");
				// CONSOLIDATE A ACTION
				uart_write_str(CONSOL_A,&btPair);
			}
			else if( IS_ContainsString(CONSOL_B, &btStream) )
			{
				xpd_puts("Consolidating B\n");
				// CONSOLIDATE B ACTION
				uart_write_str(CONSOL_B,&btPair);
			}
			else if( IS_ContainsString(SWAP, &btStream) )
			{
				xpd_puts("Swapping\n");
				// SWAP ACTION
				uart_write_str(SWAP,&btPair);
			}
			else if( IS_ContainsString(REFRESH, &btStream) )
			{
				xpd_puts("Refreshing\n");
				// REFRESH ACTION
				uart_write_str(REFRESH,&btPair);
			}
			
			IS_Clear(&btStream);
		}
		
		
	}
	return 0;
}
