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
#include "UART.h"

static const GlobalPin btTx =  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }; // J3
static const GlobalPin btRx = { GPIO_J, io_PJ, 2, Polar_ActiveHigh }; // J2

static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh },  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }, 0 };

void init()
{
	globalPin_set_dir(PinDir_Output, &btTx);
	globalPin_set_dir(PinDir_Input, &btRx);
}

int test = 0;

int main(void)
{
	init();
	
	while(1)
	{
		test += 1;
		if( test > 3)
		{
		uart_write_byte('a', &btPair);
		
		uint16_t in = uart_read_byte(&btPair);
		xpd_echo_int(test, XPD_Flag_SignedDecimal);
		
		test = 0;
		}
	}
	return 0;
}
