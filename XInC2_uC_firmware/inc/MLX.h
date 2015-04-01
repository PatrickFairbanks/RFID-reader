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
#define MLX_DATA_SIZE 					8
#define NFC_ADDRESS_SIZE				8
#define NFC_QUARTER_DATA_SIZE			16

#define CONFIG_PERIOD					65535 //58
#define TRANSMIT_PERIOD					58
#define RECEIVE_PERIOD					58

#define FIVE_US							246
#define ONE_HUNDRED_US					4195

#define TAGA_DIN  		{ GPIO_C, io_PC7, 1<<7, Polar_ActiveHigh } //C7
#define TAGA_DOUT  		{ GPIO_C, io_PC6, 1<<6, Polar_ActiveHigh } //C6
#define TAGA_DSYNC 		{ GPIO_C, io_PC5, 1<<5, Polar_ActiveHigh } //C5
#define TAGA_CK  		{ GPIO_C, io_PC4, 1<<4, Polar_ActiveHigh } //C4
#define TAGA_MODE  		{ GPIO_D, io_PD3, 1<<3, Polar_ActiveHigh } //D3
#define TAGA_RTB  		{ GPIO_D, io_PD3, 1<<2, Polar_ActiveHigh } //D2

#define TAGB_DIN  		{ GPIO_E, io_PE, 1<<7, Polar_ActiveHigh } //E7
#define TAGB_DOUT  		{ GPIO_E, io_PE, 1<<4, Polar_ActiveHigh } //E4
#define TAGB_DSYNC 		{ GPIO_E, io_PE, 1<<3, Polar_ActiveHigh } //E3
#define TAGB_CK  		{ GPIO_E, io_PE, 1<<2, Polar_ActiveHigh } //E2
#define TAGB_MODE  		{ GPIO_E, io_PE, 1<<1, Polar_ActiveHigh } //E1
#define TAGB_RTB  		{ GPIO_E, io_PE, 1<<6, Polar_ActiveHigh } //E6

int Config_Data[] = { 0x73, 0x01, 0x00, 0x0F, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x03, 0x3F, 0x00 };

typedef struct MLX_Pin_Set 
{
	GlobalPin const DIN;
	GlobalPin const DOUT;
	GlobalPin const DSYNC;
	GlobalPin const CK;
	GlobalPin const MODE;
	GlobalPin const RTB;	
} MLX_Pin_Set;

static const MLX_Pin_Set tagA = 
{
	TAGA_DIN,
	TAGA_DOUT,
	TAGA_DSYNC,
	TAGA_CK,
	TAGA_MODE,
	TAGA_RTB,
};

static const MLX_Pin_Set tagB = 
{
	TAGB_DIN,
	TAGB_DOUT,
	TAGB_DSYNC,
	TAGB_CK,
	TAGB_MODE,
	TAGB_RTB,
};

/* The following functions relate to serial data transfer,
*  for the data and clock lines of the transceiver
*/
void MLX_Data_High(MLX_Pin_Set const *pinSet)
{
	globalPin_write(ON, &pinSet->DIN);
}
void MLX_Data_Low(MLX_Pin_Set const *pinSet)
{
	globalPin_write(OFF, &pinSet->DIN);
}
void MLX_Clk_High(MLX_Pin_Set const *pinSet)
{
	globalPin_write(ON, &pinSet->CK);
}
void MLX_Clk_Low(MLX_Pin_Set const *pinSet)
{
	globalPin_write(OFF, &pinSet->CK);
}

void MLX_Config_Mode(MLX_Pin_Set const *pinSet)
{
	globalPin_write(OFF, &pinSet->RTB);
	globalPin_write(OFF, &pinSet->MODE);
	xpd_puts("Configuration Mode Enabled.\n");
}

void MLX_Receive_Mode(MLX_Pin_Set const *pinSet)
{
	sys_clock_wait(ONE_HUNDRED_US);
	globalPin_write(ON, &pinSet->RTB);
	globalPin_write(ON, &pinSet->MODE);
	globalPin_write(ON, &pinSet->DIN);
	globalPin_write(OFF, &pinSet->CK);
	xpd_puts("Reception Mode Enabled.\n");
}

