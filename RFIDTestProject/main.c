//--------------------------------------------------------------------------------------
// File: main.c
// Authors: Patrick Fairbanks
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
#define MLX_ADDRESS_SIZE 4
#define MLX_DATA_SIZE 8
#define NFC_ADDRESS_SIZE 16
#define NFC_DATA_SIZE 64
#define FIVE_DELAY 210
#define HUNDRED_DELAY 4195

static const int periodTicks = 49152000 / 106000;

static const GlobalPin btTx =  { GPIO_J, io_PJ, 1<<3,  Polar_ActiveHigh }; // J3
static const GlobalPin btRx = { GPIO_J, io_PJ, 1<<2, Polar_ActiveHigh }; // J2

//Pin Declarations

static const GlobalPin DIN = { GPIO_C, io_PC7, 1<<7, Polar_ActiveHigh }; //C7
static const GlobalPin DOUT = { GPIO_C, io_PC6, 1<<6, Polar_ActiveHigh }; //C6
static const GlobalPin DSYNC = { GPIO_C, io_PC5, 1<<5, Polar_ActiveHigh }; //C5
static const GlobalPin CK = { GPIO_C, io_PC4, 1<<4, Polar_ActiveHigh }; //C4
static const GlobalPin MODE = { GPIO_D, io_PD3, 1<<3, Polar_ActiveHigh }; //D3
static const GlobalPin RTB = { GPIO_D, io_PD2, 1<<2, Polar_ActiveHigh }; //D2
static const UART_Pin_Pair antPair = { { GPIO_C, io_PC7, 1<<7, Polar_ActiveHigh }, { GPIO_C, io_PC6, 1<<6, Polar_ActiveHigh }, 0 };
static const UART_Pin_Pair btPair = { { GPIO_J, io_PJ, 1<<3, Polar_ActiveHigh }, { GPIO_J, io_PJ, 1<<2, Polar_ActiveHigh }, 0 };

//Edit this function for reception timing

//bool detectDSYNC()
//{
//}

//DIN high and low

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

//CK high and low

void ckHigh()
{
	globalPin_write(ON, &CK);
	xpd_puts("Clock High.\n");
}
void ckLow()
{
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
	ckLow();
	sys_clock_wait(periodTicks / 2);
	ckHigh();
	sys_clock_wait(periodTicks / 2);
	ckLow();
	sys_clock_wait(periodTicks / 2);
	ckHigh();
	sys_clock_wait(periodTicks / 2);
	ckLow();
	sys_clock_wait(periodTicks / 2);
	ckHigh();
	sys_clock_wait(periodTicks / 2);
	xpd_puts("Transmission Mode Enabled.\n");
}


//global write function. The bitShift is what decides whether the data
//is being written to config the transceiver or send to the transponder.

void globalWriteTx(int address, int addressSize, int data, int dataSize)
{	
	for (int i = addressSize-1; i > 0; i--)
	{
		ckLow();
		globalPin_write((address >> i)&1, &DIN);
		sys_clock_wait(periodTicks / 2);
		ckHigh();
		sys_clock_wait(periodTicks / 2);
	}

	for (int i = dataSize - 1; i > 0; i--)
	{
		ckLow();
		globalPin_write((data >> i) & 1, &DIN);
		sys_clock_wait(periodTicks / 2);
		ckHigh();
		sys_clock_wait(periodTicks / 2);
	}
}

//Delay before reading after Powering NFC tag. Should be 100 us

//this function is used to detect a tag.
//EDIT USING THE isoWRITE();

//Set Up MLX Transceiver for 14443-B mode

void MLX_Config()
{
	configMode();
	globalWriteTx(0, MLX_ADDRESS_SIZE, 0x73, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(1, MLX_ADDRESS_SIZE, 0x01, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(2, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(3, MLX_ADDRESS_SIZE, 0x0F, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(4, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(5, MLX_ADDRESS_SIZE, 0xFF, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(6, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(7, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(8, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(9, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(10, MLX_ADDRESS_SIZE, 0x03, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(11, MLX_ADDRESS_SIZE, 0x3F, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
	globalWriteTx(12, MLX_ADDRESS_SIZE, 0x00, MLX_DATA_SIZE);
	sys_clock_wait(FIVE_DELAY);
}


void init()
{
	init_clock();
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
	
	MLX_Config();

	dataLow(); //Default - Forget if it is low or high, but based on logic, to send data DIN
			   // Must not be set high.

}

int main(void)
{
	//Data Transfer initializations
	init();

	int testDetectData = 0xFFFF;
	int testDetectAddress = 0x06;

	while (1){

		transmitMode();
		sys_clock_wait(HUNDRED_DELAY*1000);
		xpd_puts("Transmitting now.");
		globalWriteTx(testDetectAddress, 8, testDetectData, 16);
		globalWriteTx(testDetectAddress, 0, testDetectData, 16);
		xpd_puts("Done transmitting.");
		sys_clock_wait(HUNDRED_DELAY);
		receiveMode();

		/*
		bluetoothCommand = uart_read_byte(&btPair);

		if (bluetoothCommand == "a"){

			tagResponse = nfcDetection(&bluetoothCommand);

			if (tagResponse != ERROR){

				uart_write_byte("R", &btPair);
			}else{
				xpd_puts("son of a.\n");
			}

		}else{
			xpd_puts("fuuuu still.\n");
		}*/
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