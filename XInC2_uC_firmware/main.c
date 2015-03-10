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

static const UART_Pin_Pair btPair = { LED4, LED5, 0 };

void init()
{

}

int main(void)
{
	init();
	
	uart_write_byte('a', &btPair);
	
	uint16_t in = uart_read_byte(&btPair);
	xpd_echo_int(in, XPD_Flag_UnsignedDecimal);
	
	return 0;
}