void MLX_Transmit_Mode(MLX_Pin_Set const *pinSet)
{
	globalPin_write(OFF, &pinSet->RTB);
	globalPin_write(ON, &pinSet->MODE);
	globalPin_write(ON, &pinSet->DIN);                       //This needs to be on, not sure if it pulses with the clock.
	xpd_puts(" this is working ");
	for(int i = 0; i < 3; i++)
	{
		xpd_puts("Clk Low\n");
		MLX_Clk_Low(pinSet);
		for( int i = 0; i < 19200; i++ )
		{
			sys_clock_wait( TRANSMIT_PERIOD / 2 );
		}
		xpd_puts("Clk High\n");
		MLX_Clk_High(pinSet);
		for( int i = 0; i < 19200; i++ )
		{
			sys_clock_wait( TRANSMIT_PERIOD / 2 );
		}
	}
	xpd_puts("Transmission Mode Enabled.\n");
}

void MLX_Global_Write(int data, int size, int periodTicks, MLX_Pin_Set const *pinSet)
{
	globalPin_write(OFF, &pinSet->DIN);

	for( int i = size - 1; i > 0; i-- )
	{
		MLX_Clk_Low(pinSet);
		globalPin_write( (data >> i)&1, &pinSet->DIN );
		sys_clock_wait( periodTicks / 2 );
		MLX_Clk_High(pinSet);
		sys_clock_wait( periodTicks / 2);
	}

	globalPin_write(ON, &pinSet->DIN);
}
void MLX_Global_Receive(MLX_Pin_Set const *pinSet)
{

}

void MLX_Config(MLX_Pin_Set const *pinSet)
{
	MLX_Config_Mode(pinSet);
	// i, in this case, is the address of the register
	for( int i = 0; i < 13; i++ )
	{
		MLX_Global_Write( i, MLX_ADDRESS_SIZE, CONFIG_PERIOD, pinSet );
		MLX_Global_Write( Config_Data[i], MLX_DATA_SIZE, CONFIG_PERIOD, pinSet );
		sys_clock_wait(FIVE_US);
	}
}

void MLX_Setup_Pin(GlobalPin const *pin, enum GlobalPin_dir dir)
{
	globalPin_set_dir(dir, pin);
	io_set_config(DEFAULT_IO_CFG, pin->io_port);
}

/* Assumes that a clock has already been initialized 
*  to 49 152 MHz in the microcontroller.
*/
void MLX_Initialize(MLX_Pin_Set const *pinSet)
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

/*

// Code to test that all the pins are working 8/8 m8
	int periodTicks = 49152000 / 106000;
	
	// tag A
	
	GlobalPin DIN = 		{ GPIO_C, io_PC7, 1<<7, Polar_ActiveHigh }; //C7
	GlobalPin DOUT = 	{ GPIO_C, io_PC6, 1<<6, Polar_ActiveHigh }; //C6
	GlobalPin DSYNC = 	{ GPIO_C, io_PC5, 1<<5, Polar_ActiveHigh }; //C5
	GlobalPin CK = 		{ GPIO_C, io_PC4, 1<<4, Polar_ActiveHigh }; //C4
	GlobalPin MODE = 	{ GPIO_D, io_PD3, 1<<3, Polar_ActiveHigh }; //D3
	GlobalPin RTB = 		{ GPIO_D, io_PD3, 1<<2, Polar_ActiveHigh }; //D2
	
	
	// tag B
	GlobalPin DIN = 		{ GPIO_E, io_PE, 1<<7, Polar_ActiveHigh }; //E7
	GlobalPin DOUT = 	{ GPIO_E, io_PE, 1<<4, Polar_ActiveHigh }; //E4
	GlobalPin DSYNC = 	{ GPIO_E, io_PE, 1<<3, Polar_ActiveHigh }; //E3
	GlobalPin CK = 		{ GPIO_E, io_PE, 1<<2, Polar_ActiveHigh }; //E2
	GlobalPin MODE = 	{ GPIO_E, io_PE, 1<<1, Polar_ActiveHigh }; //E1
	GlobalPin RTB = 		{ GPIO_E, io_PE, 1<<6, Polar_ActiveHigh }; //E6
	
	GlobalPin pins[] = { DIN, DOUT, DSYNC, CK, MODE, RTB };
	
	for(int i = 0; i < 6; i++ )
	{
		globalPin_set_dir(PinDir_Output, &pins[i]);
		io_set_config(DEFAULT_IO_CFG, pins[i].io_port);
	}
	
		while(1)
		{
			xpd_puts("Writing 0\n");
			globalPin_write(0, &RTB);
			for(int i = 0; i < 6; i++)
			{
				globalPin_write(0, &pins[i]);
			}
			wait_ms(1000);
			xpd_puts("Writing 1\n");
			globalPin_write(1, &RTB);
			for(int i = 0; i < 6; i++)
			{
				globalPin_write(1, &pins[i]);
			}
			wait_ms(1000);
		}
		
		*/