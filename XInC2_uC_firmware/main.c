//--------------------------------------------------------------------------------------
// File: main.c
// Author: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------
#include <IOConfig.h>
#include <Structs.h>

#include "time_funcs.h"
#include "struct_aliases.h"
#include "UART.h"

static const UART_Pin_Pair btPair;

void init()
{
	btPair.rx = LED4;
	btPair.tx = LED5;
	btPair.timeout = 0;
}

int main(void)
{
	init();
	
	uart_write_byte('a', &btPair);
	
	uint16_t in = uart_read_byte(&btPair);
	xpd_echo(in);
	
	return 0;
}
