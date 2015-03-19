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

static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh },  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }, 1 };

void init()
{
	globalPin_set_dir(PinDir_Output, &btTx);
	io_set_config(DEFAULT_IO_CFG, btTx.io_port);
	
	globalPin_set_dir(PinDir_Input, &btRx);
	io_set_config(DEFAULT_IO_CFG, btRx.io_port);
	
	xpd_puts("Initialization Complete.\n");
}

int main(void)
{
	init();
	
	xpd_puts("Beginning Continuous Loop.\n");
	for( int i = 0; i < 10; i++ )
	{

		xpd_puts("Putting a byte.\n");
		uart_write_byte('a', &btPair);
		xpd_puts("Reading a byte.\n");
		uint16_t in = uart_read_byte(&btPair);
		xpd_puts("Echoing the value.\n");
		xpd_echo_int(in, XPD_Flag_SignedDecimal);

	}
	return 0;
}
