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
#include "string_defines.h"
#include "InputStream.h"
#include "uart_bluetooth.h"
#include "MLX.h"

static const MLX_Pin_Set tagA = 
{
	TAGA_DIN,
	TAGA_DOUT,
	TAGA_DSYNC,
	TAGA_CK,
	TAGA_MODE,
	TAGA_RTB,
};

void init()
{
	init_clock();
	bluetooth_init();
	
	xpd_puts(STRING_INIT_COMPLETE);
}

int main(void)
{
	init();
	
	MLX_Initialize(&tagA);
	while(1)
	{
		MLX_Transmit_Mode(&tagA);
	}
	
	// New Loop to actually parse the data from the app
	while(1)
	{
		// We read 4 bytes at a time, because
		// I'm lazy and don't want to read 
		// through the multi-threading example
		char in;
		for(int i = 0; i < 4; i++)
		{
			in = uart_read_byte(&btPair);
			IS_Insert( in, &btStream );
		}
	
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
				xpd_puts("A Has Been Deleted.");
			}
			else if( IS_ContainsString(DELETE_B, &btStream) )
			{
				xpd_puts("Deleting B\n");
				// DELETE B ACTION
				xpd_puts("B Has Been Deleted.");
			}
			else if( IS_ContainsString(CONSOL_A, &btStream) )
			{
				xpd_puts("Consolidating A\n");
				// CONSOLIDATE A ACTION
				xpd_puts("Consolidated to A.\n");
			}
			else if( IS_ContainsString(CONSOL_B, &btStream) )
			{
				xpd_puts("Consolidating B\n");
				// CONSOLIDATE B ACTION
				xpd_puts("Consolidated to B.\n");
			}
			else if( IS_ContainsString(SWAP, &btStream) )
			{
				xpd_puts("Swapping\n");
				// SWAP ACTION
				xpd_puts("Swap complete.\n");
			}
			else if( IS_ContainsString(REFRESH, &btStream) )
			{
				xpd_puts("Refreshing\n");
				// REFRESH ACTION
						uart_write_str("da2\n", 4, &btPair);
						uart_write_str("db2\n", 4, &btPair);
				xpd_puts("Refresh complete.\n");
			}
			
			IS_Clear(&btStream);
		}		
	}
	return 0;
}