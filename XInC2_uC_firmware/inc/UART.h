/*                  Copyright Eleven Engineering 2014                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file UART.h
 *  \brief Convenience functions and implementation using 2 arbitrary GPIO pins
 *  in order to utilize a UART protocol.
 */
#ifndef _SXC_LIB_UART_H_
#define _SXC_LIB_UART_H_

#include "SXC.h"
#include "SFU.h"
#include "GPIO.h"
#include "SystemClock.h"
#include "Structs.h"

typedef struct UART_Pin_Pair {
  GlobalPin const rx;
  GlobalPin const tx;
  uint16_t const timeout;
} UART_Pin_Pair;


/**
 * \brief Transmits the LSB of the input word as a byte over a UART tx pin.
 * \param [in] byte 16-bit word whose LSB is transmitted. MSB is ignored.
 * \param [in] pins Pointer to the UART_Pin_Pair of interest
 */
SXC_INLINE void uart_write_byte(uint16_t byte, UART_Pin_Pair const * pins)
{
  globalPin_write_raw(0, &pins->tx); //write the start bit
  sys_clock_wait(pins->timeout);
  //Write bits, starting with lsb
  for (int16_t i = 0; i <= 7; ++i) {
    if (_rol(1, i) & byte)
      globalPin_write_raw(1, &pins->tx);
    else
      globalPin_write_raw(0, &pins->tx);
    sys_clock_wait(pins->timeout);
  }
  globalPin_write_raw(1, &pins->tx); //write the stop bit
  sys_clock_wait(pins->timeout);
}

/**
 * \brief Reads an 8-bit byte from the rx pin of a UART pair.
 * \param [in] pins Pointer to the UART_Pin_Pair of interest
 * \return 8-bit byte read from the UART rx, in the LSB of a 16-bit word
 */
SXC_INLINE uint16_t uart_read_byte(UART_Pin_Pair const * pins)
{
  uint16_t byte = 0;
  while(globalPin_read_raw(&pins->rx)); //wait until the start bit is written (0)
  sys_clock_wait(pins->timeout);
  //Read bits, starting with lsb
  for (int16_t i = 0; i <= 7; ++i) {
    byte += _rol(globalPin_read_raw(&pins->rx), i);
    sys_clock_wait(pins->timeout);
  }
  while(!globalPin_read_raw(&pins->rx)); //wait until stop bit is written (1)
  return byte;
}

/**
 * \brief Transmits the LSBs of an array of input words over a UART tx pin.
 * \param [in] buf Array of 16-bit words whose LSBs are written. MSBs are ignored.
 * \param [in] len Length of the array (number of bytes to write)
 * \param [in] pins Pointer to the UART_Pin_Pair of interest
 */
SXC_INLINE void uart_write_buf(uint16_t* buf, size_t len,
  UART_Pin_Pair const * pins)
{
  for (size_t i = 0; i < len; ++i)
    uart_write_byte(buf[i], pins);
}

#endif //_SXC_LIB_UART_H_
