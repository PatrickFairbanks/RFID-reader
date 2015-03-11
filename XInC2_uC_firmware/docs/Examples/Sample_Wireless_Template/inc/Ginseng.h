/* Author: Sam Cristall
 * License: GPLv3
 */
#ifndef SKAA_COMMON_GINSENG_H
#define SKAA_COMMON_GINSENG_H

#include "Port.h"
#include "OutputPin.h"
#include "HW_SPI.h"
#include "HW_BBU.h"
#include "HW_LFSR.h"
#include "CRC.h"
#include "FEC.h"

using RadioPort = PortE;
static const size_t kRadioChipSelectPinNumber = 0;

using RadioSpi = SerialPortInterface<SPI1>;

using RadioBbuPort = PortH;
static const size_t kBbuDirectionPinNumber = 2;

using RadioBbu = BaseBandUnit<BBU0>;

static const uint16_t kRadioStartWord = 0x217B;
static const uint16_t kRadioPreamble = 0x5555;
static const uint16_t kRadioTimeout = 200;


#endif
