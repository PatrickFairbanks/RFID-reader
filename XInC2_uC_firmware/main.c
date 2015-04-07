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
		MLX_Global_Write(0, 0, TRANSMIT_PERIOD, &tagA);
	//	MLX_Receive_Mode(&tagA);
	//	MLX_Global_Receive(&tagA);
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
				// DELETING B
				MLX_Tag_Write(&tagA,0);
				xpd_puts("A Has Been Deleted.");
			}
			else if( IS_ContainsString(DELETE_B, &btStream) )
			{
				xpd_puts("Deleting B\n");
				// DELETING A
				MLX_Tag_Write(&tagB,0);
				xpd_puts("B Has Been Deleted.");
			}
			else if( IS_ContainsString(CONSOL_A, &btStream) )
			{
				xpd_puts("Consolidating A\n");\
				// CONSOLDATE TO A ACTION
				int b = MLX_Tag_Read(&tagB);
				int o = MLX_Tag_Read(&tagA) + b;
				MLX_Tag_Write(&tagA, o);
				MLX_Tag_Write(&tagB, 0);
				xpd_puts("Consolidated to A.\n");
			}
			else if( IS_ContainsString(CONSOL_B, &btStream) )
			{
				xpd_puts("Consolidating B\n");
				// CONSOLIDATE TO B ACTION
				int b = MLX_Tag_Read(&tagB);
				int o = MLX_Tag_Read(&tagA) + b;
				MLX_Tag_Write(&tagA, 0);
				MLX_Tag_Write(&tagB, o);
				xpd_puts("Consolidated to B.\n");
			}
			else if( IS_ContainsString(SWAP, &btStream) )
			{
				xpd_puts("Swapping\n");
				// SWAP ACTION
				int a = MLX_Tag_Read(&tagA);
				int b = MLX_Tag_Read(&tagB);
				MLX_Tag_Write(&tagA, b);
				MLX_Tag_Write(&tagB, a);
				xpd_puts("Swap complete.\n");
			}
			else if( IS_ContainsString(REFRESH, &btStream) )
			{
				xpd_puts("Refreshing\n");
				// REFRESH ACTION
				int a = MLX_Tag_Read(&tagA);
				int b = MLX_Tag_Read(&tagB);
				char stra[4] = "da2\n";
				char strb[4] = "db2\n";
				stra[3] = a;
				strb[3] = b;
				uart_write_str(stra, 4, &btPair);
				uart_write_str(strb, 4, &btPair);
				xpd_puts("Refresh complete.\n");
			}
			
			IS_Clear(&btStream);
		}		
	}
	return 0;
}