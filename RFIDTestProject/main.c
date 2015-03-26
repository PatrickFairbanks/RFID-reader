//--------------------------------------------------------------------------------------
// File: main.c
// Authors: Andrew Coulthard And Patrick Fairbanks
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

#define ERROR 0
#define TIMESTAMP 1
#define configSize 12

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
static const UART_Pin_Pair antPair = { { GPIO_C, io_PC7, 0x8, Polar_ActiveHigh }, { GPIO_C, io_PC6, 0x7, Polar_ActiveHigh }, 0 };
static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh }, { GPIO_J, io_PJ, 3, Polar_ActiveHigh }, 0 };

// Set the Data in pins High or Low. Just for readability
bool detectDSYNC()
{
	
}
void writeISOConfig(int Address, int Data){
	configMode();
	Address = Address << 12;
	for (i = 3; i >= 0; i--)
	{
		ckLow();
		globalPin_write(Address << &1, &DIN);
		wait(ticks);
		ckHigh();
		wait(ticks);
	}


}
void transmitInit()
{
	ckLow();
	wait;
	ckHigh();
	wait
	ckLow();
	wait;
	ckHigh();
	wait;
	ckLow();
	wait;
	ckHigh();
	wait;
}
void dataHigh()
{
	globalPin_write(ON, &DIN);
	xpd_puts("Data In High.\n");
}
void dataLow()
{
	globalPin_write(OFF, &DIN);
	xpd_puts("Data In Low.\n");
}
void ckHigh(){
	globalPin_write(ON, &CK);
	xpd_puts("Clock High.\n");
}
void ckLow(){
	globalPin_write(OFF, &CK);
	xpd_puts("Clock Low.\n");
}
//Transceiver Modes
//Switch To configuration Mode

void configMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(OFF, &MODE);
	xpd_puts("Configuration Mode Enabled.\n");
}
//Switch to Receive Mode
void receiveMode()
{
	globalPin_write(ON, &RTB);
	globalPin_write(ON, &MODE);
	globalPin_write(ON, &DIN);
	globalPin_write(OFF, &CK);
	xpd_puts("Reception Mode Enabled.\n");
}
//Switch to Transmit Mode
void transmitMode()
{
	globalPin_write(OFF, &RTB);
	globalPin_write(ON, &MODE);
	xpd_puts("Transmission Mode Enabled.\n");
}

void writeISOConfig(){
	configMode();
	ckHigh();
	


}


//Delay before reading after Powering NFC tag. Should be 100 us
void tDelay()
{
	//uint16_t startTime = timer_read();
	//uint16_t stopTime = 1; // must be 0.1 milliseconds
	//uint16_t ticks = 0;
	//while (stopTime < TIMESTAMP){
	//	ticks = ticks + TIMESTAMP;
	//	stopTime = timer_read() - startTime();
	//	if (ticks > 10000){
	//		break;
	//	}

	//}
}

//this function is used to detect a tag.

uint16_t nfcDetection(uint16_t *detectByte){
	
	uint16_t tagResponse = 0;

	transmitMode();
	ckHigh();

	uart_write_byte(*detectByte, &antPair);
	ckLow();
	tDelay();

	receiveMode();
	tDelay();

	tagResponse = uart_read_byte(&antPair);

	if (tagResponse != 0)
	{
		return(tagResponse);
	}
	else
	{
		xpd_puts("fuuuuuck.\n");
		return(ERROR);
	}
}


//static const UART_Pin_Pair antPair = { { GPIO_C, io_PC7, 0x8, Polar_ActiveHigh }, { GPIO_C, io_PC6, 0x7, Polar_ActiveHigh }, 0 };
//static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 2, Polar_ActiveHigh },  { GPIO_J, io_PJ, 3 ,  Polar_ActiveHigh }, 0 };

void init()
{
	globalPin_set_dir(PinDir_Output, &btTx);
	io_set_config(DEFAULT_IO_CFG, io_PJ);
	
	globalPin_set_dir(PinDir_Input, &btRx);
	io_set_config(DEFAULT_IO_CFG, io_PJ);

	globalPin_set_dir(PinDir_Output, &DIN);
	io_set_config(DEFAULT_IO_CFG, io_PC7);

	globalPin_set_dir(PinDir_Input, &DOUT);
	io_set_config(DEFAULT_IO_CFG, io_PC6);

	globalPin_set_dir(PinDir_Output, &MODE);
	globalPin_set_dir(PinDir_Output, &RTB);
	globalPin_set_dir(PinDir_Input, &DSYNC);
	globalPin_set_dir(PinDir_Input, &CK);

	dataLow(); //Default - Forget if it is low or high, but based on logic, to send data DIN
			   // Must not be set high.
	transmitMode();

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
		
		bluetoothCommand = uart_read_byte(&btPair);

		if (bluetoothCommand == 'a'){

			tagResponse = nfcDetection(&bluetoothCommand);

			if (tagResponse != ERROR){

				uart_write_byte('R', &btPair);
			}else{
				xpd_puts("son of a.\n");
			}

		}else{
			xpd_puts("fuuuu still.\n");
		}
		//if (bluetoothCommand == 'b'){
		//	tagResponse = nfcDetection(&bluetoothCommand);
		//	if (tagResponse != ERROR){
		//		uart_write_byte('Transmit Mode Active, Transmitting Data', &btTx);
		//	}
		//	else{

		//	}
		//}

	}
	return 0;
}