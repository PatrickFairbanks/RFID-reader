//--------------------------------------------------------------------------------------
// File: main.c
// Author: Andrew Coulthard
//
// Copyright (c) University of Alberta. All rights reserved.
//--------------------------------------------------------------------------------------
#include <IOConfig.h>
#include <Structs.h>
#include <XPD.h>
#include <Thread.h>

#include "time_funcs.h"
#include "struct_aliases.h"
#include "UART.h"
#include "WirelessThreads.h"

#define ERROR 0
#define TIMESTAMP 1

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


// Set the Data in pins High or Low. Just for readability
void dataHigh()
{
	globalPin_write(ON, &DIN);
}
void dataLow()
{
	globalPin_write(OFF, DIN);
}

//Transceiver Modes
//Switch To configuration Mode

void configMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(OFF, &MODE);
}
//Switch to Receive Mode
void receiveMode()
{
	globalPin_write(ON, &RTB);
	globalPin_write(ON, &MODE);
}
//Switch to Transmit Mode
void transmitMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(ON, &MODE);

}
//Delay before reading after Powering NFC tag. Should be 100 us
void tDelay()
{
	uint16_t startTime = timer_read();
	unint16_t stopTime = 1; // must be 0.1 milliseconds
	unint16_t ticks = 0;
	while (stopTime < TIMESTAMP){
		ticks = ticks + TIMESTAMP;
		stopTime = timer_read() - startTime();
		if (ticks > 10000){
			break;
		}

	}
}
//Wireless transmission Function.

int startConfig()
{
	//Set input and output pin directions
	globalPin_set_dir(PinDir_Output, &MODE);
	globalPin_set_dir(PinDir_Output, &RTB);
	globalPin_set_dir(PinDir_Output, &DIN);
	globalPin_set_dir(PinDir_Input, &CK);
	globalPin_set_dir(PinDir_Input, &DOUT);
	globalPin_set_dir(PinDir_Input, &DSYNC);

	io_set_config(DEFAULT_IO_CFG, &DIN);
	io_set_config(DEFAULT_TO_CFG, &DOUT);

	//dataHigh();
	//configMode();

	//figure out how to change to majority voting mode.

	//tModeDelay;
	//configMode();

	//Edit Other THings the same way
	dataLow();
}

//Not sure if I can have a function of this type
//If so, This function will, at first (hopefully for not long), will
//take a command from the bluetooth device read button and will
//use it to detect a tag, giving an error if not detected.

uint16_t nfcDetecion(*detectByte){
	
	uint16_t tagResponse = 0;

	transmitMode();
	tDelay();

	uart_write_byte(detectByte, &DIN);
	tDelay();

	receiveMode();
	tDelay();

	tagResponse = uart_read_byte(&DOUT);

	if (tagResponse != 0)
	{
		return(tagResponse);
	}
	else{
		return(ERROR);
	}
}



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
	//Data Transfer initializations

	uint16_t bluetoothCommand = 0;
	uint16_t tagResponse = 0;

	init();
	
		//while(1)
		//{
		//	test += 1;
		//	if( test > 3)
		//	{
		//	uart_write_byte('a', &btpair);
		//
		//	uint16_t in = uart_read_byte(&btpair);
		//	xpd_echo_int(test, xpd_flag_signeddecimal);
		//
		//	test = 0;
		//	}
		//}
	while (1){
		
		bluetoothCommand = uart_read_byte(&btRx);

		if (bluetoothCommand == 'a'){

			tagResponse = nfcDetection(&bluetoothCommand);

			if (tagResponse != ERROR){

				uart_write_byte('Receive Mode Active', &btTx);
			}
			else{
				uart_write_byte('You Fucked Up Bad', &btTx);
			}

		}
		else{
			uart_write_byte('You Fucked Up Bad', &btTx);
		}

	}
	return 0;
}
