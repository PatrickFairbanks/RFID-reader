//
//  SPI.h
//  skaa
//
//  Created by Sam Cristall on 2014-07-18.
//
//
// License: GPLv3

#ifndef SKAA_COMMON_SPI_H_
#define SKAA_COMMON_SPI_H_

#include <SPI.h>

#include "OutputPin.h"
#include "Utility.h"
#include "SysClock.h"

INTERFACE(ISerialPortInterface) {
public:
  void PushByte(uint16_t value) {STATIC_DISPATCH(PushByte(value));}
  uint16_t GetByte() { return STATIC_DISPATCH(GetByte()); }
private:
};

template <SPI_name name>
class SerialPortInterface
    : public ISerialPortInterface<SerialPortInterface<name>> {

static_assert(name == SPI1, "Cannot currently configure SPI0 (XPD)!");

public:
  SerialPortInterface(SystemFrequency sf) {
    SPI_set_config(GetSpiConfig(sf), name);
  }

  void PushByte(uint16_t value) { SPI_write(value, name); }

  uint16_t GetByte() { return SPI_read(name); }

private:
  constexpr int GetSpiConfig(SystemFrequency SystemFreq) const {
    return 
        (SystemFreq == SystemFrequency::_36_864_MHz ) ? 0x0007 : 
        (SystemFreq == SystemFrequency::_49_152_MHz ) ? 0x000B : 
        (SystemFreq == SystemFrequency::_73_728_MHz ) ? 0x000B : 
            0;    //should never get 0
  }
};

template <typename SPITy, typename CSPinTy>
class SpiWithChipSelect {
public:
  using SPI = ISerialPortInterface<SPITy>;
  using CSPin = IOutputPin<CSPinTy>;
  
  constexpr SpiWithChipSelect(SPI& spi, CSPin &cs) : spi_(spi), cs_(cs) {}

  template <class...Args>
  void Push(Args... args) {
    cs_.Set(false);
    PushImpl(args...);
    cs_.Set(true);
  }

  template <class...Args>
  uint16_t Get(Args... args) {
    cs_.Set(false);
    PushImpl(args...);
    uint16_t result = spi_.GetByte();
    cs_.Set(true);
    return result;
  }

private:
  template <class... Rest>
  void PushImpl(uint16_t value, Rest... rest) {
    spi_.PushByte(value);
    PushImpl(rest...);
  }
  void PushImpl() {
  }
  
  SPI& spi_;
  CSPin &cs_;
};

template <class S, class C>
constexpr SpiWithChipSelect<S, C> MakeSpiWithChipSelect(S& spi, C& cs) {
  return SpiWithChipSelect<S, C>(spi, cs);
}

#endif
