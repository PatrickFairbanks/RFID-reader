//--------------------------------------------------------------------------------------
// File: MLX.h
// Authors: Andrew Coulthard and Patrick Fairbanks
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------

#include <IOConfig.h>
#include <XPD.h>

#include "time_funcs.h"

#define MLX_ADDRESS_SIZE 				4
#define MLX_DATA_SIZE 						8
#define NFC_ADDRESS_SIZE					8
#define NFC_QUARTER_DATA_SIZE		16

#define FIVE_US										246
#define ONE_HUNDRED_US					4195

static const int Config_Data = { 0x73, 0x01, 0x00, 0x0F, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x00 };

typedef struct MLX_Pin_Set 
{
	GlobalPin const DIN;
	GlobalPin const DOUT;
	GlobalPin const DSYNC;
	GlobalPin const CK;
	GlobalPin const MODE;
	GlobalPin const RTB;	
	
	static const int periodTicks;
} MLX_Pin_Set;

/* The following functions relate to serial data transfer,
*  for the data and clock lines of the transceiver
*/
void MLX_Data_High(MLX_Pin_Set &pinSet)
{
	globalPin_write(ON, &pinSet->DIN);
}
void MLX_Data_Low(MLX_Pin_Set &pinSet)
{
	globalPin_write(OFF, &pinSet->DIN);
}
void MLX_Clk_High(MLX_Pin_Set &pinSet)
{
	globalPin_write(ON, &pinSet->CK);
}
void MLX_Clk_Low(MLX_Pin_Set &pinSet)
{
	globalPin_write(OFF, &pinSet->CK);
}

void MLK_Config_Mode(MLX_Pin_Set &pinSet)
{
	globalPin_write(OFF, &pinSet->RTB);
	globalPin_write(OFF, &pinSet->MODE);
	xpd_puts("Configuration Mode Enabled.\n");
}

void MLX_Receive_Mode(MLX_Pin_Set &pinSet)
{
	globalPin_write(ON, &pinSet->RTB);
	globalPin_write(ON, &pinSet->MODE);
	globalPin_write(ON, &pinSet->DIN);
	globalPin_write(OFF, &pinSet->CK);
	xpd_puts("Reception Mode Enabled.\n");
}

void MLX_Transmit_Mode(MLX_Pin_Set &pinSet)
{
	globalPin_write(OFF, &pinSet->RTB);
	globalPin_write(ON, &pinSet->MODE);
	for(int i = 0; i < 3; i++)
	{
		MLX_Clk_Low(pinSet);
		sys_clock_wait( pinSet->periodTicks / 2 );
		MLX_Clk_High(pinSet);
		sys_clock_wait( pinSet->periodTicks / 2 );
	}
	xpd_puts("Transmission Mode Enabled.\n");
}

void MLX_Global_Write(int data, int size, int periodTicks, MLX_Pin_Set &pinSet)
{
	for( int i = addressSize - 1; i > 0; i-- )
	{
		MLX_Clk_Low(pinSet);
		globalPin_write( (data >> i)&1, &pinSet->DIN );
		sys_clock_wait( periodTicks / 2 );
		MLX_Clk_High(pinSet);
		sys_clock_wait( periodTicks / 2);
	}
}

void MLX_Config(MLX_Pin_Set &pinSet)
{
	MLX_Config_Mode(pinSet);
	// i, in this case, is the address of the register
	for( int i = 0; i < 13; i++ )
	{
		MLX_Global_Write( i, MLX_ADDRESS_SIZE, CONFIG_PERIOD, pinSet );
		MLX_Global_Write( Config_Data[i], MLX_DATA_SIZE, CONFIG_PREIOD, pinSet );
		sys_clock_wait(FIVE_US);
	}
}

void MLX_Setup_Pin(GlobalPin &pin, GlobalPin_dir dir)
{
	globalPin_set_dir(dir, &pin);
	io_set_config(DEFAULT_IO_CFG, pin->io_port);
}

/* Assumes that a clock has already been initialized 
*  to 49 152 MHz in the microcontroller.
*/
void MLX_Initialize(MLX_Pin_Set &pinSet)
{
	MLX_Setup_Pin(&pinSet->DIN, PinDir_Output);
	MLX_Setup_Pin(&pinSet->DOUT, PinDir_Input);
	
	MLX_Setup_Pin(&pinSet->MODE, PinDir_Output);
	MLX_Setup_Pin(&pinSet->RTB, PinDir_Output);
	
	MLX_Setup_Pin(&pinSet->DSYNC, PinDir_Input);
	MLX_Setup_Pin(&pinSet->CK, PinDir_Input);
	
	MLX_Config(pinSet);

	MLX_Data_Low(pinSet);	
}