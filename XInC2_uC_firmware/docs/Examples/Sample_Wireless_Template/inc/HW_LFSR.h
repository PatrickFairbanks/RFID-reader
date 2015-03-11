//
//  LFSR.h
//  skaa
//
//  Created by Sam Cristall on 2014-07-28.
//
//
// License: GPLv3

#ifndef SKAA_COMMON_LFSR_HPP_
#define SKAA_COMMON_LFSR_HPP_

#include <LFSR.h>

#include "Utility.h"

static const uint16_t kWordLength = 16;

INTERFACE(ILFSR) {
public:
  void Configure(LFSR_fb_mode mode, size_t ntaps, size_t bits, uint16_t poly) {
      STATIC_DISPATCH(Configure(mode, ntaps, bits, poly));
  }
  void Push(uint16_t value) { STATIC_DISPATCH(Push(value)); }
  void Set(uint16_t value) { STATIC_DISPATCH(Set(value)); }
  uint16_t Get() { return STATIC_DISPATCH(Get()); }
  
private:
};

template <enum LFSR_name name> class LFSR : public ILFSR<LFSR<name>> {
public:
  constexpr LFSR() {}

  LFSR(LFSR_fb_mode mode, size_t ntaps, size_t bits, uint16_t poly, uint16_t init) {
    Configure(mode, ntaps, bits, poly);
    Set(init);
  }
  void Configure(LFSR_fb_mode mode, size_t ntaps, size_t bits, uint16_t poly) {
    lfsr_set_config(ntaps - 1, bits - 1, mode, poly, name);
  }
  void Push(uint16_t value) {
    lfsr_write(value, LFSR_ACCESS_SEQUENTIAL, name);
  }
  void Set(uint16_t value) {
    lfsr_write(value, LFSR_ACCESS_ALL, name);
  }
  uint16_t Get() {
    return lfsr_read(LFSR_ACCESS_ALL, name);
  }
  
private:
};

using LFSR0 = LFSR<LFSR_NAME_0>;
using LFSR1 = LFSR<LFSR_NAME_1>;

#endif
