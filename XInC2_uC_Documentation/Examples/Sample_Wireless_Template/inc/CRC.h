/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_CRC_H
#define SKAA_COMMON_CRC_H

#include "HW_LFSR.h"

static const uint16_t kCrcPolynomial = 0x1021;
//static const uint16_t kCrcInit = 0x0BA5; // Development CRC
static const uint16_t kCrcInit = 0xFFEA;

using Crc16Lfsr = LFSR<LFSR_NAME_0>;

template <class LFSR>
void ConfigureCrcEncoder(LFSR& lfsr) {
  lfsr.Configure(LFSR_FBMODE_ENCODE, kWordLength, kWordLength, kCrcPolynomial);
  lfsr.Set(kCrcInit);
}

template <class LFSR>
void ConfigureCrcDecoder(LFSR& lfsr) {
  ConfigureCrcEncoder(lfsr); // CRC is same in both cases
}

#endif
