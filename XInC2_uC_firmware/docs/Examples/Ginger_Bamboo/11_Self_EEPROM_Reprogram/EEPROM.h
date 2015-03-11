/*                  Copyright Eleven Engineering 2015                         */
/* Author: Thia Wyrod
 * License: GPLv3
 */
/** \file EEPROM.h
 *  \brief Interface for interacting with an EEPROM over SPI. The internal
 *  XInC2 EEPROM is available on SPI0, and users may connect additional ones on
 *  SPI1.
 */
#ifndef _SXC_LIB_EEPROM_H_
#define _SXC_LIB_EEPROM_H_

#include "SPI.h"
#include "Structs.h"

enum EepromCommands {
  NOP = 0,
  WRITE = 2,
  READ = 3,
  WRITE_DISABLE = 4,
  READ_STATUS_REGISTER = 5,
  WRITE_ENABLE = 6,
};

GlobalPin const kInternalChipSelect = { GPIO_B, io_PB0, 0x1, Polar_ActiveLow };
const uint16_t kInternalRAMOffset = 0xC000;

/**
 * \brief Writes an 8-bit byte to a memory address in an EEPROM on the
 * designated SPI.
 * \param [in] spi Name of the SPI where the EEPROM is located: SPI0 always
 * connects to the internal EEPROM
 * \param [in] chip_select_pin GlobalPin to which the EEPROM's chip select line
 * is wired
 * \param [in] addr 16-bit memory address in the EEPROM to which to write
 * the value
 * \param [in] word Raw 16-bit value to write to the EEPROM
 */
SXC_INLINE void EEPROM_write_byte(enum SPI_name spi,
  GlobalPin const * chip_select_pin, uint16_t* addr, uint16_t word )
{
  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)WRITE_ENABLE, spi);
  globalPin_write(OFF, chip_select_pin);

  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)WRITE, spi);
  SPI_write((_rol((uint16_t)addr, 8)), spi);
  SPI_write((uint16_t)addr, spi);
  SPI_write(word, spi);
  globalPin_write(OFF, chip_select_pin);

  uint16_t writing = 1;
  while (writing) {
    globalPin_write(ON, chip_select_pin);
    SPI_write((uint16_t)READ_STATUS_REGISTER, spi);
    writing = SPI_read(spi) & 0x1;
    globalPin_write(OFF, chip_select_pin);
  }
}

/**
 * \brief Writes a 16-bit word to a memory address in an EEPROM on the
 * designated SPI.
 * \param [in] spi Name of the SPI where the EEPROM is located: SPI0 always
 * connects to the internal EEPROM
 * \param [in] chip_select_pin GlobalPin to which the EEPROM's chip select line
 * is wired
 * \param [in] addr 16-bit memory address in the EEPROM to which to write
 * the value
 * \param [in] word Raw 16-bit value to write to the EEPROM
 */
SXC_INLINE void EEPROM_write_word(enum SPI_name spi,
  GlobalPin const * chip_select_pin, uint16_t* addr, uint16_t word )
{
  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)WRITE_ENABLE, spi);
  globalPin_write(OFF, chip_select_pin);

  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)WRITE, spi);
  SPI_write((_rol((uint16_t)addr, 8)), spi);
  SPI_write((uint16_t)addr, spi);
  SPI_write(word, spi);
  SPI_write((_rol(word, 8)), spi);
  globalPin_write(OFF, chip_select_pin);

  uint16_t writing = 1;
  while (writing) {
    globalPin_write(ON, chip_select_pin);
    SPI_write((uint16_t)READ_STATUS_REGISTER, spi);
    writing = SPI_read(spi) & 0x1;
    globalPin_write(OFF, chip_select_pin);
  }
}

/**
 * \brief Reads an 8-bit byte from a memory address in an EEPROM on the
 * designated SPI.
 * \param [in] spi Name of the SPI where the EEPROM is located: SPI0 always
 * connects to the internal EEPROM
 * \param [in] chip_select_pin GlobalPin to which the EEPROM's chip select line
 * is wired
 * \param [in] addr 16-bit memory address in the EEPROM to which to write
 * the value
 * \return Raw 8-bit value read from the EEPROM
 */
SXC_INLINE uint16_t EEPROM_read_byte(enum SPI_name spi,
  GlobalPin const * chip_select_pin, uint16_t* addr)
{
  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)READ, spi);
  SPI_write((_rol((uint16_t)addr, 8)), spi);
  SPI_write((uint16_t)addr, spi);
  uint16_t value = SPI_read(spi);
  globalPin_write(OFF, chip_select_pin);
  return value;
}

/**
 * \brief Reads a 16-bit word from a memory address in an EEPROM on the
 * designated SPI.
 * \param [in] spi Name of the SPI where the EEPROM is located: SPI0 always
 * connects to the internral EEPROM
 * \param [in] chip_select_pin GlobalPin to which the EEPROM's chip select line
 * is wired
 * \param [in] addr 16-bit memory address in the EEPROM to which to write
 * the value
 * \return Raw 16-bit value read from the EEPROM
 */
SXC_INLINE uint16_t EEPROM_read_word(enum SPI_name spi,
  GlobalPin const * chip_select_pin, uint16_t* addr)
{
  globalPin_write(ON, chip_select_pin);
  SPI_write((uint16_t)READ, spi);
  SPI_write((_rol((uint16_t)addr, 8)), spi);
  SPI_write((uint16_t)addr, spi);
  uint16_t value = SPI_read(spi);
  value |= _rol(SPI_read(spi), 8);
  globalPin_write(OFF, chip_select_pin);
  return value;
}
#endif //_SXC_LIB_EEPROM_H_
