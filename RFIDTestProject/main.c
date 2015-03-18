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

//Pin Declarations
static const GlobalPin DIN = { GPIO_C, io_PC7, 0x8, Polar_ActiveHigh }; //C7
static const GlobalPin DOUT = { GPIO_C, io_PC6, 0x7, Polar_ActiveHigh }; //C6
static const GlobalPin DSYNC = { GPIO_C, io_PC5, 0x6, Polar_ActiveHigh }; //C5
static const GlobalPin CK = { GPIO_C, io_PC4, 0x5, Polar_ActiveHigh }; //C4
static const GlobalPin MODE = { GPIO_D, io_PD2, 0x20, Polar_ActiveHigh }; //D2
static const GlobalPin RTB = { GPIO_D, io_PD3, 0x30, Polar_ActiveHigh }; //D3
static const size_t kMsPerCycle = 1; //Figure out how to change this to microseconds

//
void dataHigh()
{
	globalPin_write(ON, &DIN);
}
void dataLow()
{
	globalPin_write(OFF, DIN);
}

//
void configMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(OFF, &MODE);
}
void receiveMode()
{
	globalPin_write(ON, &RTB);
	globalPin_write(ON, &MODE);
}

void transmitMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(ON, &MODE);

}
void tModDelay()
{
	transmitMode;
	//look up function for wait in XINC2
}
int startConfig()
{

	// This function sets which pins are input and which pins are output. 
	// Needs pin numbers for Xinc2 and input/output for transceiver

	globalPin_set_dir(PinDir_Output, &MODE);
	globalPin_set_dir(PinDir_Output, &RTB);
	globalPin_set_dir(PinDir_Output, &DIN);
	globalPin_set_dir(PinDir_Input, &CK);
	globalPin_set_dir(PinDir_Input, &DOUT);
	globalPin_set_dir(PinDir_Input, &DSYNC);

	//dataHigh();
	//configMode();

	//figure out how to change to majority voting mode.

	//tModeDelay;
	//configMode();

	//Edit Other THings the same way
	dataLow();
}
//

//

 
static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh },  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }, 0 };

void init()
{
	startConfig();
	globalPin_set_dir(PinDir_Output, &btTx);
	io_set_config(DEFAULT_IO_CFG, &btTx);
	
	globalPin_set_dir(PinDir_Input, &btRx);
	io_set_config(DEFAULT_IO_CFG, &btRx);
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
